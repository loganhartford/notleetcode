# Time: O(m·n)  — two passes through the matrix
from typing import List


def setZeroes(matrix: List[List[int]]) -> None:
    rows = set()
    cols = set()
    for i, row in enumerate(matrix):
        for j, val in enumerate(row):
            if val == 0:
                rows.add(i)
                cols.add(j)
    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            if i in rows or j in cols:
                matrix[i][j] = 0
