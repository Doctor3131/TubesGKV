#pragma once
#include <cmath>

struct Vector3 {
    float x;
    float y;
    float z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

    // Constructor with initial values
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Addition
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    // Subtraction
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    // Multiplication by a scalar
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    // Division by a scalar
    Vector3 operator/(float scalar) const {
        if (scalar == 0.0f) {
            // Handle division by zero appropriately, e.g., return a zero vector
            return Vector3(0.0f, 0.0f, 0.0f);
        }
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    Vector3& operator/=(float scalar) {
        if (scalar == 0.0f) {
            // Handle division by zero appropriately
            return *this;
        }
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    // Unary negation
    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

    // Comparison (equality)
    bool operator==(const Vector3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    // Comparison (inequality)
    bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }

    // Length squared
    float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    // Length
    float length() const {
        return std::sqrt(lengthSquared());
    }

    // Normalize
    Vector3 normalized() const {
        float len = length();
        if (len > 0.0f) {
            return Vector3(x / len, y / len, z / len);
        }
        return *this;
    }

    // Dot product
    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 cross(const Vector3& other) const {
        return Vector3(y * other.z - z * other.y,
                       z * other.x - x * other.z,
                       x * other.y - y * other.x);
    }

    Vector3 rotate(float angle, const Vector3& axis) const {
        float s = sin(angle);
        float c = cos(angle);
        Vector3 k = axis.normalized(); // Ensure axis is a unit vector

        return (*this) * c + (k.cross(*this)) * s + k * (k.dot(*this)) * (1.0f - c);
    }

    // Helper for rotation (matrix-vector multiplication)
    Vector3 transform(const Vector3& other) const {
        return Vector3(
            x * other.x + y * other.y + z * other.z,
            x * other.x + y * other.y + z * other.z,
            x * other.x + y * other.y + z * other.z
        );
    }
};

// Non-member scalar multiplication (vec * scalar)
inline Vector3 operator*(float scalar, const Vector3& vec) {
    return vec * scalar;
}