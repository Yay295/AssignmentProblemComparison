#include <ctime>
#include <random>
#include <algorithm>
#include <vector>
#include <iostream>
#include "munkres.h"

typedef size_t T;

const size_t THOUSAND = 1000;
const size_t MILLION = THOUSAND * THOUSAND;
const size_t BILLION = MILLION * THOUSAND;

// Tests specific matrices.
void specificTest() {
	const std::vector<std::vector<std::vector<T>>> tests = {
		{
			{0, 0},
			{0, 0}
		},{
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 1}
		},{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0},
			{0, 0, 1, 1, 1},
			{0, 0, 1, 1, 1},
			{0, 0, 1, 1, 1}
		}
	};

	for (const auto & test : tests) {
		Matrix<T> temp(test);
		Munkres<T> m;
		m.solve(temp);

		for (size_t row = 0; row < temp.rows(); ++row) {
			for (size_t col = 0; col < temp.columns(); ++col) {
				std::cout.width(2);
				std::cout << temp(row, col) << ',';
			}
			std::cout << '\n';
		}
	}
}

// Calculates the result of `todo` `width` x `height` matrices and averages
// their execution time.
void speedTest(const size_t todo, const size_t width, const size_t height) {
	std::cout << "Speed Test (" << todo << ' ' << width << 'x' << height << "):\n";

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<size_t> random(0, T(width * height));
	clock_t totalTime = 0;

	Matrix<T> mtx(height,width);
	for (size_t total = 1; total <= todo; ++total) {
		for (size_t row = 0; row < mtx.rows(); ++row)
			for (size_t column = 0; column < mtx.columns(); ++column)
				mtx(column,row) = T(random(mt));

		if (total < 12) std::cout << '\n' << total;
		else if (total < 100 && total % 12 == 0) std::cout << '\n' << total / 12 << " Dozen";
		else if (total < THOUSAND && total % 100 == 0) std::cout << '\n' << total / 100 << " Hundred";
		else if (total < MILLION && total % THOUSAND == 0) std::cout << '\n' << total / THOUSAND << " Thousand";
		else if (total < BILLION && total % MILLION == 0) std::cout << '\n' << total / MILLION << " Million";
		else if (total % BILLION == 0) std::cout << '\n' << total / BILLION << " Billion";

		Munkres<T> m;
		clock_t start = clock();
		m.solve(mtx);
		clock_t end = clock();
		totalTime += end - start;
	}

	std::cout << "\n\n" << (totalTime / double(todo)) / CLOCKS_PER_SEC << "s Average Time\n\n";
}

int main() {
	specificTest();
	speedTest(10000, 50, 50);
	speedTest(100, 250, 250);
	speedTest(10, 1000, 1000);
}
