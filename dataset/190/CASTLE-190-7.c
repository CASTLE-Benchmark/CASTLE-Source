/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-190-7.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-190-7.c -o CASTLE-190-7
vulnerable: false
description: There are no arithmetic overflows in any of these functions.
cwe: 190
============================================================================
*/

#include <stdio.h>


void printMatrix(int rows, int cols, int mat[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

void addMatrices(int rows, int cols, int mat1[rows][cols], int mat2[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
}

void scalarMult(int rows, int cols, int mat[rows][cols], int scalar) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat[i][j] *= scalar;
        }
    }
}

void transpose(int rows, int cols, int mat[rows][cols], int result[cols][rows]) {
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            result[i][j] = mat[j][i];
        }
    }
}

int main() {
    int mat1[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int mat2[3][3] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    int sumMat[3][3] = {0};
    int scalar = 2;
    int transMat[3][3] = {0};

    printf("Matrix 1:\n");
    printMatrix(3, 3, mat1);

    printf("\nMatrix 2:\n");
    printMatrix(3, 3, mat2);

    printf("\nSum of matrices:\n");
    addMatrices(3, 3, mat1, mat2, sumMat);
    printMatrix(3, 3, sumMat);

    printf("\nScalar multiplication of matrix 1:\n");
    scalarMult(3, 3, mat1, scalar);
    printMatrix(3, 3, mat1);

    printf("\nTranspose of matrix 2:\n");
    transpose(3, 3, mat2, transMat);
    printMatrix(3, 3, transMat);

    return 0;
}