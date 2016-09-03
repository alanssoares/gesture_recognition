#ifndef TEST_H__
#define TEST_H__

#include "../../Commons/commons.hpp"

class Test {

public:
	Test();
	~Test();

	static bool sortByName(const type_gesture &g1, const type_gesture &g2) { return g1.name < g2.name; }
	static float getFinalTime(std::clock_t start_s);

	void printGesture(const type_gesture gesture);
	
	void clearSamples();
	void loadAll();
	
	void splitDataset();
	
	void initGestureTemplates();
	void initAllSamples();
	void init();
	void executeAll();
	
	void saveResults(std::string nameFile, type_gesture gestureExecuted, type_gesture gesturePredicted, float timeExecution, float bestDistance, int isRecognized);
	void saveMedianGestures();
	
	void recognizeOneHand(const type_gesture gesture, const std::string nameFile);
	void recognizeTwoHands(const type_gesture gesture, const std::string nameFile);

	void generateMedianGesture(std::vector<type_gesture> gestures);
	int  getMeanPoints(vector<type_gesture> gestures, int i, int j);
	void generateGestureEqualSize(std::vector<type_gesture>* gestures);
	
	void normCenterOriginGesture(type_gesture *gesture);

	/**
		- Normalizado
		- Centralizado na origem
		- Sem simplificação
		- Laplaciano
		- Todos Gestos
	*/
	void process1();
	
	/**
		- Normalizado
		- Centralizado na origem
		- Sem simplificação
		- B-Spline
		- Todos Gestos
	*/
	void process2();

	/**
		- Normalizado
		- Centralizado na origem
		- Sem simplificação
		- Laplaciano
		- Gesto Médio
	*/
	void process3();

	/**
		- Normalizado
		- Centralizado na origem
		- Sem simplificação
		- B-Spline
		- Gesto Médio
	*/
	void process4();

		/**
		- Normalizado
		- Centralizado na origem
		- Simplificação Curvatura
		- Laplaciano
		- Todos Gestos
	*/
	void process5();
	
	/**
		- Normalizado
		- Centralizado na origem
		- Simplificação Curvatura
		- B-Spline
		- Todos Gestos
	*/
	void process6();

	/**
		- Normalizado
		- Centralizado na origem
		- Simplificação Curvatura
		- Laplaciano
		- Gesto Médio
	*/
	void process7();

	/**
		- Normalizado
		- Centralizado na origem
		- Simplificação Curvatura
		- B-Spline
		- Gesto Médio
	*/
	void process8();

		/**
		- Normalizado
		- Centralizado na origem
		- Simplificação Douglas-Peuker
		- Laplaciano
		- Todos Gestos
	*/
	void process9();
	
	/**
		- Normalizado
		- Centralizado na origem
		- Simplificação Douglas-Peuker
		- B-Spline
		- Todos Gestos
	*/
	void process10();

	/**
		- Normalizado
		- Centralizado na origem
		- Simplificação Douglas-Peuker
		- Laplaciano
		- Gesto Médio
	*/
	void process11();

	/**
		- Normalizado
		- Centralizado na origem
		- Simplificação Douglas-Peuker
		- B-Spline
		- Gesto Médio
	*/
	void process12();

	void applyLaplacian(std::vector<type_gesture>* gestures);
	void applyBSpline(std::vector<type_gesture>* gestures);
	void applyCurvature(std::vector<type_gesture>* gestures);
	void applyDouglasPeucker(std::vector<type_gesture>* gestures);
	void applyProcess(int env);
	void applyMedian();
	
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
	
	float m_PercentTest, m_Param;
};

#endif //TEST_H__
