#!/usr/bin/env python3

from munkres import Munkres, print_matrix

matrices = [
		[[0, 0],
		 [0, 0]],

		[[0, 0, 0, 0],
		 [0, 0, 0, 0],
		 [0, 0, 0, 0],
		 [0, 0, 0, 1]],

		[[0, 0, 0, 0, 0],
		 [0, 0, 0, 0, 0],
		 [0, 0, 1, 1, 1],
		 [0, 0, 1, 1, 1],
		 [0, 0, 1, 1, 1]]]

for matrix in matrices:
	print_matrix(matrix, msg='Lowest cost through this matrix:')
	total = 0
	for row, column in Munkres().compute(matrix):
		value = matrix[row][column]
		total += value
		print('(%d, %d) -> %d' % (row, column, value))
	print('total cost: %d\n' % total)
