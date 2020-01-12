#pragma once
#include "Algorithm.h"

class GeneticAlg :
	public Algorithm
{
private:
	int liczbaPokolen;
	int prawdopodobienstwoKrzyzowania;
	int prawdopodobienstwoMutacji;
	bool OX_b = true;

	void krzyzowanie();
	void OX();  // ordered
	void PMX();  // czesciowo ordered
	void mutacje();		// mutacje przez inwersje
	void usunNajgorsze();		// nadmiar umiera (wszystko > wielkoscPopulacji)

	vector<Osobnik> losujRodzicow();
public:
	GeneticAlg() :Algorithm() {
		liczbaPokolen = 50;
		prawdopodobienstwoKrzyzowania = 20;
		prawdopodobienstwoMutacji = 20;
	}
	~GeneticAlg() = default;

	void DisplayProperties() override;
	void run() override;

	void set_liczbaPokolen(int liczbaPokolen);
};

