#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#define cell	  size_t
#define CELL_MAX  ULONG_MAX
#define CELL_STR  "%lu"

typedef uintptr_t llong;
typedef unsigned char boolean;
enum { FALSE, TRUE };


/**
 * Cell markings
 */
enum { UNMARKED, MARKED, PRIME };


/**
 * Bit set, a set of fixed number of bits/booleans
 */
typedef struct
{
	/**
	 * The set of all limbs, a limb consist of 64 bits
	 */
	llong* limbs;

	/**
	 * Index of the first non-zero limb
	 */
	ssize_t first;

	/**
	 * Array the the index of the previous non-zero limb for each limb
	 */
	ssize_t* prev;

	/**
	 * Array the the index of the next non-zero limb for each limb
	 */
	ssize_t* next;

} BitSet;



ssize_t** kuhn_match(cell** table, size_t n, size_t m);

static void BitSet_init(BitSet *this, size_t size);
static void BitSet_free(BitSet *this);
static void BitSet_set(BitSet *this, size_t i);
static void BitSet_unset(BitSet *this, size_t i);
static ssize_t BitSet_any(BitSet *this) __attribute__((pure));

static size_t lb(llong x) __attribute__((const));

/* @param   table  The table in which to perform the reduction
 * @param   marks	   The marking matrix
 * @param   colCovered  An array which tells whether a column is covered
 * @param   rowCovered  An array which tells whether a row is covered
 * @param  colMarks   Markings in the columns
 * @param  rowPrimes  Primes in the rows
 * @param   n  The table's height
 * @param   m  The table's width
 */
struct kuhn_data {
	size_t n, m;
	cell** table;
	uint8_t** marks;
	ssize_t* rowPrimes;
	ssize_t* colMarks;
	boolean* rowCovered;
	boolean* colCovered;
};

static void kuhn_reduceRows(struct kuhn_data *k);
static size_t kuhn_markZeroes(struct kuhn_data *k);
static size_t kuhn_countColumns(struct kuhn_data *k);
static boolean kuhn_findPrime(struct kuhn_data *k, size_t* primeRow, size_t* primeCol);
static void kuhn_altMarks(struct kuhn_data *k, ssize_t currRow, ssize_t currCol);
static void kuhn_addAndSubtract(struct kuhn_data *k);
static ssize_t** kuhn_assign(struct kuhn_data *k);

/**
 * Calculates an optimal bipartite minimum weight matching using an
 * O(n³)-time implementation of The Hungarian Algorithm, also known
 * as Kuhn's Algorithm.
 *
 * @param   table  The table in which to perform the matching
 * @param   n	  The height of the table
 * @param   m	  The width of the table
 * @return		 The optimal assignment, an array of row–coloumn pairs
 */
ssize_t** kuhn_match(cell** table, size_t n, size_t m)
{
	size_t i;
	struct kuhn_data k;

	/* not copying table since it will only be used once */
	k.n = n;
	k.m = m;
	k.table = table;

	k.rowPrimes = malloc(n * sizeof(ssize_t));
	k.colMarks  = malloc(m * sizeof(ssize_t));

	k.rowCovered = calloc(n, sizeof(boolean));
	k.colCovered = calloc(m, sizeof(boolean));

	k.marks = malloc(n * sizeof(uint8_t*));
	for (i = 0; i < n; i++)
		k.marks[i] = calloc(m, sizeof(uint8_t));

	kuhn_reduceRows(&k);
	if (kuhn_markZeroes(&k) < n) {
		do {
			size_t primeRow, primeCol;
			while (!kuhn_findPrime(&k, &primeRow, &primeCol))
			kuhn_addAndSubtract(&k);

		kuhn_altMarks(&k, primeRow, primeCol);
		} while (kuhn_countColumns(&k) < n);
	}

	free(k.rowCovered);
	free(k.colCovered);
	free(k.rowPrimes);
	free(k.colMarks);

	ssize_t** rc = kuhn_assign(&k);

	for (i = 0; i < n; i++)
		free(k.marks[i]);
	free(k.marks);

	return rc;
}


/**
 * Reduces the values on each rows so that, for each row, the
 * lowest cells value is zero, and all cells' values is decrease
 * with the same value [the minium value in the row].
 *
 * @param k The data from kuhn_match
 */
void kuhn_reduceRows(struct kuhn_data *k)
{
	size_t i, j;
	cell min;
	cell* ti;
	for (i = 0; i < k->n; i++) {
		ti = k->table[i];
		min = ti[0];
	for (j = 1; j < k->m; j++)
		if (min > ti[j])
			min = ti[j];
	
	for (j = 0; j < k->m; j++)
		ti[j] -= min;
	}
}


