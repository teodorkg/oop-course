#include "automata.hpp"

Automata::Automata() : states(nullptr), transition(nullptr), alphabet(nullptr), abcLen(0), id(0), numStates(0) {}

int Automata::get_numStates() const { return numStates; }
int Automata::get_abcLen() const { return abcLen; }
char* Automata::get_alphabet()  const
{
	char* toReturn = new char[abcLen + 1];
	String::strcopy(toReturn, alphabet);
	return toReturn;
}
State* Automata::get_States() const
{
	State* toReturn = new State[numStates];
	for (int i = 0; i < numStates; ++i)
	{
		toReturn[i] = states[i];
	}
	return toReturn;
}
int Automata::get_id()  const { return id; }

void Automata::set_id(int _id) { id = _id; }