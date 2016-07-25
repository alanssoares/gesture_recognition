#ifndef TEST_H__
#define TEST_H__

#include "../../Commons/commons.hpp"

class Test {

public:
	Test(){};
	~Test(){};

	void init();
	void execute();
	void saveResults();

	// Sort Container by name function
	bool sortByName(const type_gesture g1, const type_gesture g2) { return g1.name < g2.name; }

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

private:
	std::vector<type_gesture> m_GesturesTemplate;
	std::vector<type_gesture> m_GesturesTest;
	std::vector<type_gesture> m_AllGestures;
	
};

#endif //TEST_H__
