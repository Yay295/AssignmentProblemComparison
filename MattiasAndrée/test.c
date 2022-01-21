#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "hungarian.c"


void specificTests(void) {
	Cell** tests[3] = {
		(Cell*[]) {
			(Cell[]){0, 0},
			(Cell[]){0, 0}
		}, (Cell*[]) {
			(Cell[]){0, 0, 0, 0},
			(Cell[]){0, 0, 0, 0},
			(Cell[]){0, 0, 0, 0},
			(Cell[]){0, 0, 0, 1}
		}, (Cell*[]) {
			(Cell[]){0, 0, 0, 0, 0},
			(Cell[]){0, 0, 0, 0, 0},
			(Cell[]){0, 0, 1, 1, 1},
			(Cell[]){0, 0, 1, 1, 1},
			(Cell[]){0, 0, 1, 1, 1}
		}
	};

	for (size_t test = 0; test < 3; ++test) {
		const size_t size = (size_t)(2.5f + (float)test * 1.5f);

		printf("Test %li\n\n", test + 1);
		print(size, size, tests[test], NULL);

		CellPosition* assignment = kuhn_match(size, size, tests[test]);

		printf("Output:\n\n");
		print(size, size, tests[test], assignment);
	}
}

void speedTest(size_t count, size_t size) {
	printf("==Speed Test (%lu %lux%lu)==\n", count, size, size);

	// allocate matrix
	Cell** t = malloc(size * sizeof(Cell*));
	for (size_t i = 0; i < size; ++i)
		t[i] = malloc(size * sizeof(Cell));

	const size_t THOUSAND = 1000;
	const size_t MILLION = THOUSAND * THOUSAND;
	const size_t BILLION = MILLION * THOUSAND;
	const Cell max_size = size * size;
	clock_t totalTime = 0;

	for (size_t total = 1; total <= count; ++total) {
		// fill with random values
		for (size_t i = 0; i < size; ++i) {
			for (size_t j = 0; j < size; ++j) {
				int num;
				do { num = rand();
				} while (num > (RAND_MAX - ((RAND_MAX % max_size) + 1) % max_size));
				t[i][j] = (Cell)num % max_size;
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
		kuhn_match(size, size, t);
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
