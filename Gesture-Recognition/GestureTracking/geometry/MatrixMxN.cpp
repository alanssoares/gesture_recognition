#include "MatrixMxN.h"


using namespace std;



void MatrixMxN::AllocArrays()
{
	pData = new float*[dx];

	for(int i=0; i<dx; i++)
		pData[i] = new float[dy];
}


MatrixMxN::MatrixMxN(int sizeX, int sizeY) : dx(sizeX), dy(sizeY)
{
	AllocArrays();
	for(int i=0; i<dx; i++)
		for(int j=0; j<dy; j++)
			pData[i][j] = 0;
}




MatrixMxN::MatrixMxN(const MatrixMxN& other) : dx(other.dx), dy(other.dy)
{
	AllocArrays();
	for(int i=0; i<dx; i++)
		for(int j=0; j<dy; j++)
			pData[i][j] = other.pData[i][j];

}


MatrixMxN::~MatrixMxN()
{
	for(int i=0; i<dx; i++)
		delete [] pData[i];
	delete [] pData;
}


MatrixMxN& MatrixMxN::operator=(const MatrixMxN& other)
{
	if(this == &other) //avoid self assignment
		return *this;
	else
	{
		if(dx!=other.dx || dy!=other.dy) //Resize matrix
		{
			this->~MatrixMxN();
			dx = other.dx;
			dy = other.dy;
			AllocArrays();
		}
		for(int i=0; i<dx; i++)
			for(int j=0; j<dy; j++)
				pData[i][j] = other.pData[i][j];
	}
	return *this;
}





MatrixMxN MatrixMxN::operator+(const MatrixMxN& other) const
{
	MatrixMxN temp(*this);

	return (temp += other);
}

MatrixMxN& MatrixMxN::operator+=(const MatrixMxN& other)
{
	for(int i=0; i<dx; i++)
		for(int j=0; j<dy; j++)
			pData[i][j] += other.pData[i][j];
	return *this;
}

MatrixMxN MatrixMxN::operator-(const MatrixMxN& other) const
{
	MatrixMxN temp(*this);

	return (temp -= other);
}

MatrixMxN& MatrixMxN::operator-=(const MatrixMxN& other)
{
	for(int i=0; i<dx; i++)
		for(int j=0; j<dy; j++)
			pData[i][j] -= other.pData[i][j];
	return *this;
}

MatrixMxN& MatrixMxN::operator*=(const MatrixMxN& other)
{
	MatrixMxN prod(dx, other.dy);

	for(int i=0; i<dx; i++)
		for(int j=0; j<dy; j++)
			for(int k=0; k<dy; k++)
				prod.pData[i][j] += pData[i][k] * other.pData[k][j];

	return *this;
}

MatrixMxN operator*(const MatrixMxN& m1, const MatrixMxN& m2)
{
	MatrixMxN prod(m1.dx, m2.dy);

	for(int i=0; i<prod.dx; i++)
		for(int j=0; j<prod.dy; j++)
			for(int k=0; k<m1.dy; k++)
				prod.pData[i][j] += m1.pData[i][k] * m2.pData[k][j];

	return prod;
}

MatrixMxN operator*(float c, const MatrixMxN& m)
{
	MatrixMxN prod(m);

	for(int i=0; i<prod.dx; i++)
		for(int j=0; j<prod.dy; j++)
			prod.pData[i][j] = c * m.pData[i][j];

	return prod;
}

MatrixMxN operator*(const MatrixMxN& m, float c)
{
	return c*m;
}





float& MatrixMxN::Get(int x, int y)
{
	return pData[x][y];
}





std::ostream& operator<<(ostream& os, MatrixMxN& mat)
{
	for(int i=0; i<mat.dx; i++)
	{
		for(int j=0; j<mat.dy; j++)
			cout << mat.pData[i][j] << "\t";
		cout << endl;
	}
	return os;
}


MatrixMxN MatrixMxN::Zeros(int x, int y)
{
	MatrixMxN aux(x, y);

	for(int i=0; i<aux.dx; i++)
		for(int j=0; j<aux.dy; j++)
			aux.Get(i, j) = 0;

	return aux;
}


MatrixMxN MatrixMxN::Ones(int x, int y)
{
	MatrixMxN aux(x, y);

	for(int i=0; i<aux.dx; i++)
		for(int j=0; j<aux.dy; j++)
			aux.Get(i, j) = 1;

	return aux;
}

MatrixMxN MatrixMxN::Identity(int x)
{
	MatrixMxN aux = Zeros(x, x);

	for(int i=0; i<x; i++)
		aux.Get(i, i) = 1;

	return aux;
}

MatrixMxN MatrixMxN::Transpose(const MatrixMxN& m)
{
	MatrixMxN aux(m.dy, m.dx);

	for(int i=0; i<aux.dx; i++)
		for(int j=0; j<aux.dy; j++)
			aux.pData[i][j] = m.pData[j][i];

	return aux;
}




