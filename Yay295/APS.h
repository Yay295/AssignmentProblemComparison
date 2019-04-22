/*
Assignment Problem Solver - Coded by Yay295
Last Updated July 19, 2018


Usage:
Create a new Assignment Problem Solver Object by initializing a new APSO.
The APSO takes three things to initialize:
	a pointer to your cost matrix,
	its width, and
	its height.
The special constructor takes a 1D C++ vector instead of a pointer. All values
must be integers, but the type and signedness do not matter (except for the
special constructor, which must be unsigned).

After the APSO has been constructed, it will contain a vector of APSOResult
objects, each of which contain the x and y value of an assigned cell. The top
left matrix index is position (0, 0).


Notes:
I use vector<char> in place of vector<bool>, because vector<bool>
is a bitset, which is not what I want.


Maximum RAM Usage = sizeof(cost matrix data-type) * (width * height)
					+ sizeof(char) * (3 * width)
					+ sizeof(bool)
					+ sizeof(pointer) * (25 + 6 * height)
This does not include the size of your original cost matrix,
any other variables in your program, any external libraries,
or the exe itself. This assumes width >= height. Just switch
them if that isn't true. If the special constructor is used,
the Maximum RAM Usage is the greater of the following:
1) sizeof(cost matrix data-type) * (width * height) + sizeof(pointer) * 3
2) sizeof(char) * (3 * width) + sizeof(bool) + sizeof(pointer) * (25 + 6 * height)
The first case will only occur if width < height.
*/


#pragma once


#ifndef APS
#define APS


#include <vector>
#include <type_traits>
#include <numeric>


#define V std::vector // This is undefined at the bottom.


struct APSOResult {
	size_t x, y;
	APSOResult(const size_t & X, const size_t & Y) : x(X), y(Y) {}
};


class APSO {
	public:

	V<APSOResult> results;


	// Note that since this class has no public member functions
	// (other than constructors), the only use for this empty constructor
	// is to create an APSO that can be assigned to other APSO's.
	APSO() {};


	template<typename T>
	APSO(const T * const newValues, const size_t newWidth, const size_t newHeight) {
		const size_t nSize = newWidth * newHeight;

		if (newWidth >= newHeight) { // width >= height -> do not transpose matrix
			if (std::is_signed<T>::value) {
				constexpr T max = std::numeric_limits<T>::max();
				V<typename std::make_unsigned<T>::type> values(nSize);
				std::transform(newValues, newValues + nSize, values.begin(), [max](T x){return x + max;});
				math(values, newWidth, newHeight, false);
			} else math(V<T>(newValues, newValues + nSize), newWidth, newHeight, false);
		} else math(transposeToUnsigned(newValues, newWidth, newHeight), newHeight, newWidth, true);
	}


	// This constructor is special in that it does not use a copy of the
	// matrix given to it, but the original matrix itself. This will use
	// less memory as well as increase performance. However, the matrix
	// given to it must already be in the form this class uses internally
	// - a C++ vector of an unsigned integer type.
	template<typename T>
	APSO(V<T> & newValues, const size_t newWidth, const size_t newHeight) {
		static_assert(!std::numeric_limits<T>::is_signed, "The value type used for the APSO's special constructor must be unsigned.");

		if (newWidth < newHeight) {
			// Okay, it does technically create a copy here,
			// but this copy is destroyed after it's been used.
			newValues = transposeToUnsigned(newValues.data(), newWidth, newHeight);
			math(newValues, newHeight, newWidth, true);
		} else math(newValues, newWidth, newHeight, false);
	}

	// Prints the results as a 2d matrix of 'O's and 'X's.
	void printResults(std::ostream & out) const {
		for (size_t column = 0; column < width; ++column) {
			for (size_t row = 0; row < height; ++row) {
				bool assigned = false;
				for (const auto & result : results) {
					if (result.x == column && result.y == row) {
						assigned = true;
						break;
					}
				}
				out << (assigned ? 'X' : 'O') << ' ';
			}
			out << '\n';
		}
	}

	// Calculates the cost of the chosen solution. The original matrix must be
	// passed in because it is not stored anywhere. Its width and height are
	// stored, so they do not have to be passed in again.
	template<typename T>
	T resultCost(const T * const values) const {
		T cost = 0;
		for (const auto & result : results)
			cost += values[width*result.y+result.x];
		return cost;
	}


	private:

	size_t width, height;