/**
 * Fill a matrix with marking of cells in the table whose
 * value is zero [minimal for the row]. Each marking will
 * be on an unique row and an unique column.
 *
 * @param k The data from kuhn_match
 * @return  The number of covered columns
 */
size_t kuhn_markZeroes(struct kuhn_data *k)
{
	size_t i, j;
	size_t count = 0;

	for (i = 0; i < k->n; i++)
		for (j = 0; j < k->m; j++)
		if (!k->colCovered[j] && k->table[i][j] == 0) {
			k->marks[i][j] = MARKED;
		k->colCovered[j] = TRUE;
				count++;
				break;
		}

	return count;
}


/**
 * Determines whether the marking is complete, that is
 * if each row has a marking which is on a unique column.
 * Find covered columns while at it.
 *
 * @param k The data from kuhn_match
 * @return			  Number of rows with a mark
 */
size_t kuhn_countColumns(struct kuhn_data *k)
{
	size_t i, j;
	size_t count = 0;

	memset(k->colCovered, 0, k->m);
	for (i = 0; i < k->n; i++)
		for (j = 0; j < k->m; j++)
		if (!k->colCovered[j] && k->marks[i][j] == MARKED) {
			k->colCovered[j] = TRUE;
				count++;
		break;
		}

	return count;
}


/**
 * Finds a prime
 *
 * @param k The data from kuhn_match
 * @param primeRow	The row of the found prime
 * @param primeCol	The column of the found prime
 * @return			  The row and column of the found print, <code>NULL</code> will be returned if none can be found
 */
boolean kuhn_findPrime(struct kuhn_data *k, size_t* primeRow, size_t* primeCol)
{
	size_t i, j;
	size_t row, col;
	BitSet zeroes;

	BitSet_init(&zeroes, k->n * k->m);

	for (i = 0; i < k->n; i++)
	for (j = 0; j < k->m; j++)
		if (!k->colCovered[j] && k->table[i][j] == 0)
		BitSet_set(&zeroes, i * k->m + j);

	memset(k->rowCovered, 0, k->n);
	for (;;) {
		ssize_t p = BitSet_any(&zeroes);
	if (p < 0) {
		BitSet_free(&zeroes);
		return FALSE;
	}
	
	row = (size_t)p / k->m;
	col = (size_t)p % k->m;
	
	k->marks[row][col] = PRIME;
	
	for (j = 0; j < k->m; j++)
		if (k->marks[row][j] == MARKED) {
		col = j;
				break;
		}
	
	/* No other marks?  We're done.  */
	if (j == k->m)
			break;

	BitSet_unset(&zeroes, p);
	k->rowCovered[row] = TRUE;
	k->colCovered[col] = FALSE;
	
	/* Add zeroes to the bitmap for the now-uncovered column.  */
	for (i = 0; i < k->n; i++)
		if (row != i && k->table[i][col] == 0) {
			if (!k->rowCovered[i])
				BitSet_set(&zeroes, i * k->m + col);
			else
				BitSet_unset(&zeroes, i * k->m + col);
		}
	
	/* Remove zeroes from the bitmap for the now-covered row.  */
	for (j = 0; j < k->m; j++)
		if (col != j && k->table[row][j] == 0)
			BitSet_unset(&zeroes, row * k->m + j);
	
	}

	*primeRow = row;
	*primeCol = col;
	BitSet_free(&zeroes);
	return TRUE;
}


static inline void kuhn_altMark(struct kuhn_data *k, ssize_t currRow, ssize_t currCol)
{
	if (k->marks[currRow][currCol] == MARKED)
		k->marks[currRow][currCol] = UNMARKED;
	else
		k->marks[currRow][currCol] = MARKED;
}

/**
 * Removes all prime marks and modifies the marking
 *
 * @param k The data from kuhn_match
 * @param  currRow	The row of the last found prime
 * @param  currCol	The column of the last found prime
 */
void kuhn_altMarks(struct kuhn_data *k, ssize_t currRow, ssize_t currCol)
{
	size_t i, j;

	for (i = 0; i < k->n; i++)
		k->rowPrimes[i] = -1;
	for (j = 0; j < k->m; j++)
		k->colMarks[j] = -1;

	for (i = 0; i < k->n; i++)
		for (j = 0; j < k->m; j++)
		if (k->marks[i][j] == MARKED)
			k->colMarks[j] = (ssize_t)i;
		else if (k->marks[i][j] == PRIME)
			k->rowPrimes[i] = (ssize_t)j;

	kuhn_altMark(k, currRow, currCol);
	for (;;) {
		currRow = k->colMarks[currCol];
	if (currRow < 0)
		break;
		kuhn_altMark(k, currRow, currCol);
	
	currCol = k->rowPrimes[currRow];
		assert(currCol >= 0);
		kuhn_altMark(k, currRow, currCol);
	}

	for (i = 0; i < k->n; i++) {
		uint8_t *marksi = k->marks[i];
		for (j = 0; j < k->m; j++)
		if (marksi[j] == PRIME)
			marksi[j] = UNMARKED;
	}
}


