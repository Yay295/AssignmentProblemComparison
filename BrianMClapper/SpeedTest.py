#!/usr/bin/env python3

from munkres import Munkres
import random, time

tests = (
	# todo, size
	(10000,50),
	(100,250),
	(10,1000)
)

THOUSAND = 1000
MILLION = THOUSAND * THOUSAND
BILLION = MILLION * THOUSAND

for todo, size in tests:
	max_size = size * size
	total_time = 0

	for total in range(1,todo+1):
		matrix = [[random.randint(0,max_size) for y in range(size)] for x in range(size)]

		if (total < 12): print(total)
		elif (total < 100 and total % 12 == 0): print(total / 12, " Dozen")
		elif (total < THOUSAND and total % 100 == 0): print(total / 100, " Hundred")
		elif (total < MILLION and total % THOUSAND == 0): print(total / THOUSAND, " Thousand")
		elif (total < BILLION and total % MILLION == 0): print(total / MILLION, " Million")
		elif (total % BILLION == 0): print(total / BILLION, " Billion")

		start = time.time()
		Munkres().compute(matrix)
		end = time.time()

		total_time += end - start

	print('\nAverage Time (', size, 'x', size, '):', total_time / todo, '\n')
