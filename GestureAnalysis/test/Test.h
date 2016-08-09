#ifndef TEST_H__
#define TEST_H__

#include "../../Commons/commons.hpp"

class Test {

public:
	Test();
	~Test();

	// Sort Container by name function
	static bool sortByName(const type_gesture g1, const type_gesture g2) { return g1.name < g2.name; }

	void printGesture(const type_gesture gesture);
	
	void loadAll();
	void clearAll();
	void splitDataset();
	void initializeGestureTemplates();

	void init();
	void execute();
	void saveResults();

	void recognizeOneHand(const type_gesture gesture);
	void recognizeTwoHands(const type_gesture gesture);

	void generateMedianGesture();
	void generateGestureEqualSize();
	int  getMeanPoints(vector<type_gesture> gestures, int i, int j);
	type_gesture getMeanGesture(const int k, const int p);
	
	/**
		Pré-requisitos:
		- Normalizado
		- Centralizado na origem
		Suavização:
		- Laplaciano, B-Spline
		Sem simplificação:
		Comparação:
		- Gesto Médio, Todos
	*/
	void experiment1();

	/**
		Pré-requisitos:
		- Normalizado
		- Centralizado na origem
		Suavização:
		- Laplaciano, B-Spline
		Com simplificação:
		- Curvatura, Douglas-Peuker
		Comparação:
		- Gesto Médio, Todos
	*/
	void experiment2();

	void improveGestures();

private:
	std::vector<type_gesture> m_MedianGestures;
	std::vector<type_gesture> m_GesturesTemplate;
	std::vector<type_gesture> m_GesturesTest;
	std::vector<type_gesture> m_AllGestures;
    
	std::string m_NameGestureRecognized;
	std::vector<type_gesture> m_GesturesFromFileOneHand;
	std::vector<type_gesture> m_GesturesFromFileTwoHands;
	
	float m_PercentTest;
};

#endif //TEST_H__
