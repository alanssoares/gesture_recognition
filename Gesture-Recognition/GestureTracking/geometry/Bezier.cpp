#include "Bezier.h"

Bezier::Bezier(const Vector3f p0, const Vector3f p1, float duration)
{
    this->p0 = p0;
    this->p1 = p1;
    this->p2 = Vector3f(0.f, 0.f, 0.f);
    this->p3 = Vector3f(0.f, 0.f, 0.f);
    this->duration = duration;
    type = "Linear";
};

Bezier::Bezier(const Vector3f p0, const Vector3f p1, const Vector3f p2, float duration)
{
    this->p0 = p0;
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = Vector3f(0.f, 0.f, 0.f);
    this->duration = duration;
    type = "Quadratic";
};

Bezier::Bezier(const Vector3f p0, const Vector3f p1, const Vector3f p2, const Vector3f p3, float duration)
{
    this->p0 = p0;
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->duration = duration;
    type = "Cubic";
};

void Bezier::setDuration(float duration)
{
    this->duration = duration;
}

void Bezier::setLinear(const Vector3f p0, const Vector3f p1, float duration)
{
	this->p0 = p0;
	this->p1 = p1;
	this->duration = duration;
	type = "Linear";
}

void Bezier::setQuadratic(const Vector3f p0, const Vector3f p1, const Vector3f p2, float duration)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->duration = duration;
	type = "Quadratic";

	/*
	cout << "Bezier Quadratic created:" << endl;
	cout << "\tType = " << this->type << endl;
	cout << "\tP0 = " << this->p0 << endl;
	cout << "\tP1 = " << this->p1 << endl;
	cout << "\tP2 = " << this->p2 << endl;
	cout << "\tDuration = " << this->duration << endl;
	*/
}

void Bezier::setCubic(const Vector3f p0, const Vector3f p1, const Vector3f p2, const Vector3f p3, float duration)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->duration = duration;
	type = "Cubic";

/*
	cout << "Bezier Cubic Modified:" << endl;
	cout << "\tType = " << this->type << endl;
	cout << "\tP0 = " << this->p0 << endl;
	cout << "\tP1 = " << this->p1 << endl;
	cout << "\tP2 = " << this->p2 << endl;
	cout << "\tP3 = " << this->p3 << endl;
	cout << "\tDuration = " << this->duration << endl;
*/
}


float Bezier::getDuration() const
{
    return duration;
}

string Bezier::getType() const
{
    return type;
}



Vector3f Bezier::getLinearPosition(float t) const
{
    t = t/duration;
    if(t>1.f) t=1.f;
    if(t<0.f) t=0.f;
    
    float x = (1-t)*p0.x + t*p1.x;
    float y = (1-t)*p0.y + t*p1.y;
    float z = (1-t)*p0.z + t*p1.z;

    return Vector3f(x,y,z);
}


Vector3f Bezier::getQuadraticPosition(float t) const
{
    t = t/duration;
    if(t>1.f) t=1.f;
    if(t<0.f) t=0.f;

    float x = pow((1-t),2)*p0.x + 2*(1-t)*t*p1.x + pow(t,2)*p2.x;
    float y = pow((1-t),2)*p0.y + 2*(1-t)*t*p1.y + pow(t,2)*p2.y;
    float z = pow((1-t),2)*p0.z + 2*(1-t)*t*p1.z + pow(t,2)*p2.z;

    return Vector3f(x,y,z);
}



Vector3f Bezier::getCubicPosition(float t) const
{
    t = t/duration;
    if(t>1.f) t=1.f;
    if(t<0.f) t=0.f;
    
    float x = pow((1-t),3)*p0.x + 3*pow((1-t),2)*t*p1.x + 3*(1-t)*pow(t,2)*p2.x + pow(t,3)*p3.x;
    float y = pow((1-t),3)*p0.y + 3*pow((1-t),2)*t*p1.y + 3*(1-t)*pow(t,2)*p2.y + pow(t,3)*p3.y;
    float z = pow((1-t),3)*p0.z + 3*pow((1-t),2)*t*p1.z + 3*(1-t)*pow(t,2)*p2.z + pow(t,3)*p3.z;
    
    return Vector3f(x,y,z);
}