/**
 * Depending on whether the cells' rows and columns are covered,
 * the the minimum value in the table is added, subtracted or
 * neither from the cells.
 *
 * @param k The data from kuhn_match
 */
void kuhn_addAndSubtract(struct kuhn_data *k)
{
	size_t i, j;
	cell min = CELL_MAX;
	for (i = 0; i < k->n; i++)
		if (!k->rowCovered[i])
		for (j = 0; j < k->m; j++)
			if (!k->colCovered[j] && min > k->table[i][j])
			min = k->table[i][j];

	for (i = 0; i < k->n; i++) {
	if (k->rowCovered[i]) {
			for (j = 0; j < k->m; j++) {
			if (k->colCovered[j])
				k->table[i][j] += min;
			}
	} else {
			for (j = 0; j < k->m; j++) {
			if (!k->colCovered[j])
				k->table[i][j] -= min;
			}
		}
	}
}


/**
 * Creates a list of the assignment cells
 *
 * @param k The data from kuhn_match
 * @return		 The assignment, an array of row–coloumn pairs
 */
ssize_t** kuhn_assign(struct kuhn_data *k)
{
	ssize_t** assignment = malloc(k->n * sizeof(ssize_t*));

	size_t i, j;
	for (i = 0; i < k->n; i++) {
		assignment[i] = malloc(2 * sizeof(ssize_t));
		for (j = 0; j < k->m; j++)
		if (k->marks[i][j] == MARKED) {
		assignment[i][0] = (ssize_t)i;
		assignment[i][1] = (ssize_t)j;
		}
	}

	return assignment;
}


/**
 * Constructor for BitSet
 *
 * @param   size  The (fixed) number of bits to bit set should contain
 * @return		The a unique BitSet instance with the specified size
 */
void BitSet_init(BitSet *this, size_t size)
{
	size_t c = size >> 6L;
	if (size & 63L)
		c++;

	this->limbs = calloc(c, sizeof(llong));
	this->prev = calloc(c, sizeof(size_t));
	this->next = calloc(c, sizeof(size_t));
	this->first = -1;
}

void BitSet_free(BitSet *this)
{
	free(this->limbs);
	free(this->prev);
	free(this->next);
}

/**
 * Turns on a bit in a bit set
 *
 * @param  this  The bit set
 * @param  i	 The index of the bit to turn on
 */
void BitSet_set(BitSet *this, size_t i)
{
	size_t j = i >> 6L;
	llong old = this->limbs[j];

	this->limbs[j] |= 1LL << (llong)(i & 63L);

	if (!old) {
		if (this->first != -1)
		this->prev[this->first] = j;
	this->prev[j] = -1;
	this->next[j] = this->first;
	this->first = j;
	}
}

/**
 * Turns off a bit in a bit set
 *
 * @param  this  The bit set
 * @param  i	 The index of the bit to turn off
 */
void BitSet_unset(BitSet *this, size_t i)
{
	size_t j = i >> 6L;
	llong old = this->limbs[j];

	if (!old)
		return;

	this->limbs[j] &= ~(1LL << (llong)(i & 63L));

	if (!this->limbs[j]) {
	size_t p = this->prev[j];
	size_t n = this->next[j];
		if (n != -1)
		this->prev[n] = p;
		if (p == -1)
		this->first = n;
		else
		this->next[p] = n;
	}
}

/**
 * Gets the index of any set bit in a bit set
 *
 * @param   this  The bit set
 * @return		The index of any set bit
 */
ssize_t BitSet_any(BitSet *this)
{
	ssize_t i = this->first;
	if (i == -1)
		return -1;

	return (ssize_t)(lb(this->limbs[i] & -this->limbs[i]) + (i << 6L));
}


/**
 * Calculates the floored binary logarithm of a positive integer
 *
 * @param   value  The integer whose logarithm to calculate
 * @return		 The floored binary logarithm of the integer
 */
