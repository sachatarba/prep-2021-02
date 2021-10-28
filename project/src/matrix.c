#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

/*
Формула перехода от номера строки и столбца элемента матрицы к индексу,
который он занимает в одномерном массиве:
index=row * (matrix->columns - 1) + col -1
*/

// Базовые операции
Matrix* create_matrix_from_file(const char* path_file) {
    FILE* matrix_input;
    matrix_input = fopen(path_file, "r");
    if (matrix_input == NULL) {
        return NULL;
    } else {
        size_t cols;
        size_t rows;
        fscanf(matrix_input, "%zu", &cols);
        fscanf(matrix_input, "%zu", &rows);
        Matrix* ret = create_matrix(rows, cols);
        for (size_t current_row = 0; current_row < rows; ++current_row) {
            for (size_t current_col = 0; current_col < cols; ++current_col) {
                fscanf(matrix_input, "%lf", &ret->body[current_row*cols+current_col]);
            }
        }
    return ret;
    }
}

Matrix* create_matrix(size_t rows, size_t cols) {
    Matrix* ret = (Matrix*)malloc(sizeof(Matrix));
    ret->rows = rows;
    ret->columns = cols;
    ret->body = (double*)calloc(ret->rows * ret->columns, sizeof(double));
    return ret;
}

void free_matrix(Matrix* matrix) {
    if (matrix == NULL) {
        return;
    }
    free(matrix->body);
    free(matrix);
}

int get_rows(const Matrix* matrix, size_t* rows) {
    if (matrix == NULL || matrix->body == NULL) {
        return 1;
    }
    *rows = matrix->rows;
    return 0;
}

int get_cols(const Matrix* matrix, size_t* cols) {
    if (matrix == NULL || matrix->body == NULL) {
        return 1;
    }
    *cols = matrix->columns;
    return 0;
}

int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
    if (matrix == NULL || matrix->body == NULL) {
        return 1;
    }
    if (row >= matrix->rows) {
        return 1;
    }
    if (col >= matrix->columns) {
        return 1;
    }
    *val = matrix->body[row * min(matrix->rows, matrix->columns) + col];
    return 0;
}

int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
    if (matrix == NULL || matrix->body == NULL) {
        return 1;
    }
    if (row >= matrix->rows) {
        return 1;
    }
    if (col >= matrix->columns) {
        return 1;
    }
    matrix->body[row * min(matrix->rows, matrix->columns) + col] = val;
    return 0;
}
// Математические операции
Matrix* mul_scalar(const Matrix* matrix, double val) {
    if (matrix == NULL) {
        return NULL;
    } else {
        Matrix* ret = create_matrix(matrix->rows, matrix->columns);
        for (size_t i = 0; i < matrix->rows * matrix->columns; ++i) {
            ret->body[i] = matrix->body[i] * val;
        }
        return ret;
    }
}

Matrix* transp(const Matrix* matrix) {
    if (matrix == NULL || matrix->rows != matrix->columns) {
        return NULL;
    } else {
        Matrix* ret = create_matrix(matrix->rows, matrix->columns);
        for (size_t current_row = 0; current_row < matrix->rows; ++current_row) {
            for (size_t current_col = 0; current_col < matrix->columns; ++current_col) {
                // Т.к. элемент матрицы при ее транспонировании поменяет местами свой номер строки и столбца,
                // то вычислим с учeтом этого индекс элемента, который он займет после транспонирования
                // в одномерном массиве.
                unsigned int index_after_transp = current_col * matrix->columns + current_row;
                ret->body[index_after_transp] = matrix->body[current_row * matrix->rows + current_col];
            }
        }
        return ret;
    }
}

Matrix* sum(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL || l->columns != r->columns || l->rows != r->columns) {
        return NULL;
    } else {
        Matrix* ret = create_matrix(l->rows, l->columns);
        for (size_t i = 0; i < l->columns * l->columns; ++i) {
            ret->body[i] = l->body[i] + r->body[i];
        }
        return ret;
    }
}

Matrix* sub(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL || l->columns != r->columns || l->rows != r->columns) {
        return NULL;
    } else {
        Matrix* ret = create_matrix(l->rows, l->columns);
        for (size_t i = 0; i < l->columns * l->columns; ++i) {
            ret->body[i] = l->body[i] - r->body[i];
        }
        return ret;
    }
}

// Дополнительные операции
int det(const Matrix* matrix, double* val) {
    if (matrix->columns != matrix->rows || matrix->rows < 1) {
        return 1;
    }
    int size = matrix->rows;
    if (size == 1) {
        *val = matrix->body[0];
        return 0;
    }
    if (size == 2) {
        *val = matrix->body[0] * matrix->body[3] - matrix->body[1] * matrix->body[2];
        return 0;
    }
    double ret = 0;
    double temp_ret = 1;
    int sign = 1;
    for (int i = 0; i < size; ++i) {
        Matrix* temp_matrix_minor;
        temp_matrix_minor = create_matrix(size - 1, size - 1);
        for (int j = 0, k = 0; j < size * size - size; ++j) {
            if ((j - i) % size) {
                temp_matrix_minor->body[k] = matrix->body[size + j];
                ++k;
            }
        }
        det(temp_matrix_minor, &temp_ret);
        ret += sign * matrix->body[i] * temp_ret;
        sign *= -1;
        free_matrix(temp_matrix_minor);
    }
    *val = ret;
    return 0;
}
