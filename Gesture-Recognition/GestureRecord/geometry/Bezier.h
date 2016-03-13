#ifndef BEZIER_H
#define BEZIER_H

#include <string>
#include <cmath>
#include <XnCppWrapper.h>

#include "Vector3f.h"



class Bezier
{
public:
	Bezier(){};
    Bezier(const Vector3f p0, const Vector3f p1, float duration=1.f);
    Bezier(const Vector3f p0, const Vector3f p1, const Vector3f p2, float duration=1.f);
    Bezier(const Vector3f p0, const Vector3f p1, const Vector3f p2, const Vector3f p3, float duration=1.f);
    
    void setDuration(float duration);
    void setLinear(const Vector3f p0, const Vector3f p1, float duration=1.f);
    void setQuadratic(const Vector3f p0, const Vector3f p1, const Vector3f p2, float duration);
    void setCubic(const Vector3f p0, const Vector3f p1, const Vector3f p2, const Vector3f p3, float duration);
    void setCubic(const XnPoint3D p0, const XnPoint3D p1, const XnPoint3D p2, const XnPoint3D p3, float duration);
    float getDuration() const;
    string getType() const;
    Vector3f getCubicPosition(float t) const;
    Vector3f getQuadraticPosition(float t) const;

    Vector3f getLinearPosition(float t) const;

    
private:
    Vector3f p0, p1, p2, p3;
    string type;
    float duration;

};

#endif // BEZIER_H
