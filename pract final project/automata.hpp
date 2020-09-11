#pragma once
#include "vector.hpp"
#include "string.hpp"
#include "stringstream.hpp"
#include "automataState.hpp"
#include <iostream>
#include <fstream>
#include <cassert>


class Automata
{
protected:
	State* states;
	Vector<int>** transition;
	int numStates;
	int abcLen;
	int id;
	char* alphabet;
public:
	Automata();

	void set_id(int);
	int get_numStates() const;
	int get_abcLen() const;
	int get_id() const;
	char* get_alphabet() const;
	State* get_States() const;

	virtual bool recognize(const char*, const int currRow = 0, const int lettersRead = 0, const int consequentEpsilons = 0) const = 0;
	virtual bool isLangEmpty() const = 0;
	virtual bool infinite() const  = 0;
	virtual void showTransitions() const = 0;
	virtual void save(const char*) = 0;	
	virtual bool determined() const = 0;
};
