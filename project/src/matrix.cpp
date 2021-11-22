#include "matrix.h"
#include "exceptions.h"
#include <iomanip>
#include <limits>
namespace prep {
Matrix::Matrix(size_t, size_t) {
        body.reserve(rows * cols + 1);
    }
Matrix::Matrix(std::istream& is) {
    if (is.eof() || is.bad()) {
        throw InvalidMatrixStream();
    }
    if (is >> rows >> cols) {
        body.reserve(rows  * cols + 1);
        for (size_t current_row = 0; current_row < rows; ++current_row) {
            for (size_t current_col = 0; current_col < cols; ++current_col) {
                if (!(is >> body[rows * current_row + current_col])) {
                    throw InvalidMatrixStream();
                }
            }
        }
    } else {
        throw InvalidMatrixStream();
    }
}
size_t Matrix::getRows() const {
    return rows;
    }
size_t Matrix::getCols() const {
    return cols;
}

double Matrix::operator()(size_t i, size_t j) const {
        if (i >= rows || j >= cols) {
            throw OutOfRange(i, j, *this);
        }
        size_t pos = rows * i + j;
        return body[pos];
}
double& Matrix::operator()(size_t i, size_t j) {
    if (i >= rows || j >= cols) {
        throw OutOfRange(i, j, *this);
    }
    size_t pos = rows * i + j;
    double& elem = body[pos];
    return elem;
}

bool Matrix::operator==(const Matrix& rhs) const {
        if (rows != rhs.rows || cols != rhs.cols) {
            return false;
        }
        bool is_equal = true;

        for (size_t current_elem = 0; current_elem < rows * cols; ++current_elem) {
            if (std::abs(body[current_elem] - rhs.body[current_elem]) > 1e-7) {
                is_equal = false;
                break;
            }
        }
        return is_equal;
}
bool Matrix::operator!=(const Matrix& rhs) const {
        return !((*this) == rhs);
}

Matrix Matrix::operator+(const Matrix& rhs) const {
        if (rows != rhs.rows || cols != rhs.cols) {
            throw DimensionMismatch(*this, rhs);
        }
        Matrix new_matrix(rows, cols);
        for (size_t current_element = 0; current_element < rows * cols; ++current_element) {
            new_matrix.body[current_element] = body[current_element] + rhs.body[current_element];
        }
        return new_matrix;
}
Matrix Matrix::operator-(const Matrix& rhs) const {
    if (rows != rhs.rows || cols != rhs.cols) {
        throw DimensionMismatch(*this, rhs);
        }
    Matrix new_matrix(rows, cols);
    for (size_t current_element = 0; current_element < rows * cols; ++current_element) {
        new_matrix.body[current_element] = body[current_element] - rhs.body[current_element];
        }
        return new_matrix;
}
Matrix Matrix::operator*(const Matrix& rhs) const {
    if (cols != rhs.rows) {
        throw DimensionMismatch(*this, rhs);
    }
    Matrix new_matrix(rows, cols);
    for (size_t current_row = 0; current_row < rows; ++current_row) {
        for (size_t current_col = 0; current_col < rhs.cols; ++current_col) {
            double element_of_ret = 0;
            for (size_t current_pos = 0; current_pos < rhs.rows; ++current_pos) {
                element_of_ret += body[current_row * cols + current_pos] * \
                rhs.body[current_pos * rhs.cols + current_col];
            }
            new_matrix.body[current_row * rhs.cols + current_col] = element_of_ret;
        }
    }

    return new_matrix;
}

Matrix Matrix::operator*(double val) const {
        Matrix new_matrix(rows, cols);
        for (size_t current_element = 0; current_element < rows * cols; ++current_element) {
            new_matrix.body[current_element] = body[current_element] * val;
        }
        return new_matrix;
}
Matrix Matrix::transp() const {
    Matrix new_matrix(rows, cols);
    for (size_t current_row = 0; current_row < rows; ++current_row) {
        for (size_t current_col = current_row + 1; current_col < cols; ++current_col) {
            new_matrix(current_row, current_col) = (*this)(current_row, current_col);
        }
    }
    return new_matrix;
}

double Matrix::det() const {
    int size = rows;

    if (size == 1) {
        return body[0];
    }

    if (size == 2) {
        return body[0] * body[3] - body[1] * body[2];
    }

    double ret = 0;
    int sign = 1;

    for (int current_col = 0; current_col < size; ++current_col) {
        Matrix temp_matrix_minor(size - 1, size - 1);
        double temp_ret = 1;
        for (int current_pos = 0, counter = 0; current_pos < size * size - size; ++current_pos) {
            if ((current_pos - current_col) % size) {
                temp_matrix_minor.body[counter] = body[size + current_pos];
                    ++counter;
            }
        }
        temp_ret = temp_matrix_minor.det();
        ret += sign * body[current_col] * temp_ret;
        sign *= -1;
    }

    return ret;
}

Matrix Matrix::adj() const {
    Matrix adj_matrix(rows, rows);
    size_t size = rows;

    for (size_t current_row = 0; current_row < size; ++current_row) {
        for (size_t current_col = 0; current_col < size; ++current_col) {
            Matrix temp_matrix_minor(size - 1, size - 1);
            for (size_t current_pos = 0, counter = 0; current_pos < size * size; ++current_pos) {
                if (current_pos % size != current_col && current_pos / size != current_row) {
                    double element_of_minor = 1;
                    element_of_minor = (*this)(current_pos / size, current_pos % size);
                    temp_matrix_minor.body[counter] = element_of_minor;
                    ++counter;
                }
            }
            double alg_compl = 0;
            alg_compl = temp_matrix_minor.det();

            if ((current_row + current_col) % 2 != 0) {
                alg_compl *= -1;
            }
            adj_matrix(current_col, current_row) = alg_compl;
        }
    }
    return adj_matrix;
}
    Matrix Matrix::inv() const {
        Matrix inv_matrix(rows, cols);
        inv_matrix = (*this).adj();
        double det = (*this).det();
        for (size_t current_elem = 0; current_elem < rows * cols; ++current_elem) {
            inv_matrix.body[current_elem] /= det;
        }
        return inv_matrix;
}

Matrix operator*(double val, const Matrix& matrix) {
    Matrix new_matrix(matrix.rows, matrix.cols);
    for (size_t current_elem = 0; current_elem < matrix.rows * matrix.cols; ++current_elem) {
        new_matrix.body[current_elem] *= val;
    }
    return new_matrix;
}
std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    os << matrix.rows << " " << matrix.cols << std::endl;
    for (size_t current_row = 0; current_row < matrix.rows; ++current_row) {
        for (size_t current_col = 0; current_col < matrix.cols; ++current_col) {
            os << matrix.body[matrix.rows * current_row + current_col];
            if (current_col != matrix.cols - 1) {
                os << std::setprecision(std::numeric_limits<double>::max_digits10) \
                 << matrix.body[current_row * matrix.rows + current_col] << " ";
            } else {
                os << std::setprecision(std::numeric_limits<double>::max_digits10) \
                 << matrix.body[current_row * matrix.rows + current_col] << std::endl;
            }
        }
    }
    return os;
}
}  // namespace prep
