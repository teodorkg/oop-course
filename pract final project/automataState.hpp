#pragma once
#include "vector.hpp"
#include "string.hpp"
#include "stringstream.hpp"

class State
{
private:
	String name;
	bool startState;
	bool finalState;

	void copyState(const State& other)
	{
		startState = other.startState;
		finalState = other.finalState;
		name = other.name;
	}

public:

	State(Vector<int>&, bool, bool);
	State(String _name = "q", bool _start = 0, bool _final = 0);
	State(const State&);

	State& operator =(const State&);

	Vector<int>* nameToVector();
	void set_name(String);
	void setStart(bool);
	void setFinal(bool);
	void set_name(Vector<int>&);

	int numInName();
	String get_name() const; 
	bool isFinal() const;
	bool isStart() const;

};
