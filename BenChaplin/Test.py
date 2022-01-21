#!/usr/bin/env python3

from algorithm import find_matching

'''graphs = [
	{
		'x1': {'y1': 0, 'y2': 0},
		'x2': {'y1': 0, 'y2': 0}
	}, {
		'x1': {'y1': 0, 'y2': 0, 'y3': 0, 'y4': 0},
		'x2': {'y1': 0, 'y2': 0, 'y3': 0, 'y4': 0},
		'x3': {'y1': 0, 'y2': 0, 'y3': 0, 'y4': 0},
		'x4': {'y1': 0, 'y2': 0, 'y3': 0, 'y4': 1}
	}, {
		'x1': {'y1': 0, 'y2': 0, 'y3': 0, 'y4': 0, 'y5': 0},
		'x2': {'y1': 0, 'y2': 0, 'y3': 0, 'y4': 0, 'y5': 0},
		'x3': {'y1': 0, 'y2': 0, 'y3': 1, 'y4': 1, 'y5': 1},
		'x4': {'y1': 0, 'y2': 0, 'y3': 1, 'y4': 1, 'y5': 1},
		'x5': {'y1': 0, 'y2': 0, 'y3': 1, 'y4': 1, 'y5': 1}
	}
]'''

# There is a bug that makes it not work if any edge is 0, so here's test cases with everything incremented by 1.
# https://github.com/benchaplin/hungarian-algorithm/issues/4#issuecomment-668621678
graphs = [
	{
		'x1': {'y1': 1, 'y2': 1},
		'x2': {'y1': 1, 'y2': 1}
	}, {
		'x1': {'y1': 1, 'y2': 1, 'y3': 1, 'y4': 1},
		'x2': {'y1': 1, 'y2': 1, 'y3': 1, 'y4': 1},
		'x3': {'y1': 1, 'y2': 1, 'y3': 1, 'y4': 1},
		'x4': {'y1': 1, 'y2': 1, 'y3': 1, 'y4': 2}
	}, {
		'x1': {'y1': 1, 'y2': 1, 'y3': 1, 'y4': 1, 'y5': 1},
		'x2': {'y1': 1, 'y2': 1, 'y3': 1, 'y4': 1, 'y5': 1},
		'x3': {'y1': 1, 'y2': 1, 'y3': 2, 'y4': 2, 'y5': 2},
		'x4': {'y1': 1, 'y2': 1, 'y3': 2, 'y4': 2, 'y5': 2},
		'x5': {'y1': 1, 'y2': 1, 'y3': 2, 'y4': 2, 'y5': 2}
	}
]

for graph in graphs:
	result = find_matching(graph,matching_type='min')
	print(result)
