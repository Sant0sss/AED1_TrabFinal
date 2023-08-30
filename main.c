#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Matrix.h"

int main(void) {
    /* Inicializacao da aplicacao ... */
    Matrix *A = matrix_create(10, 10);
    matrix_print(A);
    Matrix *B = matrix_create(10, 10);
    matrix_print(B);
    Matrix *C = matrix_add(A, B);
    matrix_print(C);
    matrix_destroy(C);
    C = matrix_multiply(A, B);
    matrix_print(C);
    matrix_destroy(C);
    matrix_print(C);
    matrix_destroy(C);
    matrix_destroy(A);
    matrix_destroy(B);
    return 0;
}

