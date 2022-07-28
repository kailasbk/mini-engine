#ifndef OPENGL_RENDERER_VECTOR_H
#define OPENGL_RENDERER_VECTOR_H


template<typename T, unsigned int S>
struct Vector;

template<typename T>
struct Vector<T, 2> {
    constexpr Vector(T x, T y) noexcept : x(x), y(y) {};
    T x;
    T y;

    /**
     * @returns the number of components in the vector
     */
    constexpr unsigned int components() const {
        return 2;
    };
};

template<typename T>
struct Vector<T, 3> {
    constexpr Vector(T x, T y, T z) noexcept : x(x), y(y), z(z) {};
    T x;
    T y;
    T z;

    /**
     * @returns the number of components in the vector
     */
    constexpr unsigned int components() const {
        return 3;
    };
};

template<typename T>
struct Vector<T, 4> {
    constexpr Vector(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) {};
    T x;
    T y;
    T z;
    T w;

    /**
     * @returns the number of components in the vector
     */
    constexpr unsigned int components() const {
        return 4;
    };
};


#endif //OPENGL_RENDERER_VECTOR_H
