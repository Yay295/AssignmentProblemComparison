#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <random>
#include <algorithm>
#include "APS.h"

// The int type to use to hold the matrix cost data.
typedef size_t D_TYPE;

const size_t THOUSAND = 1000;
const size_t MILLION = THOUSAND * THOUSAND;
const size_t BILLION = MILLION * THOUSAND;

std::vector<D_TYPE> values;

// Tests specific matrices.
void specificTest() {
	std::cout << "== Specific Tests ==\n\n";

	std::vector<std::vector<unsigned char>> matrices = {
		{0, 0,
		 0, 0},

		{0, 0, 0, 0,
		 0, 0, 0, 0,
		 0, 0, 0, 0,
		 0, 0, 0, 1},

		{0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0,
		 0, 0, 1, 1, 1,
		 0, 0, 1, 1, 1,
		 0, 0, 1, 1, 1}
	};

	for (auto & matrix : matrices) {
		APSO X(matrix.data(), size_t(sqrt(matrix.size())), size_t(sqrt(matrix.size())));
		std::cout << "Results:\n";
		X.printResults(std::cout);
		std::cout << "Result Cost: " << size_t(X.resultCost(matrix.data())) << "\n\n";
	}
}

// Calculates the result of `todo` `width` x `height` matrices and averages
// their execution time.
void speedTest(const size_t todo, const size_t width, const size_t height) {
	std::cout << "== Speed Test (" << todo << ' ' << width << 'x' << height << ") ==\n";

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<size_t> random(0, D_TYPE(width * height));
	clock_t totalTime = 0;

	values.resize(width * height);

	for (size_t total = 1; total <= todo; ++total) {
		for (size_t i = 0; i < values.size(); ++i)
			values[i] = D_TYPE(random(mt));

		if (total < 12) std::cout << '\n' << total;
		else if (total < 100 && total % 12 == 0) std::cout << '\n' << total / 12 << " Dozen";
		else if (total < THOUSAND && total % 100 == 0) std::cout << '\n' << total / 100 << " Hundred";
		else if (total < MILLION && total % THOUSAND == 0) std::cout << '\n' << total / THOUSAND << " Thousand";
		else if (total < BILLION && total % MILLION == 0) std::cout << '\n' << total / MILLION << " Million";
		else if (total % BILLION == 0) std::cout << '\n' << total / BILLION << " Billion";

		clock_t start = clock();
		APSO X(values, width, height);
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
