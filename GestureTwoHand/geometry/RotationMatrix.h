#ifndef ROTATIONMATRIX_H
#define ROTATIONMATRIX_H

#include <cmath>
#include <iomanip>
#include "Vector3f.h"




class RotationMatrix
{
protected:


public:
    Vector3f l0, l1, l2; //LINES
    
    
    RotationMatrix();
    RotationMatrix(const Vector3f& lin0, const Vector3f& lin1, const Vector3f& lin2);

    RotationMatrix operator*(const RotationMatrix& other) const;
    Vector3f operator*(const Vector3f& vector) const;
    
    RotationMatrix& rotateX(const float angle);
    RotationMatrix& rotateY(const float angle);
    RotationMatrix& rotateZ(const float angle);
    RotationMatrix getInverse() const;
    
    
    friend std::ostream& operator<<(std::ostream& os, const RotationMatrix& r);   
    
    
private:
    
    
};

RotationMatrix GetRotationMatrixX(const float& angle);
RotationMatrix GetRotationMatrixY(const float& angle);
RotationMatrix GetRotationMatrixZ(const float& angle);

#endif // ROTATIONMATRIX_H