	template<typename T>
	auto transposeToUnsigned(const T * const input, const size_t width, const size_t height) const {
		// If the input values are signed, add their maximum possible (signed) value to each to
		// ensure they are positive. Because they are being made unsigned, this operation will not
		// cause overflow. Since this is a constexpr, it can be evaluated at compile time and
		// should prevent this operation from being done if the values are already unsigned.
		constexpr T max = std::numeric_limits<T>::is_signed ? std::numeric_limits<T>::max() : 0;

		V<typename std::make_unsigned<T>::type> values;
		values.reserve(width*height);

		for (size_t column = 0; column < width; ++column)
			for (size_t row = 0; row < height; ++row)
				values.push_back(input[row*width+column] + max);

		return values;
	}


	template<typename T>
	void math(V<T> && values, const size_t & newWidth, const size_t & newHeight, const bool flip) {
		// Why is this necessary...
		math(values, newWidth, newHeight, flip);
	}
	template<typename T>
	void math(V<T> & values, const size_t & newWidth, const size_t & newHeight, const bool flip) {
		static_assert(!std::numeric_limits<T>::is_signed, "A signed value type was passed to the APSO's math() function.");

		width = newWidth; height = newHeight;
		results.reserve(height);

		rowReduce(values);
		if (width == height) columnReduce(values);

		getResults(values);

		// This is true if the matrix was flipped. Matrices are flipped
		// if they are taller than they are wide. This improves speed and
		// decreases code size. The results are then flipped to be correct.
		if (flip) for (auto & result : results)
			std::swap(result.x, result.y);
	}


	template<typename T>
	void rowReduce(V<T> & values) const {
		size_t row, column;
		T min;

		for (row = 0; row < height; ++row) { // traverse rows
			T * rowPtr = &values[row*width];

			min = rowPtr[0];

			if (min) {
				for (column = 1; column < width; ++column) { // find smallest number in row
					if (rowPtr[column] < min) {
						min = rowPtr[column];
						if (!min) break;
					}
				}

				if (min) {
					for (column = 0; column < width; ++column) { // subtract all by that num
						rowPtr[column] -= min;
					}
				}
			}
		}
	}


	template<typename T>
	void columnReduce(V<T> & values) const {
		size_t column, row;
		T min;

		for (column = 0; column < width; ++column) { // traverse columns
			min = values[column];

			if (min) {
				for (row = 1; row < height; ++row) { // find smallest number in column
					if (values[row*width+column] < min) {
						min = values[row*width+column];
						if (!min) break;
					}
				}

				if (min) {
					for (row = 0; row < height; ++row) { // subtract all by that num
						values[row*width+column] -= min;
					}
				}
			}
		}
	}


	template<typename T>
	void getResults(V<T> & values) {
		// Here be pointers, goto statements, and recursion. Coders beware.

		/*
		Steps:
		1) Assign a zero in an unused row.
		2) If there is already an assignment in the same column as the newly assigned zero,
		-a) and that assignment is alone in its row, reassign the newly assigned
		zero to another zero in the same row and retry step 2.
		-b) reassign that zero to another zero in its row. Repeat step 2.
		3) If there are still assignments to be made, go to step 1.
		4) Else Return
		*/

		V<char> usedRows(height, false), usedColumns(width, false);
		size_t row = -1, column;

		while (true) {
			V<char> forStep2(height, false);

			step1:

			while (++row < height) {
				if (!usedRows[row]) {
					const T * rowPtr = &values[row*width];

					for (column = 0; column < width; ++column) { // skipping step 2
						if (!usedColumns[column] && !rowPtr[column]) {
							results.emplace_back(column,row);

							usedColumns[column] = true; usedRows[row] = true;

							if (results.size() != height) goto step1; // Step 3

							else return; // Step 4
						}
					}

					forStep2[row] = true;

					for (column = 0; column < width; ++column) { // using step 2
						if (!rowPtr[column]) {
							results.emplace_back(column,row);

							if (valueSwap(values, forStep2, usedColumns, column, row)) { // Step 2
								usedRows[row] = true;

								if (results.size() != height) goto step1; // Step 3

								else return; // Step 4
							}

							results.pop_back();
						}
					}
				}
			}

			drawLines(values);
			row = -1;
		}
	}


