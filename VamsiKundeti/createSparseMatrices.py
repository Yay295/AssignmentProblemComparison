#!/usr/bin/env python3

matrices = [
	[
		[0, 0],
		[0, 0]
	], [
		[0, 0, 0, 0],
		[0, 0, 0, 0],
		[0, 0, 0, 0],
		[0, 0, 0, 1]
	], [
		[0, 0, 0, 0, 0],
		[0, 0, 0, 0, 0],
		[0, 0, 1, 1, 1],
		[0, 0, 1, 1, 1],
		[0, 0, 1, 1, 1]
	]
]

for i, matrix in enumerate(matrices):
	with open('test' + str(i+1) + '.txt','w', newline='\n') as file:
		# width, height, count
		print(len(matrix[0]), len(matrix), len(matrix[0]) * len(matrix), file=file)
		for y, row in enumerate(matrix):
			for x, value in enumerate(row):
				# x, y, value
				print(x + 1, y + 1, value, file=file)
