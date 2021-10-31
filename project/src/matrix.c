#include "matrix.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


// Базовые операции
Matrix* create_matrix_from_file(const char* path_file) {
    FILE* matrix_input;
    matrix_input = fopen(path_file, "r");

    if (matrix_input == NULL) {
        return NULL;
    }
    size_t cols = 0;
    size_t rows = 0;
    int arg_count = fscanf(matrix_input, "%zu", &rows);

    if (arg_count != 1) {
        fclose(matrix_input);
        return NULL;
    }
    arg_count = fscanf(matrix_input, "%zu", &cols);

    if (arg_count != 1) {
        fclose(matrix_input);
        return NULL;
    }
    bool error_flag = 0;
    Matrix* ret = create_matrix(rows, cols);

    for (size_t current_row = 0; current_row < rows; ++current_row) {
        for (size_t current_col = 0; current_col < cols; ++current_col) {
            arg_count = fscanf(matrix_input, "%lf", &ret->matrix[current_row * cols + current_col]);
            if (arg_count != 1) {
                error_flag = 1;
                break;
            }
        }
    }
    fclose(matrix_input);

    if (error_flag) {
        free_matrix(ret);
        return NULL;
    }

    return ret;
}

Matrix* create_matrix(size_t rows, size_t cols) {
    Matrix* ret = (Matrix*) malloc(sizeof(Matrix));
    if (ret == NULL) {
        free_matrix(ret);
        return NULL;
    }
    ret->matrix = (double*) calloc(rows * cols, sizeof(double));

    if (ret->matrix == NULL) {
        free_matrix(ret);
        return NULL;
    }

    ret->rows = rows;
    ret->columns = cols;
    return ret;
}

void free_matrix(Matrix* matrix) {
    if (matrix == NULL) {
        return;
    }

    if (matrix->matrix != NULL) {
        free(matrix->matrix);
    }

    free(matrix);
}

int get_rows(const Matrix* matrix, size_t* rows) {
    if (matrix == NULL || matrix->matrix == NULL || rows == NULL) {
        return 1;
    }

    *rows = matrix->rows;
    return 0;
}

int get_cols(const Matrix* matrix, size_t* cols) {
    if (matrix == NULL || matrix->matrix == NULL || cols == NULL) {
        return 1;
    }

    *cols = matrix->columns;
    return 0;
}

int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
    if (matrix == NULL || matrix->matrix == NULL || val == NULL) {
        return 1;
    }

    if (row >= matrix->rows || col >= matrix->columns) {
        return 1;
    }

    *val = matrix->matrix[row * (matrix->columns) + col];
    return 0;
}

int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
    if (matrix == NULL || matrix->matrix == NULL) {
        return 1;
    }

    if (row >= matrix->rows || col >= matrix->columns) {
        return 1;
    }

    matrix->matrix[row * matrix->columns + col] = val;
    return 0;
}

// Математические операции
Matrix* mul_scalar(const Matrix* matrix, double val) {
    if (matrix == NULL) {
        return NULL;
    }
    Matrix* ret = create_matrix(matrix->rows, matrix->columns);

    if (ret == NULL) {
        free_matrix(ret);
        return NULL;
    }

    for (size_t i = 0; i < matrix->rows * matrix->columns; ++i) {
        ret->matrix[i] = matrix->matrix[i] * val;
    }

    return ret;
}

Matrix* transp(const Matrix* matrix) {
    if (matrix == NULL) {
        return NULL;
    }
    Matrix* ret = create_matrix(matrix->columns, matrix->rows);

    if (ret == NULL) {
        free_matrix(ret);
        return NULL;
    }

    for (size_t current_row = 0; current_row < matrix->rows; ++current_row) {
        for (size_t current_col = 0; current_col < matrix->columns; ++current_col) {
            double element = 0;
            get_elem(matrix, current_row, current_col, &element);
            set_elem(ret, current_col, current_row, element);
        }
    }

    return ret;
}

Matrix* sum(const Matrix* left, const Matrix* right) {
    if (left == NULL || right == NULL || left->columns != right->columns || left->rows != right->rows  || \
    left->columns == 0 || left->rows == 0) {
        return NULL;
    }
    Matrix* ret = create_matrix(right->rows, right->columns);

    if (ret == NULL) {
        free_matrix(ret);
        return NULL;
    }

    for (size_t i = 0; i < left->columns * left->rows; ++i) {
        ret->matrix[i] = left->matrix[i] + right->matrix[i];
    }

    return ret;
}

Matrix* sub(const Matrix* left, const Matrix* right) {
    if (left == NULL || right == NULL || left->columns != right->columns || left->rows != right->rows) {
        return NULL;
    }
    Matrix* ret = create_matrix(left->rows, left->columns);

    if (ret == NULL) {
        free_matrix(ret);
        return NULL;
    }

    for (size_t i = 0; i < left->columns * left->rows; ++i) {
        ret->matrix[i] = left->matrix[i] - right->matrix[i];
    }

    return ret;
}

