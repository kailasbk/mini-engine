#ifndef OPENGL_RENDERER_UNIFORM_H
#define OPENGL_RENDERER_UNIFORM_H

#include "../util/Vector.h"
#include "../util/Matrix.h"
class UniformVisitor;

class Uniform {
public:
    explicit Uniform(const void* data) : m_data(data) {}

    virtual ~Uniform() = default;

    /**
     * @returns the size consumed by the uniform, in bytes
     */
    virtual uint32_t size() const = 0;

    /**
     * @returns the alignment of the uniform, in bytes
     */
    virtual uint32_t alignment() const = 0;

    /**
     * Writes the uniform's data to the given pointer.
     */
    virtual void write(void* destination) const = 0;

    /**
     * Accepts uniform visitor to bind the uniform to the current context using double dispatch.
     *
     * @param visitor the uniform visitor
     */
    virtual void bind(UniformVisitor& visitor) const {
        throw std::runtime_error("Uniform has not implemented accepting the visitor.");
    };

    /**
     * @returns a pointer to the uniform's data, or nullptr if the uniform does not directly link to data
     */
    const void* data() const {
        return m_data;
    };

    /**
     * @returns n rounded up to the next multiple of k.
     */
    constexpr static uint32_t roundUp(uint32_t n, uint32_t k) {
        uint32_t remainder = n % k;
        if (remainder == 0) {
            return n;
        } else {
            return n - remainder + k;
        }
    }

protected:
    const void* m_data;
};

template<typename T>
class ScalarUniform : public Uniform {
public:
    static_assert(sizeof(T) == 4, "ScalarUniform only accepts int, uint, and float");

    explicit ScalarUniform(const T& scalar) : Uniform(std::addressof(scalar)) {};

    /**
     * Binds the scalar uniform bound to a given scalar variable.
     *
     * @param scalar a reference to the variable to bind to
     * @returns a pointer to the scalar uniform
     */
    static ScalarUniform* make(const T& scalar) {
        return new ScalarUniform(scalar);
    };

    uint32_t size() const override {
        return sizeof(T);
    }

    uint32_t alignment() const override {
        return sizeof(T);
    }

    void write(void* destination) const override {
        std::memcpy(destination, m_data, sizeof(T));
    }

    void bind(UniformVisitor& visitor) const override;

};

template<typename T, unsigned int S>
class VectorUniform : public Uniform {
public:
    static_assert(sizeof(T) == 4, "VectorUniform only accepts int, uint, and float.");

    explicit VectorUniform(const Vector<T, S>& vector) : Uniform(std::addressof(vector)) {};

    /**
     * Creates and binds a vector uniform bound to a given vector variable.
     *
     * @param vector a reference to the variable to bind to
     * @returns a pointer to the vector uniform
     */
    static VectorUniform* make(const Vector<T, S>& vector) {
        return new VectorUniform(vector);
    }

    uint32_t size() const override {
        return S * sizeof(T);
    }

    uint32_t alignment() const override {
        if (S == 3) {
            return 4 * sizeof(float);
        }
        else {
            return S * sizeof(float);
        }
    }

    void write(void* destination) const override {
        std::memcpy(destination, m_data, S * sizeof(T));
    }

    void bind(UniformVisitor& visitor) const override;

};

template<unsigned int C, unsigned int R>
class MatrixUniform : public Uniform {
public:
    explicit MatrixUniform(const Matrix<float, C, R>& matrix) : Uniform(std::addressof(matrix)) {};

    /**
     * Creates and binds a matrix uniform bound to a given vector variable.
     *
     * @param vector a reference to the variable to bind to
     * @returns a pointer to the matrix uniform
     */
    static MatrixUniform* make(const Matrix<float, C, R>& matrix) {
        return new MatrixUniform(matrix);
    }

    uint32_t size() const override {
        return C * alignment();
    }

    uint32_t alignment() const override {
        if (R == 3) {
            return 4 * sizeof(float);
        }
        else {
            return R * sizeof(float);
        }
    }

    void write(void* destination) const override {
        auto* m_vector = (Matrix<float, C, R>*)m_data;
        for (int i = 0; i < C; i++) {
            std::memcpy(destination, std::addressof(m_vector->operator[](i)), R * sizeof(float));
            destination = (unsigned char*)destination + alignment();
        }
    }

    void bind(UniformVisitor& visitor) const override;

};

template<class T>
class StructUniform : public Uniform {
public:
    explicit StructUniform(std::vector<Uniform*> uniforms)
            : Uniform(nullptr), m_uniforms(std::move(uniforms)) {}

    ~StructUniform() override {
        for (auto uniform: m_uniforms) {
            delete uniform;
        }
    }

    uint32_t size() const override {
        uint32_t offset = 0;
        for (auto uniform: m_uniforms) {
            offset = roundUp(offset, uniform->alignment());
            offset += uniform->size();
        }
        return roundUp(offset, alignment());
    }

    uint32_t alignment() const override {
        uint32_t maxAlignment = 0;
        for (auto uniform: m_uniforms) {
            maxAlignment = std::max(maxAlignment, uniform->alignment());
        }
        return roundUp(maxAlignment, 16); // only for std140
    }

    void write(void* destination) const override {
        auto byteDestination = static_cast<unsigned char*>(destination);
        uint32_t offset = 0;
        for (auto uniform: m_uniforms) {
            offset = roundUp(offset, uniform->alignment());
            uniform->write(byteDestination + offset);
            offset += uniform->size();
        }
    }

    const Uniform& operator[](uint32_t index) {
        if (index >= m_uniforms.size()) {
            throw std::out_of_range("Uniform index out of range.");
        }

        return *(m_uniforms[index]);
    }

    /**
     * @returns the number of uniforms in the struct
     */
    uint32_t numFields() const {
        return m_uniforms.size();
    }

private:
    std::vector<Uniform*> m_uniforms;
};

using UniformBlock = StructUniform<void>;

/*
 * IMPLEMENT FULLY ONCE DONE WITH SCALAR, VEC, MAT
template<class T, unsigned int S>
class ArrayUniform : public Uniform {
public:
    static_assert(S >= 1, "ArrayUniform must contain at least one element.");

    using BindType = std::array<T, S>;

    explicit ArrayUniform(const BindType& array) : m_array(std::addressof(array)) {}

    static ArrayUniform* bind(const BindType& array) {
        return new ArrayUniform(array);
    }

    uint32_t size() const override {
        return S * alignment();
    }

    uint32_t alignment() const override {
        return roundUp(m_uniforms[0]->alignment(), 16); // only for std140
    }

    void write(void* destination) const override {
        auto byteDestination = static_cast<unsigned char*>(destination);
        uint32_t stride = alignment();
        for (auto uniform : m_uniforms) {
            uniform->write(byteDestination);
            byteDestination += stride;
        }
    }

private:
    const BindType* m_array;
};
*/


#endif //OPENGL_RENDERER_UNIFORM_H
