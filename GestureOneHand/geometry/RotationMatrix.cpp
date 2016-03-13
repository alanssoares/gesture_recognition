#include "RotationMatrix.h"


RotationMatrix::RotationMatrix()
{
    l0 = Vector3f(1,0,0);
    l1 = Vector3f(0,1,0);
    l2 = Vector3f(0,0,1);
}

RotationMatrix::RotationMatrix(const Vector3f& lin0, const Vector3f& lin1, const Vector3f& lin2)
{
    l0 = lin0;
    l1 = lin1;
    l2 = lin2;
}

RotationMatrix RotationMatrix::operator*(const RotationMatrix& other) const
{
    RotationMatrix result;

    result.l0.x = l0.x*other.l0.x + l0.y*other.l1.x + l0.z*other.l2.x;
    result.l0.y = l0.x*other.l0.y + l0.y*other.l1.y + l0.z*other.l2.y;
    result.l0.z = l0.x*other.l0.z + l0.y*other.l1.z + l0.z*other.l2.z;

    result.l1.x = l1.x*other.l0.x + l1.y*other.l1.x + l1.z*other.l2.x;
    result.l1.y = l1.x*other.l0.y + l1.y*other.l1.y + l1.z*other.l2.y;
    result.l1.z = l1.x*other.l0.z + l1.y*other.l1.z + l1.z*other.l2.z;

    result.l2.x = l2.x*other.l0.x + l2.y*other.l1.x + l2.z*other.l2.x;
    result.l2.y = l2.x*other.l0.y + l2.y*other.l1.y + l2.z*other.l2.y;
    result.l2.z = l2.x*other.l0.z + l2.y*other.l1.z + l2.z*other.l2.z;

    return result;
}


Vector3f RotationMatrix::operator*(const Vector3f& vector) const
{
    return Vector3f(l0.x*vector.x + l0.y*vector.y + l0.z*vector.z,
		    l1.x*vector.x + l1.y*vector.y + l1.z*vector.z,
		    l2.x*vector.x + l2.y*vector.y + l2.z*vector.z );
}

ostream& operator<<(ostream& os, const RotationMatrix& r)
{
    os << setiosflags(ios::fixed) << setprecision(3);
    os << r.l0.x << ", " << r.l0.y << ", " << r.l0.z << endl;
    os << r.l1.x << ", " << r.l1.y << ", " << r.l1.z << endl;
    os << r.l2.x << ", " << r.l2.y << ", " << r.l2.z;
    return os;
}



RotationMatrix RotationMatrix::getInverse() const
{
    //The inverse of a rotation matrix is its transpose, which is also a rotation matrix
    return RotationMatrix(Vector3f(l0.x, l1.x, l2.x),
			   Vector3f(l0.y, l1.y, l2.y),
			   Vector3f(l0.z, l1.z, l2.z));
}


RotationMatrix& RotationMatrix::rotateX(const float angle)
{
	const float c = cos(angle), s = sin(angle);
	RotationMatrix aux = *this;
	
	l0.y = c*aux.l0.y + s*aux.l0.z;
	l1.y = c*aux.l1.y + s*aux.l1.z;
	l2.y = c*aux.l2.y + s*aux.l2.z;
	
	l0.z = -s*aux.l0.y + c*aux.l0.z;
	l1.z = -s*aux.l1.y + c*aux.l1.z;
	l2.z = -s*aux.l2.y + c*aux.l2.z;
	
	return *this;
}

RotationMatrix& RotationMatrix::rotateY(const float angle)
{
	const float c = cos(angle), s = sin(angle);
	RotationMatrix aux = *this;
	
	l0.x = c*aux.l0.x - s*aux.l0.z;
	l1.x = c*aux.l1.x - s*aux.l1.z;
	l2.x = c*aux.l2.x - s*aux.l2.z;
	
	l0.z = s*aux.l0.x + c*aux.l0.z;
	l1.z = s*aux.l1.x + c*aux.l1.z;
	l2.z = s*aux.l2.x + c*aux.l2.z;
	
	return *this;
}

RotationMatrix& RotationMatrix::rotateZ(const float angle)
{
	const float c = cos(angle), s = sin(angle);
	RotationMatrix aux = *this;
	
	l0.x = c*aux.l0.x + s*aux.l0.y;
	l1.x = c*aux.l1.x + s*aux.l1.y;
	l2.x = c*aux.l2.x + s*aux.l2.y;
	
	l0.y = -s*aux.l0.x + c*aux.l0.y;
	l1.y = -s*aux.l1.x + c*aux.l1.y;
	l2.y = -s*aux.l2.x + c*aux.l2.y;	
	return *this;
}





RotationMatrix GetRotationMatrixX(const float& angle)
{
    RotationMatrix result;
    result.l0 = Vector3f(1, 0, 0);
    result.l1 = Vector3f(0, cos(angle), -sin(angle));
    result.l2 = Vector3f(0, sin(angle), cos(angle));
    return result;
}

RotationMatrix GetRotationMatrixY(const float& angle)
{
    RotationMatrix result;
    result.l0 = Vector3f(cos(angle), 0, sin(angle));
    result.l1 = Vector3f(0, 1, 0);
    result.l2 = Vector3f(-sin(angle), 0, cos(angle));    
    return result;
}

RotationMatrix GetRotationMatrixZ(const float& angle)
{
    RotationMatrix result;
    result.l0 = Vector3f(cos(angle), -sin(angle), 0);
    result.l1 = Vector3f(sin(angle), cos(angle), 0);    
    result.l2 = Vector3f(0, 0, 1);
    return result;  
}