Matrix* mul(const Matrix* left, const Matrix* right) {
    if (left == NULL || right == NULL || left->columns != right->rows) {
        return NULL;
    }
    Matrix* ret = create_matrix(left->rows, right->columns);

    if (ret == NULL) {
        free_matrix(ret);
        return NULL;
    }

    for (size_t current_row = 0; current_row < left->rows; ++current_row) {
        for (size_t current_col = 0; current_col < right->columns; ++current_col) {
            double element_of_ret = 0;
            for (size_t current_pos = 0; current_pos < right->rows; ++current_pos) {
                element_of_ret += left->matrix[current_row * left->columns + current_pos] * \
                right->matrix[current_pos * right->columns + current_col];
            }
        ret->matrix[current_row * right->columns + current_col] = element_of_ret;
        }
    }

    return ret;
}

// Дополнительные операции
int det(const Matrix* matrix, double* val) {
    if (matrix == NULL || val == NULL || matrix->columns != matrix->rows || matrix->rows < 1) {
        return 1;
    }
    int size = matrix->rows;

    if (size == 1) {
        *val = matrix->matrix[0];
        return 0;
    }

    if (size == 2) {
        *val = matrix->matrix[0] * matrix->matrix[3] - matrix->matrix[1] * matrix->matrix[2];
        return 0;
    }
    double ret = 0;
    double temp_ret = 1;
    int sign = 1;

    for (int current_col = 0; current_col < size; ++current_col) {
        Matrix* temp_matrix_minor;
        temp_matrix_minor = create_matrix(size - 1, size - 1);

        if (temp_matrix_minor == NULL) {
            free_matrix(temp_matrix_minor);
            return 1;
        }

        for (int current_pos = 0, counter = 0; current_pos < size * size - size; ++current_pos) {
            if ((current_pos - current_col) % size) {
                temp_matrix_minor->matrix[counter] = matrix->matrix[size + current_pos];
                ++counter;
            }
        }
        det(temp_matrix_minor, &temp_ret);
        ret += sign * matrix->matrix[current_col] * temp_ret;
        sign *= -1;
        free_matrix(temp_matrix_minor);
    }

    *val = ret;
    return 0;
}

Matrix* adj(const Matrix* matrix) {
    if (matrix == NULL || matrix->matrix == NULL || matrix->rows != matrix->columns) {
        return NULL;
    }
    Matrix* ret = create_matrix(matrix->rows, matrix->rows);

    if (ret == NULL) {
        free_matrix(ret);
        return NULL;
    }
    size_t size = matrix->rows;

    for (size_t current_row = 0; current_row < size; ++current_row) {
        for (size_t current_col = 0; current_col < size; ++current_col) {
            Matrix* temp_matrix_minor;
            temp_matrix_minor = create_matrix(size - 1, size - 1);

            if (temp_matrix_minor == NULL) {
                free_matrix(ret);
                free_matrix(temp_matrix_minor);
                return NULL;
            }

            for (size_t current_pos = 0, counter = 0; current_pos < size * size; ++current_pos) {
                if (current_pos % size != current_col && current_pos / size != current_row) {
                    double element_of_minor = 1;
                    get_elem(matrix, current_pos / size, current_pos % size, &element_of_minor);
                    temp_matrix_minor->matrix[counter] = element_of_minor;
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
    if (matrix == NULL || matrix->matrix == NULL || matrix->rows != matrix->columns || matrix->rows < 1) {
        return NULL;
    }

    if (matrix->rows == 1) {
        Matrix* ret = create_matrix(1, 1);

        if (ret == NULL) {
            free_matrix(ret);
            return NULL;
        }
        ret->matrix[0] = matrix->matrix[0];
        ret->rows = 1;
        return ret;
    }
    int size = matrix->rows;
    double det_matrix = 1;
    det(matrix, &det_matrix);

    if (det_matrix < 1e-6) {
        return NULL;
    }
    Matrix* ret = create_matrix(size + 1, size + 1);

    if (ret == NULL) {
        free_matrix(ret);
        return NULL;
    }
    Matrix* adj_matrix = adj(matrix);

    if (adj_matrix == NULL) {
        free_matrix(ret);
        free_matrix(adj_matrix);
        return NULL;
    }

    for (int current_pos = 0; current_pos < size * size; ++current_pos) {
        ret->matrix[current_pos] = adj_matrix->matrix[current_pos] / det_matrix;
    }
    // ret->body[size * size - 1] = adj_matrix->body[size * size - 1] / det_matrix;
    // int current_pos = 0;
    // while (current_pos < size * size) {
    //    ret->body[current_pos] = adj_matrix->body[current_pos] / det_matrix;
    //    ++current_pos;
    // }
    // ret->rows = size;
    // ret->columns = size;
    free_matrix(adj_matrix);
    return ret;
}
