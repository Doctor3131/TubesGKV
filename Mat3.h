// Matrix3D.h
#ifndef MATRIX3D_H
#define MATRIX3D_H

#include <vector>
#include <stdexcept>

template <typename T>
class Mat3d {
public:
    Mat3d(size_t depth, size_t rows, size_t cols)
        : d(depth), r(rows), c(cols), data(depth * rows * cols) {}

    T& at(size_t z, size_t y, size_t x) {
        validate_indices(z, y, x);
        return data[z * r * c + y * c + x];
    }

    const T& at(size_t z, size_t y, size_t x) const {
        validate_indices(z, y, x);
        return data[z * r * c + y * c + x];
    }

    size_t depth() const { return d; }
    size_t rows() const { return r; }
    size_t cols() const { return c; }

private:
    size_t d, r, c;
    std::vector<T> data;

    void validate_indices(size_t z, size_t y, size_t x) const {
        if (z >= d || y >= r || x >= c) {
            throw std::out_of_range("Mat3d indices out of range");
        }
    }
};

#endif // MATRIX3D_H
