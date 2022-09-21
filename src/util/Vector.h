#ifndef OPENGL_RENDERER_VECTOR_H
#define OPENGL_RENDERER_VECTOR_H


template<typename T, unsigned int S>
struct Vector;

template<typename T>
struct Vector<T, 2> {
    constexpr Vector() noexcept : x(0), y(0) {};
    constexpr Vector(T x, T y) noexcept : x(x), y(y) {};
    T x;
    T y;

    constexpr T& operator[](uint32_t index) {
        switch(index) {
            case 0: return x;
            case 1: return y;
            default: throw std::out_of_range("Vector index out of range.");
        }
    };

    constexpr const T& operator[](uint32_t index) const {
        switch (index) {
            case 0: return x;
            case 1: return y;
            default: throw std::out_of_range("Vector index out of range.");
        }
    };

    constexpr Vector<T, 2> operator+(const Vector<T, 2>& other) const {
        return Vector<T, 2>(x + other.x, y + other.y);
    };

    constexpr Vector<T, 2> operator-(const Vector<T, 2>& other) const {
        return Vector<T, 2>(x - other.x, y - other.y);
    };

    constexpr Vector<T, 2> operator*(T scalar) const {
        return Vector<T, 2>(x * scalar, y * scalar);
    };

    /**
     * Calculates the vector dot product of the vector with another vector.
     *
     * @param other the other vector
     * @returns the dot product of the two vectors
     */
    constexpr T dot(const Vector<T, 2>& other) const {
        return x * other.x + y * other.y;
    }

    /**
     * @returns the number of components in the vector
     */
    constexpr unsigned int components() const {
        return 2;
    };
};

template<typename T>
struct Vector<T, 3> {
    constexpr Vector() noexcept : x(0), y(0), z(0) {};
    constexpr Vector(T x, T y, T z) noexcept : x(x), y(y), z(z) {};
    T x;
    T y;
    T z;

    constexpr T& operator[](uint32_t index) {
        switch(index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Vector index out of range.");
        }
    };

    constexpr const T& operator[](uint32_t index) const {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Vector index out of range.");
        }
    };

    constexpr Vector<T, 3> operator+(const Vector<T, 3>& other) const {
        return Vector<T, 3>(x + other.x, y + other.y, z + other.z);
    };

    constexpr Vector<T, 3> operator-(const Vector<T, 3>& other) const {
        return Vector<T, 3>(x - other.x, y - other.y, z - other.z);
    };

    constexpr Vector<T, 3> operator*(T scalar) const {
        return Vector<T, 3>(x * scalar, y * scalar, z * scalar);
    };

    /**
     * Calculates the vector dot product of the vector with another vector.
     *
     * @param other the other vector
     * @returns the dot product of the two vectors
     */
    constexpr T dot(const Vector<T, 3>& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    /**
     * Calculates the vector cross product of the vector with another vector.
     *
     * @param other the other vector
     * @returns the cross product of the two vectors
     */
    constexpr Vector<T, 3> cross(const Vector<T, 3>& other) const {
        return {y * other.z - z * other.y, -(x * other.z - z * other.x), x * other.y - y * other.x};
    }

    /**
     * @returns the number of components in the vector
     */
    constexpr unsigned int components() const {
        return 3;
    };
};

template<typename T>
struct Vector<T, 4> {
    constexpr Vector() noexcept : x(0), y(0), z(0), w(0) {};
    constexpr Vector(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) {};
    T x;
    T y;
    T z;
    T w;

    constexpr T& operator[](uint32_t index) {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: throw std::out_of_range("Vector index out of range.");
        }
    };

    constexpr const T& operator[](uint32_t index) const {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: throw std::out_of_range("Vector index out of range.");
        }
    };

    constexpr Vector<T, 4> operator+(const Vector<T, 4>& other) const {
        return Vector<T, 4>(x + other.x, y + other.y, z + other.z, w + other.w);
    };

    constexpr Vector<T, 4> operator-(const Vector<T, 4>& other) const {
        return Vector<T, 4>(x - other.x, y - other.y, z - other.z, w - other.w);
    };

    constexpr Vector<T, 4> operator*(T scalar) const {
        return Vector<T, 4>(x * scalar, y * scalar, z * scalar, w * scalar);
    };

    /**
     * Calculates the vector dot product of the vector with another vector.
     *
     * @param other the other vector
     * @returns the dot product of the two vectors
     */
    constexpr T dot(const Vector<T, 4>& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    /**
     * @returns the number of components in the vector
     */
    constexpr unsigned int components() const {
        return 4;
    };
};

using Vec2 = Vector<float, 2>;
using Vec3 = Vector<float, 3>;
using Vec4 = Vector<float, 4>;


#endif //OPENGL_RENDERER_VECTOR_H
