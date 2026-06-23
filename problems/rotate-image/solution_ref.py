from typing import List


def rotate(matrix: List[List[int]]) -> None:
    n = len(matrix)
    # Transpose, then reverse each row.
    for i in range(n):
        for j in range(i + 1, n):
            matrix[i][j], matrix[j][i] = matrix[j][i], matrix[i][j]
    for row in matrix:
        row.reverse()
