#include "HomogMatrix.h"
#include <iomanip>



HomogMatrix::HomogMatrix()
{
    for(int i=0; i<4; i++)
      for(int j=0; j<4; j++)
	elem[i][j]=0;
      
    for(int i=0; i<4; i++)
      elem[i][i] = 1;
  
}

HomogMatrix::HomogMatrix(const RotationMatrix& rot, const Vector3f& trans)
{
  elem[0][0] = rot.l0.x;
  elem[0][1] = rot.l0.y;
  elem[0][2] = rot.l0.z;
  elem[1][0] = rot.l1.x;
  elem[1][1] = rot.l1.y;
  elem[1][2] = rot.l1.z;
  elem[2][0] = rot.l2.x;
  elem[2][1] = rot.l2.y;
  elem[2][2] = rot.l2.z;
  
  elem[0][3] = trans.x;
  elem[1][3] = trans.y;
  elem[2][3] = trans.z;
  
  elem[3][0] = elem[3][1] = elem[3][2] = 0;
  elem[3][3] = 1;
}

HomogMatrix::HomogMatrix(const Vector3f& trans)
{
  elem[0][0] = 1;
  elem[0][1] = 0;
  elem[0][2] = 0;
  elem[1][0] = 0;
  elem[1][1] = 1;
  elem[1][2] = 0;
  elem[2][0] = 0;
  elem[2][1] = 0;
  elem[2][2] = 1;
  
  elem[0][3] = trans.x;
  elem[1][3] = trans.y;
  elem[2][3] = trans.z;
  
  elem[3][0] = elem[3][1] = elem[3][2] = 0;
  elem[3][3] = 1;
}

HomogMatrix::HomogMatrix(const RotationMatrix& rot)
{
  elem[0][0] = rot.l0.x;
  elem[0][1] = rot.l0.y;
  elem[0][2] = rot.l0.z;
  elem[1][0] = rot.l1.x;
  elem[1][1] = rot.l1.y;
  elem[1][2] = rot.l1.z;
  elem[2][0] = rot.l2.x;
  elem[2][1] = rot.l2.y;
  elem[2][2] = rot.l2.z;
  
  elem[0][3] = 0;
  elem[1][3] = 0;
  elem[2][3] = 0;
  
  elem[3][0] = elem[3][1] = elem[3][2] = 0;
  elem[3][3] = 1;
}



RotationMatrix HomogMatrix::getRotation()
{
    RotationMatrix result;
    result.l0.x = elem[0][0];
    result.l0.y = elem[0][1];
    result.l0.z = elem[0][2];
    result.l1.x = elem[1][0];
    result.l1.y = elem[1][1];
    result.l1.z = elem[1][2];
    result.l2.x = elem[2][0];
    result.l2.y = elem[2][1];
    result.l2.z = elem[2][2];
    return result;
}

Vector3f HomogMatrix::getTranslation()
{
    return Vector3f(elem[0][3], elem[1][3], elem[2][3]);
}



ostream& operator<<(ostream& os, const HomogMatrix& h) {
    os << setiosflags(ios::fixed) << setprecision(3);
    os << h.elem[0][0] << ", " << h.elem[0][1] << ", " << h.elem[0][2] << ", " << h.elem[0][3] << endl;
    os << h.elem[1][0] << ", " << h.elem[1][1] << ", " << h.elem[1][2] << ", " << h.elem[1][3] << endl;
    os << h.elem[2][0] << ", " << h.elem[2][1] << ", " << h.elem[2][2] << ", " << h.elem[2][3] << endl;
    os << h.elem[3][0] << ", " << h.elem[3][1] << ", " << h.elem[3][2] << ", " << h.elem[3][3];
    return os;
}

