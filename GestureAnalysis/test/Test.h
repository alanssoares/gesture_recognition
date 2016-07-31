#ifndef TEST_H__
#define TEST_H__

#include "../../Commons/commons.hpp"

class Test {

public:
	Test();
	~Test();

	// Sort Container by name function
	static bool sortByName(const type_gesture g1, const type_gesture g2) { return g1.name < g2.name; }

	void loadAll();
	void clearAll();
	void splitDataset();
	void initializeGestureTemplates();

	void init();
	void execute();
	void saveResults();

	void recognizeOneHand(const type_gesture gesture);
	void recognizeTwoHands(const type_gesture gesture);

	void generateMedianGestures();
	int getMeanPoints(vector<type_gesture> gestures, int i, int j);
	type_gesture resampleGesture(int k, int p);
	std::vector<XnPoint3D> inserePositions(std::vector<XnPoint3D> points, int diff);
	std::vector<XnPoint3D> removePositions(std::vector<XnPoint3D> points, int diff);
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
