#ifndef TEST_H__
#define TEST_H__

#include "../../Commons/commons.hpp"
#include "../util/Util.h"

class Test {

public:
	Test();
	~Test();

	static float getFinalTime(std::clock_t start_s);

	//Main method that initialize the execution of the tests
	void executeAll();
	//Execute the tests using the normal Samples.txt
	void executeNormal(std::string folder);
	//Execute the tests using Samples.txt with all gestures with same length
	void executeWithEqualSize(std::string folder);
	//Execute the tests using the median gestures as templates
	void executeWithMedian(std::string folder);
	//Execute the especific test according with the methods above
	void execute(std::string folder);
	//Transform all gestures to equal number of points
	void transformAllToEqualSize();
	//Clear all vectores of samples
	void clearSamples();
	//Loal all gestures of the file Samples.txt
	void loadAll();
	//Loal all median gestures of the file SamplesMedian.txt
	void loadMedian();
	//Split all gestures in X percent for test and Y percent for template
	void splitDataset();
	//Init loading both Samples.txt and SamplesMedian.txt
	void init();
	//Save the result after the execution of the recognize method
	void saveResults(std::string nameFile, type_gesture gestureExecuted, type_gesture gesturePredicted, float timeExecution, float bestDistance, int isRecognized);
	//Execute the recognize of the gesture test and save the result
	void recognize(const type_gesture gesture, const std::string nameFile);
	//Apply an process according with the environment
	void applyProcess(int env);
	//Apply the method Laplacian
	void process1();
	//Apply the method Curvature
	void process2();
	//Apply both Laplacian and Curvature
	void process3();
	//Apply the method DouglasPeucker
	void process4();
	//Apply both Laplacian and DouglasPeucker
	void process5();
	//Create the specific result file, apply procees and execute all tests
	void experiment(int env, std::string nameFile);

public:
	std::vector<type_gesture> m_MedianGestures;
	std::vector<type_gesture> m_GesturesTemplate;
	std::vector<type_gesture> m_GesturesTest;
	std::vector<type_gesture> m_AllGestures;

	std::string m_NameGestureRecognized;
	std::vector<type_gesture> m_GesturesOneHand;
	std::vector<type_gesture> m_GesturesTwoHands;

	float m_PercentTest, m_RecThreshold;
	bool 	m_isMedian;

	Util m_Util;
};

#endif //TEST_H__
