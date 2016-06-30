#ifndef MATRIX4D_H_
#define MATRIX4D_H_

#include "Vector3f.h"
#include <iostream>

#define M_ROWS      4
#define M_COLS      4
#define M_LENGTH    (M_ROWS * M_COLS)

/**
 * @class Matrix4D
 *
 * This class represents a 4x4 matrix and containts methods
 * to operate on it
 *
 * @author Nuno Almeida (nuno.alm@ua.pt)
 */
class Matrix4D {
    float content[M_LENGTH];    // content of the matrix, vector-like

    public:
        /**
         * Default constructor returns the identity matrix
         */
        Matrix4D();

        /**
         * Constructor returns a matrix from a vector of floats
         */
        Matrix4D(const float[]);

        /**
         * Copy constructor
         */
        Matrix4D(const Matrix4D& other);

        /**
         * Constructor returns a translation matrix
         */
        Matrix4D(const Vector3f& v);

        /**
         * Destructor
         */
        ~Matrix4D();

        /**
         * Sets a value in some position (vector-like)
         */
        void set(unsigned i, float value);

        /**
         * Sets a value in some position (matrix-like)
         */
        void set(unsigned i, unsigned j, float value);

        /**
         * Gets a value from some position (vector-like)
         */
        float get(unsigned i);

        /**
         * Gets a value from some position (matrix-like)
         */
        float get(unsigned i, unsigned j);

        /**
         * Assigns another matrix to this one
         *
         * @param other Another matrix
         */
        void operator=(const Matrix4D& other);

        /**
         * Gets the sum of another vector with this one
         *
         * @param other Another matrix
         */
        Matrix4D operator+(const Matrix4D& other) const;

        /**
         * Sums this matrix to another
         *
         * @param other Another matrix
         * @return Sum of this matrix with another
         */
        void operator+=(const Matrix4D& other);

        /**
         * Gets the subtraction of another vector from this one
         *
         * @param other Another matrix
         */
        Matrix4D operator-(const Matrix4D&) const;

        /**
         * Subtracts another matrix from this one
         *
         * @param other Another matrix
         * @return This matrix minus another
         */
        void operator-=(const Matrix4D& other);

        /**
         * Multiplies two matrices
         *
         * @param other Another matrix
         * @return Multiplication matrix
         */
        Matrix4D operator*(const Matrix4D& other) const;

        /**
         * Multiplies a matrix with a vector
         *
         * @param other Another matrix
         * @return Multiplication vector
         */
        Vector3f operator*(const Vector3f& other) const;

        /**
         * Checks whether this matrix is equal to another
         *
         * @param other Another matrix
         * @return true/false
         */
        bool operator==(const Matrix4D&) const;

        /**
         * Gets the content of the position i (in vector representation) of this
         * matrix
         *
         * @param pos Position
         * @return Value in the position
         */
        float& operator[](const unsigned pos);

        /**
         * Gets the translation vector from this matrix
         *
         * @return Translation vector
         */
        Vector3f toVector3f();

        /**
         * Gets the transpose of this matrix
         *
         * @return Transpose
         */
        Matrix4D transpose();

        void print();

        /**
         * Gets the rotation matrix around x-axis
         *
         * @param angle Angle (degrees)
         */
        static Matrix4D rotationX(float angle);

        /**
         * Gets the rotation matrix around y-axis
         *
         * @param angle Angle (degrees)
         */
        static Matrix4D rotationY(float angle);

        /**
         * Gets the rotation matrix around z-axis
         *
         * @param angle Angle (degrees)
         */
        static Matrix4D rotationZ(float angle);

        /**
         * Gets the rotation matrix around an arbitrary axis
         *
         * @param axis Axis (x, y and z)
         * @param angle Angle (degrees)
         */
        static Matrix4D rotation(Vector3f axis, float angle);

        /**
         * Gets the translation matrix
         *
         * @param x x-axis coordinate
         * @param y y-axis coordinate
         * @param z z-axis coordinate
         */
        static Matrix4D translation(Vector3f v) { return translation(v.getX(),v.getY(),v.getZ()); }
        static Matrix4D translation(float x, float y, float z);

        /**
         * Prints the matrix in the output stream
         */
        friend std::ostream& operator<<(std::ostream& os, Matrix4D& mat);
};

#endif // MATRIX4D_H_
