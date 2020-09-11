#pragma once
#include "automata.hpp"
#include "detAutomata.hpp"

class NondetAutomata : public Automata
{	
	friend class DetAutomata;
private:
	DetAutomata* detForm;

	void updateTable(Vector<int>**&, int&);
	void addState(int&, int&, Vector<int>**&, int&);
	void produceStates(State*&, Vector<int>**&, int);
	void renameTransitions(Vector<int>**&, int&);	

	void combineAutomatas(const DetAutomata&, const DetAutomata&);
public:
	NondetAutomata(const char*, State*, int, Vector<int>**);
	~NondetAutomata();
	void determine();
	DetAutomata* get_detForm();

	void save(const char*);
	bool recognize(const char*, const int currRow = 0, const int lettersRead = 0, const int consequentEpsilons = 0) const;
	bool isLangEmpty() const;
	void showTransitions() const;
	bool infinite() const;
	bool determined() const;

	//virtual void list() = 0;
};