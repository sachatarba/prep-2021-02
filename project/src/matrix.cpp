#include "matrix.h"
#include "exceptions.h"

namespace prep {
class Matrix {
    private:
        size_t rows;
        size_t cols;
        std::vector<double> body;
    
    public:
        Matrix(size_t rows = 0, size_t cols = 0) {
            body.reserve(rows * cols);
        }        
        Matrix(std::istream& is) {
            is >> rows;
            is >> cols;
            body.reserve(rows * cols);
            for (size_t current_row = 0; current_row < rows; ++current_row) {
                for (size_t current_col = 0; current_col < cols; ++current_col) {
                    is >> body[rows * current_row + current_col];
                }
            }
        };
        Matrix(const Matrix& rhs) = default;
        Matrix& operator=(const Matrix& rhs) = default;
        ~Matrix() = default;

        size_t getRows() const {
            return rows;
        };
        size_t getCols() const {
            return cols;
        };

        double operator()(size_t i, size_t j) const {
            if (i > rows || j > cols) {
                throw OutOfRange(i, j, *this);
            }
            size_t pos = rows * i + j;
            return body[pos];
        };
        double& operator()(size_t i, size_t j) {
            if (i > rows || j > cols) {
                throw OutOfRange(i, j, *this);
            }
            size_t pos = rows * i + j;
            double& elem = body[pos];
            return elem;
        };

        bool operator==(const Matrix& rhs) const {
            if (rows != rhs.rows || cols != rhs.cols) {
                return false;
            }
            bool Flag = true;

            for (size_t current_elem = 0; current_elem < rows * cols; ++current_elem) {
                if (abs(body[current_elem] - rhs.body[current_elem]) >= 1e-7) {
                    Flag = false;
                    break;
                }
            }
            return Flag;
        };
        bool operator!=(const Matrix& rhs) const {
            return !((*this) == rhs);
        };

        Matrix operator+(const Matrix& rhs) const {
            if (rows != rhs.rows || cols != rhs.cols) {
                throw DimensionMismatch(*this, rhs);
            }
            Matrix new_matrix(rows, cols);
            for (size_t current_element = 0; current_element < rows * cols; ++current_element) {
                new_matrix.body[current_element] = body[current_element] + rhs.body[current_element];
            }
            return new_matrix;
        };
        Matrix operator-(const Matrix& rhs) const {
            if (rows != rhs.rows || cols != rhs.cols) {
                throw DimensionMismatch(*this, rhs);
            }
            Matrix new_matrix(rows, cols);
            for (size_t current_element = 0; current_element < rows * cols; ++current_element) {
                new_matrix.body[current_element] = body[current_element] - rhs.body[current_element];
            }
            return new_matrix;
        };
        Matrix operator*(const Matrix& rhs) const {
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
        };

        Matrix operator*(double val) const {
            Matrix new_matrix(rows, cols);
            for (size_t current_element = 0; current_element < rows * cols; ++current_element) {
                new_matrix.body[current_element] = body[current_element] * val;
            }
            return new_matrix;
        };

        friend
        Matrix operator*(double val, const Matrix& matrix);
        friend
        std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

        Matrix transp() const {
            Matrix new_matrix(rows, cols);
            for (size_t current_row = 0; current_row < rows; ++current_row) {
                for (size_t current_col = current_row + 1; current_col < cols; ++current_col) {
                    new_matrix(current_row, current_col) = (*this)(current_row, current_col);
                }
            }
        };

        double det() const {
            int size = rows;

            if (size == 1) {
                return body[0];
            }

            if (size == 2) {
            return body[0] * body[3] - body[1] * body[2];
            }

            double ret = 0;
            double temp_ret = 1;
            int sign = 1;

            for (int current_col = 0; current_col < size; ++current_col) {
                Matrix temp_matrix_minor(size - 1, size - 1);

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
        };

        Matrix adj() const;
        Matrix inv() const;
};

Matrix operator*(double val, const Matrix& matrix) {
    Matrix new_matrix(matrix.rows, matrix.cols);
    for (size_t current_elem = 0; current_elem < matrix.rows * matrix.cols; ++current_elem) {
        new_matrix.body[current_elem] *= val;
    }
    return new_matrix;
};
std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (size_t current_row = 0; current_row < matrix.rows; ++current_row) {
        for (size_t current_col = 0; current_col < matrix.cols; ++current_col) {
            os << matrix.body[matrix.rows * current_row + current_col];
            if (current_col != matrix.cols - 1) {
                os << " ";
            } else {
                os << std::endl;
            }
        }
    }
    return os;
};
    
}
