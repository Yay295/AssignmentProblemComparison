#include "dlib/optimization/max_cost_assignment.h"
#include <iostream>

int main() {
	std::vector<dlib::matrix<unsigned char>> tests = {dlib::matrix<unsigned char>(2,2), dlib::matrix<unsigned char>(4,4), dlib::matrix<unsigned char>(5,5)};

	tests[0] = 0, 0,
			   0, 0;

	tests[1] = 0, 0, 0, 0,
			   0, 0, 0, 0,
			   0, 0, 0, 0,
			   0, 0, 0, 1;

	tests[2] = 0, 0, 0, 0, 0,
			   0, 0, 0, 0, 0,
			   0, 0, 1, 1, 1,
			   0, 0, 1, 1, 1,
			   0, 0, 1, 1, 1;

	for (const auto & test : tests) {
		const auto assignment = dlib::max_cost_assignment(test);
		for (size_t i = 0; i < assignment.size(); ++i)
			std::cout << '(' << assignment[i] << ", " << i << ") ";
		std::cout << "\ncost: " << size_t(dlib::assignment_cost(test,assignment)) << '\n';
	}
}