size_t lb(llong value)
{
	size_t rc = 0;
	llong v = value;

	if (v & (int_fast64_t)0xFFFFFFFF00000000LL)  {  rc |= 32L;  v >>= 32LL;  }
	if (v & (int_fast64_t)0x00000000FFFF0000LL)  {  rc |= 16L;  v >>= 16LL;  }
	if (v & (int_fast64_t)0x000000000000FF00LL)  {  rc |=  8L;  v >>=  8LL;  }
	if (v & (int_fast64_t)0x00000000000000F0LL)  {  rc |=  4L;  v >>=  4LL;  }
	if (v & (int_fast64_t)0x000000000000000CLL)  {  rc |=  2L;  v >>=  2LL;  }
	if (v & (int_fast64_t)0x0000000000000002LL)	 rc |=  1L;

	return rc;
}


void print(cell** t, size_t n, size_t m, ssize_t** assignment);
void print(cell** t, size_t n, size_t m, ssize_t** assignment) {
	size_t i, j;

	ssize_t** assigned = malloc(n * sizeof(ssize_t*));
	for (i = 0; i < n; ++i) {
		assigned[i] = malloc(m * sizeof(ssize_t));
		for (j = 0; j < m; j++)
			assigned[i][j] = 0;
	}

	if (assignment != NULL) {
		for (i = 0; i < n; ++i)
			++(*(*(assigned + **(assignment + i)) + *(*(assignment + i) + 1)));
	}

	for (i = 0; i < n; ++i) {
		printf("    ");
		for (j = 0; j < m; ++j) {
			if (assigned[i][j])
				printf("\033[%im", (int)(30 + assigned[i][j]));
			printf(CELL_STR "%s\033[m ", t[i][j], assigned[i][j] ? "^" : " ");
		}
		printf("\n\n");

		free(assigned[i]);
	}

	free(assigned);
}

void specificTests(void);
void specificTests(void) {
	cell** tests[3] = {
		(cell*[]) {
			(cell[]){0, 0},
			(cell[]){0, 0}
		}, (cell*[]) {
			(cell[]){0, 0, 0, 0},
			(cell[]){0, 0, 0, 0},
			(cell[]){0, 0, 0, 0},
			(cell[]){0, 0, 0, 1}
		}, (cell*[]) {
			(cell[]){0, 0, 0, 0, 0},
			(cell[]){0, 0, 0, 0, 0},
			(cell[]){0, 0, 1, 1, 1},
			(cell[]){0, 0, 1, 1, 1},
			(cell[]){0, 0, 1, 1, 1}
		}
	};

	for (size_t test = 0; test < 3; ++test) {
		const size_t size = (size_t)(2.5f + (float)test * 1.5f);

		printf("Test %li\n\n", test + 1);
		print(tests[test], size, size, NULL);

		ssize_t** assignment = kuhn_match(tests[test], size, size);

		printf("Output:\n\n");
		print(tests[test], size, size, assignment);
	}
}

void speedTest(size_t count, size_t size);
void speedTest(size_t count, size_t size) {
	printf("==Speed Test (%lu %lux%lu)==\n", count, size, size);

	// allocate matrix
	cell** t = malloc(size * sizeof(cell*));
	for (size_t i = 0; i < size; ++i)
		t[i] = malloc(size * sizeof(cell));

	const size_t THOUSAND = 1000;
	const size_t MILLION = THOUSAND * THOUSAND;
	const size_t BILLION = MILLION * THOUSAND;
	const cell max_size = size * size;
	clock_t totalTime = 0;

	for (size_t total = 1; total <= count; ++total) {
		// fill with random values
		for (size_t i = 0; i < size; ++i) {
			for (size_t j = 0; j < size; ++j) {
				int num;
				do { num = rand();
				} while (num > (RAND_MAX - ((RAND_MAX % max_size) + 1) % max_size));
				t[i][j] = (cell)num % max_size;
			}
		}

		// print iteration count
		if (total < 12) printf("\n%lu", total);
		else if (total < 100 && total % 12 == 0) printf("\n%lu Dozen", total / 12);
		else if (total < THOUSAND && total % 100 == 0) printf("\n%lu Hundred", total / 100);
		else if (total < MILLION && total % THOUSAND == 0) printf("\n%lu Thousand", total / THOUSAND);
		else if (total < BILLION && total % MILLION == 0) printf("\n%lu Million", total / MILLION);
		else if (total % BILLION == 0) printf("\n%lu Billion", total / BILLION);

		clock_t start = clock();
		kuhn_match(t, size, size);
		clock_t end = clock();
		totalTime += end - start;
	}

	printf("\n\n%fs Average Time\n\n", ((double)totalTime / (double)count) / CLOCKS_PER_SEC);

	// free matrix
	for (size_t i = 0; i < size; ++i) free(t[i]);
	free(t);
}

int main(void) {
	srand((unsigned)time(NULL));

	specificTests();
	speedTest(10000,50);
	speedTest(100,250);
	speedTest(10,1000);

	return 0;
}
