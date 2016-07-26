#ifndef TEST_H__
#define TEST_H__

#include "../../Commons/commons.hpp"
#include "gesture/gesture.h"

class Test {

public:
	Test();
	~Test();

	void loadAll();
	void clearAll();
	void splitDataset();
	void initializeGestureTemplates();

	void init();
	void execute();
	void saveResults();

	// Sort Container by name function
	static bool sortByName(const type_gesture g1, const type_gesture g2) { return g1.name < g2.name; }

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
	std::vector<type_gesture> m_GesturesTemplate;
	std::vector<type_gesture> m_GesturesTest;
	std::vector<type_gesture> m_AllGestures;

	float m_PercentTest;
};

#endif //TEST_H__
