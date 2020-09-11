#pragma once
#include "vector.hpp"
#include "string.hpp"
#include "stringstream.hpp"
#include "automataState.hpp"
#include <cassert>
#include "automata.hpp"
#include "nondetAutomata.hpp"


class DetAutomata : public Automata
{
	friend class NondetAutomata;
private:
	void copyAutomata(const DetAutomata& other);
	void deleteAutomata();
	
	bool checkLoop(Vector<int>, Vector<bool>, Vector<int>, const int&) const;
	bool checkSequence(Vector<int>&, Vector<bool>&) const;
	int findCharInAbc(char) const;
	bool hasLanguage(const int, Vector<bool>) const;

public:
	DetAutomata(const char* , State*, int, int** );	
	DetAutomata(const char*, int, State*, Vector<int>**);
	DetAutomata(const NondetAutomata&);

	DetAutomata(const DetAutomata&);
	DetAutomata& operator = (const DetAutomata&);
	~DetAutomata();

	bool recognize(const char*,const int currRow = 0,const int lettersRead = 0,const int consequentEpsilons = 0) const;
	bool isLangEmpty() const;
	bool infinite() const;
	bool determined() const;

	void save(const char*);
	static DetAutomata open(const char*);

	void showTransitions() const;

	DetAutomata concat(const DetAutomata&);
	DetAutomata unite(const DetAutomata&);

	
};