	template<typename T>
	bool valueSwap(const V<T> & values, V<char> & visitedRows, V<char> & usedColumns,
				   const size_t x, const size_t y) {
		// Values x and y are the position of the newly assigned zero.
		// There is no need to ever check already visited rows, so we skip them.

		/*
		Steps:
		2) If there is already an assignment in the same column as the newly assigned zero,
		-a) and that assignment is alone in its row, reassign the newly assigned
		zero to another zero in the same row and retry step 2.
		-b) reassign that zero to another zero in its row. Repeat step 2.
		*/

		// This function returns true if it can properly shift all
		// assignments so that there are not more than one in a row or
		// column. It returns false otherwise.


		APSOResult * conflict = nullptr;

		for (auto & result : results) { // find conflicting assignment
			if (result.x == x && !visitedRows[result.y]) {
				conflict = &result;
				break;
			}
		}

		if (conflict == nullptr) return false;


		size_t column;
		const T * rowPtr = &values[conflict->y*width];

		visitedRows[conflict->y] = true;


		for (column = 0; column < width; ++column) { // find another zero in the same row, in an unused column
			if (!usedColumns[column] && !rowPtr[column]) {
				conflict->x = column;
				usedColumns[column] = true;
				return true;
			}
		}

		// OR

		for (column = 0; column < width; ++column) { // find another zero in the same row, in a used column
			if (usedColumns[column] && !rowPtr[column]) {
				conflict->x = column;

				if (valueSwap(values, visitedRows, usedColumns, column, conflict->y))
					return true;

				conflict->x = x;
			}
		}

		return false;
	}


	template<typename T>
	void drawLines(V<T> & values) const {
		/*
		This function uses the method detailed below. You can also watch a
		lecture of it here: https://www.youtube.com/watch?v=BUGIhEecipE&t=895
		*/

		/*
		The original steps for this method work well for pencil and paper
		where it may be difficult/annoying to have to erase a mark/line.
		The modified steps I use here do the same as the original steps but
		faster, because I can easily undraw lines.

		Original Steps:
		1) Mark all unassigned rows.
		2) If a marked row has a zero, mark that column.
		3) If that column has an assignment, mark that row.
		4) Repeat steps 2 and 3 until no more marks can be made.
		5) Draw lines across unmarked rows and marked columns.

		Modified Steps:
		1) Draw a line across every row with an assignment.
		2) If there is a zero in an uncovered row
		-a) draw a line across its column.
		-b) If there is an assigned zero in that column, uncross its row.
		3) Repeat step 2 if a new line is drawn/undrawn.
		*/

		V<char> coveredRows(height, false), coveredColumns(width, false);
		bool newLine;

		for (const auto & result : results) // Modified Step 1
			coveredRows[result.y] = true;

		do {
			newLine = false;

			for (size_t row = 0; row < height; ++row) { // Modified Step 2
				if (!coveredRows[row]) {
					const T * rowPtr = &values[row*width];

					for (size_t column = 0; column < width; ++column) {
						if (!coveredColumns[column] && !rowPtr[column]) {
							coveredColumns[column] = true; // a
							newLine = true;

							for (const auto & result : results) {
								if (result.x == column) { // b
									coveredRows[result.y] = false;
									break;
								}
							}

							break;
						}
					}
				}
			}
		} while (newLine); // Modified Step 3

		updateMatrix(values, coveredRows, coveredColumns);
	}


	template<typename T>
	void updateMatrix(V<T> & values, const V<char> & coveredRows, const V<char> & coveredColumns) const {
		size_t row, column;
		T * rowPtr, min = std::numeric_limits<T>::max();

		for (row = 0; row < height; ++row) { // get smallest uncovered value
			if (!coveredRows[row]) {
				rowPtr = &values[row*width];

				for (column = 0; column < width; ++column)
					if (!coveredColumns[column] && rowPtr[column] < min)
						min = rowPtr[column];
			}
		}


		for (row = 0; row < height; ++row) {
			rowPtr = &values[row*width];

			if (coveredRows[row]) {
				for (column = 0; column < width; ++column) {
					if (coveredColumns[column]) { // add min to each doubly covered cell
						T & val = rowPtr[column];
						val += min;
						if (val < min) // prevent overflow by setting a cap
							val = std::numeric_limits<T>::max();
					}
				}
			} else {
				for (column = 0; column < width; ++column) // subtract min from each uncovered cell
					if (!coveredColumns[column]) rowPtr[column] -= min;
			}
		}
	}
};


#undef V


#endif /* APS */
