#ifndef MATRIXMXN_H
#define MATRIXMXN_H

#include <iostream>


/**
 * @class MatrixMxN
 *
 * This class represents a M lines by N columns matrix and containts methods
 * to operate on it
 *
 * @author Rui Ferreira
 */


/*
 * TODO's
 *
 * ERROR CHECKING
 *
 */




class MatrixMxN {
private:
	int dx, dy;
	float **pData;    // content of the matrix


private:
    void AllocArrays();

public:


    //MatrixMxN();
	MatrixMxN(int sizeX = 1, int sizeY = 1);
	//MatrixMxN(const float **m);
	MatrixMxN(const MatrixMxN& other);

	~MatrixMxN();



	MatrixMxN& operator=(const MatrixMxN& other);

	MatrixMxN operator+(const MatrixMxN& other) const;
	MatrixMxN& operator+=(const MatrixMxN& other);

	MatrixMxN operator-(const MatrixMxN& other) const;
	MatrixMxN& operator-=(const MatrixMxN& other);

	MatrixMxN& operator*=(const MatrixMxN& other);
	friend MatrixMxN operator*(const MatrixMxN& m1, const MatrixMxN& m2);
	friend MatrixMxN operator*(float c, const MatrixMxN& m);
	friend MatrixMxN operator*(const MatrixMxN& m, float c);



	float& Get(int x, int y);
	float& operator()(int x, int y);
	//float& operator[](int x, int y);



	friend std::ostream& operator<<(std::ostream& os, MatrixMxN& mat);



	static MatrixMxN Zeros(int x, int y);
	static MatrixMxN Ones(int x, int y);
	static MatrixMxN Identity(int x);
	static MatrixMxN Transpose(const MatrixMxN& m);
	static MatrixMxN Inverse(const MatrixMxN& m);





	/*
	MatrixMxND operator*(const MatrixMxND& other) const;
	Vector3f operator*(const Vector3f& other) const;

	bool operator==(const MatrixMxND&) const;

	float& operator[](const unsigned pos);
	*/


};


#endif // MatrixMxN_H_
