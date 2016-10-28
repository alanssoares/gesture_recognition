#ifndef UTIL_H__
#define UTIL_H__

#include "../../Commons/commons.hpp"

class Util {

public:
	Util();
	~Util();

	static bool sortByName(const type_gesture &g1, const type_gesture &g2) { return g1.name < g2.name; }

	//Print all informations of the gesture
	void printGesture(const type_gesture gesture);
	//Get the mean number of points of an set of gestures of the same type
	int  getMeanPoints(vector<type_gesture> gestures, int i, int j);
	//Generate a file with median gestures
	void generateMedianGestures();
	//Generate gestures with equal number of points
	void generateGestureEqualSize(std::vector<type_gesture> *gestures);
	//Generate a file with gestures centered in the origin using centroid
	void normCenterOriginGesture(type_gesture *gesture);
	//Apply normalization in the interval [-1,1] in the gestures
	void applyNormalization(type_gesture *gesture);
	void applyNormalization(std::vector<type_gesture>* gestures);
	//Apply the method Laplacian in the gestures
	void applyLaplacian(type_gesture *gesture);
	void applyLaplacian(std::vector<type_gesture> *gestures);
	//Apply the method uniform B-Spline in the gestures
	void applyUniformBSpline(type_gesture *gesture);
	void applyUniformBSpline(std::vector<type_gesture> *gestures);
	//Apply the method B-Spline in the gestures
	void applyBSpline(type_gesture *gesture);
	void applyBSpline(std::vector<type_gesture> *gestures);
	//Apply the method Curvature for simplification in the gestures
	void applyCurvature(type_gesture *gesture);
	void applyCurvature(std::vector<type_gesture> *gestures);
	//Apply the method DouglasPeucker for simplification in the gestures
	void applyDouglasPeucker(type_gesture *gesture);
	void applyDouglasPeucker(std::vector<type_gesture> *gestures);
	//Generate a new file with gestures centered in the origin
	void centerOriginGestures();
	//Generate a new file scaled in the interval [-1,1]
	void scaleGestures();

	float m_CurvThreshold, m_DougThreshold;
	bool 	m_isMedian;
};

#endif //UTIL_H__
