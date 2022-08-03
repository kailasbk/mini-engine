#ifndef OPENGL_RENDERER_MATRIX_H
#define OPENGL_RENDERER_MATRIX_H

#include "Vector.h"

template<typename T, unsigned int C, unsigned int R>
class Matrix {};

template<unsigned int C, unsigned int R>
class Matrix<float, C, R> {
public:
    static_assert(2 <= C && C <= 4, "Matrix must have between 2 and 4 columns.");
    static_assert(2 <= R && R <= 4, "Matrix must have between 2 and 4 rows.");

    constexpr Matrix() : m_cols() {};
    explicit constexpr Matrix(std::array<Vector<float, R>, C> cols) : m_cols(std::move(cols)) {}

    constexpr Vector<float, R>& operator[](uint32_t index) {
        if (index >= C) throw std::out_of_range("Matrix column index out of range.");
        return m_cols[index];
    }

    constexpr Vector<float, R> column(uint32_t index) const {
        if (index >= C) throw std::out_of_range("Matrix column index out of range.");
        return m_cols[index];
    }

    constexpr Vector<float, C> row(uint32_t index) const {
        if (index >= R) throw std::out_of_range("Matrix row index out of range.");
        Vector<float, C> vec;
        for (int i = 0; i < C; i++) {
            vec[i] = m_cols[i][index];
        }
        return vec;
    }

    constexpr Matrix<float, R, C> transpose() const {
        std::array<Vector<float, R>, C> cols;
        for (int i = 0; i < R; i++) {
            cols[i] = row(i);
        }
        return Matrix<float, R, C>(cols);
    }

    template<unsigned int CC>
    constexpr Matrix<float, CC, R> operator*(const Matrix<float, CC, C>& other) const {
        std::array<Vector<float, R>, CC> cols;
        for (int i = 0; i < CC; i++) {
            for (int j = 0; j < R; j++) {
                cols[i][j] = row(j).dot(other.column(i));
            }
        }
        return Matrix<float, CC, R>(cols);
    }

    /**
     * @returns the number of columns in the matrix
     */
    constexpr uint32_t numColumns() const {
        return C;
    };

    /**
     * @returns the number of rows in the matrix
     */
    constexpr uint32_t numRows() const {
        return R;
    };

private:
    std::array<Vector<float, R>, C> m_cols;
};

using Mat2 = Matrix<float, 2, 2>;
using Mat3 = Matrix<float, 3, 3>;
using Mat4 = Matrix<float, 4, 4>;

#endif //OPENGL_RENDERER_MATRIX_H
