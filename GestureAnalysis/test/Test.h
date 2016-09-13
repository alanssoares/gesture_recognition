#ifndef TEST_H__
#define TEST_H__

#include "../../Commons/commons.hpp"

class Test {

public:
	Test();
	~Test();

	static bool sortByName(const type_gesture &g1, const type_gesture &g2) { return g1.name < g2.name; }
	static float getFinalTime(std::clock_t start_s);

	void executeAll();

	void transformAllToEqualSize();
	void printGesture(const type_gesture gesture);
	void clearSamples();
	void loadAll();
	void loadMedian();
	void splitDataset();
	void init();

	void saveResults(std::string nameFile, type_gesture gestureExecuted, type_gesture gesturePredicted, float timeExecution, float bestDistance, int isRecognized);
	void saveMedianGestures();
	void recognize(const type_gesture gesture, const std::string nameFile);
	void generateMedianGesture(std::vector<type_gesture> gestures);
	int  getMeanPoints(vector<type_gesture> gestures, int i, int j);
	void generateGestureEqualSize(std::vector<type_gesture>* gestures);
	void normCenterOriginGesture(type_gesture *gesture);

	void process1();
	void process2();
	void process3();
	void process4();
	void process5();

	void applyLaplacian(std::vector<type_gesture>* gestures);
	void applyBSpline(std::vector<type_gesture>* gestures);
	void applyCurvature(std::vector<type_gesture>* gestures);
	void applyDouglasPeucker(std::vector<type_gesture>* gestures);
	void applyProcess(int env);

	void experiment(int env, std::string nameFile);

	void improveGestures();

public:
	std::vector<type_gesture> m_MedianGestures;
	std::vector<type_gesture> m_GesturesTemplate;
	std::vector<type_gesture> m_GesturesTest;
	std::vector<type_gesture> m_AllGestures;

	std::string m_NameGestureRecognized;
	std::vector<type_gesture> m_GesturesOneHand;
	std::vector<type_gesture> m_GesturesTwoHands;

	float m_PercentTest, m_RecThreshold, m_CurvThreshold, m_DougThreshold;
};

#endif //TEST_H__
