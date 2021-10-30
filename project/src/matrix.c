#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

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
    }
    size_t cols;
    size_t rows;
    fscanf(matrix_input, "%zu", &rows);
    fscanf(matrix_input, "%zu", &cols);
    Matrix* ret = create_matrix(rows, cols);
    for (size_t current_row = 0; current_row < rows; ++current_row) {
        for (size_t current_col = 0; current_col < cols; ++current_col) {
            fscanf(matrix_input, "%lf", &ret->body[current_row * cols + current_col]);
        }
    }
    fclose(matrix_input);
    return ret;
}

Matrix* create_matrix(size_t rows, size_t cols) {
    Matrix* ret = (Matrix*) malloc(sizeof(Matrix));
    ret->rows = rows;
    ret->columns = cols;
    ret->body = (double*) calloc(rows * cols, sizeof(double));
    return ret;
}

void free_matrix(Matrix* matrix) {
    if (matrix == NULL) {
        return;
    }
    if (matrix->body != NULL) {
        free(matrix->body);
    }
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
    *val = matrix->body[row * (matrix->columns) + col];
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
    matrix->body[row * matrix->columns + col] = val;
    return 0;
}

// Математические операции
Matrix* mul_scalar(const Matrix* matrix, double val) {
    if (matrix == NULL) {
        return NULL;
    }
    Matrix* ret = create_matrix(matrix->rows, matrix->columns);
    for (size_t i = 0; i < matrix->rows * matrix->columns; ++i) {
        ret->body[i] = matrix->body[i] * val;
    }
    return ret;
}

Matrix* transp(const Matrix* matrix) {
    if (matrix == NULL) {
        return NULL;
    }
    Matrix* ret = create_matrix(matrix->columns, matrix->rows);
    for (size_t current_row = 0; current_row < matrix->rows; ++current_row) {
        for (size_t current_col = 0; current_col < matrix->columns; ++current_col) {
            double element = 0;
            get_elem(matrix, current_row, current_col, &element);
            set_elem(ret, current_col, current_row, element);
        }
    }
    return ret;
}

Matrix* sum(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL || l->columns != r->columns || l->rows != r->rows  || \
    l->columns == 0 || l->rows == 0) {
        return NULL;
    }
    Matrix* ret = create_matrix(r->rows, r->columns);
    for (size_t i = 0; i < l->columns * l->rows; ++i) {
        ret->body[i] = l->body[i] + r->body[i];
    }
    return ret;
}

Matrix* sub(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL || l->columns != r->columns || l->rows != r->rows) {
        return NULL;
    }
    Matrix* ret = create_matrix(l->rows, l->columns);
    for (size_t i = 0; i < l->columns * l->rows; ++i) {
        ret->body[i] = l->body[i] - r->body[i];
    }
    return ret;
}

Matrix* mul(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL || l->columns != r->rows) {
        return NULL;
    }
    Matrix* ret = create_matrix(l->rows, r->columns);
    for (size_t current_row = 0; current_row < l->rows; ++current_row) {
        for (size_t current_col = 0; current_col < r->columns; ++current_col) {
            double element_of_ret = 0;
            for (size_t current_pos = 0; current_pos < r->rows; ++current_pos) {
                element_of_ret += l->body[current_row * l->columns + current_pos] * \
                r->body[current_pos * r->columns + current_col];
            }
        ret->body[current_row * r->columns + current_col] = element_of_ret;
        }
    }
    return ret;
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
    for (int current_col = 0; current_col < size; ++current_col) {
        Matrix* temp_matrix_minor;
        temp_matrix_minor = create_matrix(size - 1, size - 1);
        for (int current_pos = 0, counter = 0; current_pos < size * size - size; ++current_pos) {
            if ((current_pos - current_col) % size) {
                temp_matrix_minor->body[counter] = matrix->body[size + current_pos];
                ++counter;
            }
        }
        det(temp_matrix_minor, &temp_ret);
        ret += sign * matrix->body[current_col] * temp_ret;
        sign *= -1;
        free_matrix(temp_matrix_minor);
    }
    *val = ret;
    return 0;
}

Matrix* adj(const Matrix* matrix) {
    if (matrix == NULL || matrix->body == NULL || matrix->rows != matrix->columns) {
        return NULL;
    }
    Matrix* ret = create_matrix(matrix->rows, matrix->rows);
    size_t size = matrix->rows;
    for (size_t current_row = 0; current_row < size; ++current_row) {
        for (size_t current_col = 0; current_col < size; ++current_col) {
            Matrix* temp_matrix_minor;
            temp_matrix_minor = create_matrix(size - 1, size - 1);
            for (size_t current_pos = 0, counter = 0; current_pos < size * size; ++current_pos) {
                if (current_pos % size != current_col && current_pos / size != current_row) {
                    double element_of_minor = 1;
                    get_elem(matrix, current_pos / size, current_pos % size, &element_of_minor);
                    temp_matrix_minor->body[counter] = element_of_minor;
                    ++counter;
                }
            }
            double alg_compl = 0;
            det(temp_matrix_minor, &alg_compl);
            if ((current_row + current_col) % 2 != 0) {
                alg_compl *= -1;
            }
            free_matrix(temp_matrix_minor);
            set_elem(ret, current_col, current_row, alg_compl);
        }
    }
    return ret;
}

Matrix* inv(const Matrix* matrix) {
    if (matrix == NULL || matrix->body == NULL || matrix->rows != matrix->columns || matrix->rows <= 1) {
        return NULL;
    }
    int size = matrix->rows;
    double det_matrix = 1;
    det(matrix, &det_matrix);
    if (det_matrix < 1e-6) {
        return NULL;
    }
    Matrix* ret = create_matrix(size + 1, size + 1);
    Matrix* adj_matrix = adj(matrix);
    for (int current_pos = 0; current_pos < size * size; ++current_pos) {
        ret->body[current_pos] = adj_matrix->body[current_pos] / det_matrix;
    }
    // ret->body[size * size - 1] = adj_matrix->body[size * size - 1] / det_matrix;
    // int current_pos = 0;
    // while (current_pos < size * size) {
    //    ret->body[current_pos] = adj_matrix->body[current_pos] / det_matrix;
    //    ++current_pos;
    //}
    // ret->rows = size;
    // ret->columns = size;
    free_matrix(adj_matrix);
    return ret;
}