HomogMatrix HomogMatrix::operator*(const HomogMatrix& other) const
{
    HomogMatrix result;
    
    result.elem[0][0] = elem[0][0]*other.elem[0][0] + elem[0][1]*other.elem[1][0] + elem[0][2]*other.elem[2][0] + elem[0][3]*other.elem[3][0];
    result.elem[0][1] = elem[0][0]*other.elem[0][1] + elem[0][1]*other.elem[1][1] + elem[0][2]*other.elem[2][1] + elem[0][3]*other.elem[3][1];
    result.elem[0][2] = elem[0][0]*other.elem[0][2] + elem[0][1]*other.elem[1][2] + elem[0][2]*other.elem[2][2] + elem[0][3]*other.elem[3][2];
    result.elem[0][3] = elem[0][0]*other.elem[0][3] + elem[0][1]*other.elem[1][3] + elem[0][2]*other.elem[2][3] + elem[0][3]*other.elem[3][3];
    
    result.elem[1][0] = elem[1][0]*other.elem[0][0] + elem[1][1]*other.elem[1][0] + elem[1][2]*other.elem[2][0] + elem[1][3]*other.elem[3][0];
    result.elem[1][1] = elem[1][0]*other.elem[0][1] + elem[1][1]*other.elem[1][1] + elem[1][2]*other.elem[2][1] + elem[1][3]*other.elem[3][1];
    result.elem[1][2] = elem[1][0]*other.elem[0][2] + elem[1][1]*other.elem[1][2] + elem[1][2]*other.elem[2][2] + elem[1][3]*other.elem[3][2];
    result.elem[1][3] = elem[1][0]*other.elem[0][3] + elem[1][1]*other.elem[1][3] + elem[1][2]*other.elem[2][3] + elem[1][3]*other.elem[3][3];    
    
    result.elem[2][0] = elem[2][0]*other.elem[0][0] + elem[2][1]*other.elem[1][0] + elem[2][2]*other.elem[2][0] + elem[2][3]*other.elem[3][0];
    result.elem[2][1] = elem[2][0]*other.elem[0][1] + elem[2][1]*other.elem[1][1] + elem[2][2]*other.elem[2][1] + elem[2][3]*other.elem[3][1];
    result.elem[2][2] = elem[2][0]*other.elem[0][2] + elem[2][1]*other.elem[1][2] + elem[2][2]*other.elem[2][2] + elem[2][3]*other.elem[3][2];
    result.elem[2][3] = elem[2][0]*other.elem[0][3] + elem[2][1]*other.elem[1][3] + elem[2][2]*other.elem[2][3] + elem[2][3]*other.elem[3][3];    
    
    result.elem[3][0] = elem[3][0]*other.elem[0][0] + elem[3][1]*other.elem[1][0] + elem[3][2]*other.elem[2][0] + elem[3][3]*other.elem[3][0];
    result.elem[3][1] = elem[3][0]*other.elem[0][1] + elem[3][1]*other.elem[1][1] + elem[3][2]*other.elem[2][1] + elem[3][3]*other.elem[3][1];
    result.elem[3][2] = elem[3][0]*other.elem[0][2] + elem[3][1]*other.elem[1][2] + elem[3][2]*other.elem[2][2] + elem[3][3]*other.elem[3][2];
    result.elem[3][3] = elem[3][0]*other.elem[0][3] + elem[3][1]*other.elem[1][3] + elem[3][2]*other.elem[2][3] + elem[3][3]*other.elem[3][3];
    
    return result;
}



void HomogMatrix::setTranslation(const Vector3f& trans)
{
    elem[0][3] = trans.x;
    elem[1][3] = trans.y;
    elem[2][3] = trans.z;
}

void HomogMatrix::translate(const float& x, const float& y, const float& z)
{
    elem[0][3] += x;
    elem[1][3] += y;
    elem[2][3] += z; 
}

void HomogMatrix::translate(const Vector3f& trans)
{
    elem[0][3] += trans.x;
    elem[1][3] += trans.y;
    elem[2][3] += trans.z; 
}



void HomogMatrix::setRotation(const RotationMatrix& rot)
{
  elem[0][0] = rot.l0.x;
  elem[0][1] = rot.l0.y;
  elem[0][2] = rot.l0.z;
  elem[1][0] = rot.l1.x;
  elem[1][1] = rot.l1.y;
  elem[1][2] = rot.l1.z;
  elem[2][0] = rot.l2.x;
  elem[2][1] = rot.l2.y;
  elem[2][2] = rot.l2.z;
}




HomogMatrix HomogMatrix::getInverse()
{
    HomogMatrix result;

    RotationMatrix rot = RotationMatrix(getRotation());
    Vector3f trans = Vector3f(getTranslation());
    
    result.setRotation(rot.getInverse());
    result.setTranslation(result.getRotation() * (-trans));
    return result;
}




