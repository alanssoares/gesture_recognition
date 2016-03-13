#ifndef HOMOGMATRIX_H
#define HOMOGMATRIX_H

#include <sstream>
#include "RotationMatrix.h"

class HomogMatrix
{
public:
	float elem[4][4];
	
	HomogMatrix();
	HomogMatrix(const RotationMatrix& rot, const Vector3f& trans);
	HomogMatrix(const Vector3f& trans);
	HomogMatrix(const RotationMatrix& rot);

	
	HomogMatrix operator*(const HomogMatrix& other) const;
	friend std::ostream& operator<<(std::ostream& os, const HomogMatrix& h);
	
	
	void setTranslation(const Vector3f& trans);
	void setRotation(const RotationMatrix& rot);
	void translate(const float& x, const float& y, const float& z);
	void translate(const Vector3f& trans);
	Vector3f getTranslation();
	RotationMatrix getRotation();
	
	HomogMatrix getInverse();
	
private:
};

#endif // HOMOGMATRIX_H