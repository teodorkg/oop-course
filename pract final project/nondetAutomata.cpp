#include "nondetAutomata.hpp"

NondetAutomata::NondetAutomata(const char* _abc, State* _states, int _numStates, Vector<int>** _transition)  
{
	abcLen = String::strlength(_abc);
	if (abcLen > 0)
	{
		alphabet = new char[abcLen + 1];
		for (int i = 0; i < abcLen; ++i)
			alphabet[i] = _abc[i];
		alphabet[abcLen] = '\0';
	}
	else alphabet = nullptr;

	numStates = _numStates;
	transition = new Vector<int>*[numStates];
	states = new State[numStates];
	int k = 0, ind = 0;
	for (int i = 0; i < numStates; ++i)
	{
		if (_states[i].isStart() && !k)
		{
			k = 1;
			states[0] = _states[i];
			transition[0] = new Vector<int>[abcLen + 2];
			for (int j = 0; j < abcLen + 1; ++j)
				transition[0][j] << _transition[i][j];

			bool isFinal = states[i].isFinal();
			if (isFinal)	transition[0][abcLen + 1] << 3;
			else			transition[0][abcLen + 1] << 0;
		}
		else
		{
			ind = i + 1 - k;
			transition[ind] = new Vector<int>[abcLen + 2];
			states[ind] = _states[i];
			for (int j = 0; j < abcLen + 1; ++j)
				transition[ind][j] << _transition[i][j];

			bool isFinal = states[ind].isFinal();
			bool isStart = states[ind].isStart();

			if (isFinal && isStart) transition[ind][abcLen + 1] << 3;
			else if (isStart)		transition[ind][abcLen + 1] << 0;
			else if (isFinal)		transition[ind][abcLen + 1] << 2;
			else					transition[ind][abcLen + 1] << 1;
		}
	}

	if (numStates >= 1) determine();
}



void NondetAutomata::determine()
{
	DetAutomata a(nullptr, nullptr, 0, nullptr);
	int newNumOfStates = 1;

	Vector<int>** newTransition = new Vector<int>*;
	newTransition[0] = new Vector<int>[abcLen + 2];
	newTransition[0][abcLen + 1] << 0;						//newTransition[i][abcLen + 1] is vector containing the old state/s that this state represents

	updateTable(newTransition, newNumOfStates);
	
	State* newStates = nullptr;

	produceStates(newStates, newTransition, newNumOfStates);
	renameTransitions(newTransition, newNumOfStates);

	int** tr = new int*[newNumOfStates];
	for (int i = 0; i < newNumOfStates; ++i)
	{
		tr[i] = new int[abcLen + 1];
		for (int j = 0; j <= abcLen; ++j)
			tr[i][j] = newTransition[i][j][0];
	}

	detForm = new DetAutomata(alphabet, newStates, newNumOfStates, tr);	

	for (int i = 0; i < newNumOfStates; i++)
	{
		delete[] tr[i];
		delete[] newTransition[i];
	}
	delete[] newStates;
	delete[] tr;
	delete[] newTransition;
}	

void NondetAutomata::updateTable(Vector<int>**& newTransition, int& newNumOfStates)
{
	bool newState = false;
	for (int i = 0; i < newNumOfStates; ++i)
	{
		int len = newTransition[i][abcLen + 1].get_size();
		for (int j = 0; j < len; ++j)
		{
			int row = newTransition[i][abcLen + 1][j];
			for (int k = 0; k <= abcLen; ++k)
			{
				if (transition[row][k][0] != -1)
					newTransition[i][k] << transition[row][k];
			}			
		}

		for (int k = 0; k <= abcLen; ++k)
			if (newTransition[i][k].get_size() == 0)
				newTransition[i][k] << -1;

		for (int j = 0; j <= abcLen && !newState; ++j)
		{
			newState = true;
			for (int k = 0; k < newNumOfStates; ++k)
				if (newTransition[i][j][0]==-1 || newTransition[i][j] == newTransition[k][abcLen + 1])
					newState = false;
			if (newState)
			{
				addState(i, j, newTransition, newNumOfStates);
				newState = false;
			}
		}
	}
}

void NondetAutomata::addState(int& i, int& j, Vector<int>**& newTransition, int& newNumOfStates)
{
	Vector<int>** expandTransition = new Vector<int>*[newNumOfStates + 1];
	for (int k = 0; k < newNumOfStates + 1; ++k)
	{
		expandTransition[k] = new Vector<int>[abcLen + 2];
		if (k != newNumOfStates)
			for (int l = 0; l <= abcLen + 1; ++l)
				expandTransition[k][l] = newTransition[k][l];
	}
	expandTransition[newNumOfStates][abcLen + 1] = newTransition[i][j];
	for (int k = 0; k < newNumOfStates; ++k)
		delete[] newTransition[k];
	delete[] newTransition;
	newTransition = expandTransition;
	++newNumOfStates;
}

void NondetAutomata::produceStates(State*& newStates,Vector<int>**& newTransition, int newNumOfStates)
{
	newStates = new State[newNumOfStates];
	newStates[0].setStart(true);
	for (int i = 0; i < newNumOfStates; ++i)
	{
		newStates[i].set_name(newTransition[i][abcLen + 1]);
		int len = newTransition[i][abcLen + 1].get_size();
		for (int k = 0; k < len; ++k)
		{
			int row = newTransition[i][abcLen + 1][k];
			if (transition[row][abcLen + 1][0] >= 2)
				newStates[i].setFinal(true);
		}
	}
}

void NondetAutomata::renameTransitions(Vector<int>**& newTransition, int& newNumOfStates)
{
	for (int i = 0; i < newNumOfStates; ++i)
	{
		for (int j = 0; j <= abcLen; ++j)
		{
			for (int k = 0; k < newNumOfStates; ++k)
			{
				if (newTransition[i][j] == newTransition[k][abcLen + 1])
				{
					(newTransition[i][j].empty()) << k;
					break;
				}
			}
		}
	}
}

void NondetAutomata::combineAutomatas(const DetAutomata& first, const DetAutomata& second)
{
	numStates = first.numStates + second.numStates;
	transition = new Vector<int>*[numStates];
	String newAbc;
	for (int i = 0; i < first.abcLen; ++i)
		newAbc += first.alphabet[i];
	for (int i = 0; i < second.abcLen; ++i)
		if (!newAbc.contains(second.alphabet[i]))
			newAbc += second.alphabet[i];

	abcLen = newAbc.get_len();
	alphabet = newAbc;
	for (int i = 0; i < first.numStates; ++i)
	{
		transition[i] = new Vector<int>[abcLen + 2];
		for (int j = 0; j < abcLen; ++j)
		{
			if (j < first.abcLen) transition[i][j] << first.transition[i][j][0];
			else transition[i][j] << -1;
		}
		transition[i][abcLen] << first.transition[i][first.abcLen];
		transition[i][abcLen + 1] << first.transition[i][first.abcLen + 1];
	}
	for (int i = 0; i < second.numStates; ++i)
	{
		transition[first.numStates + i] = new Vector<int>[abcLen + 2];
		for (int j = 0; j < abcLen; ++j)
		{
			int index = String::findChar(alphabet[j], second.alphabet);
			if (index >= 0 && second.transition[i][index][0] != -1)
				transition[first.numStates + i][j] << (second.transition[i][index][0] + first.numStates);
			else 
				transition[first.numStates + i][j] << -1;
		}
		if (second.transition[i][second.abcLen][0] != -1)
			transition[first.numStates + i][abcLen] << (second.transition[i][second.abcLen][0] + first.numStates);
		else transition[first.numStates + i][abcLen] << -1;
		transition[first.numStates + i][abcLen + 1] << second.transition[i][second.abcLen + 1];
	}
}

NondetAutomata::~NondetAutomata()
{
	delete[] alphabet;
	delete[] states;

	for (int i = 0; i < numStates; ++i)
	{
		if (transition[i] != 0)
			delete[] transition[i];
	}
	delete[] transition;
	delete detForm;
}

bool NondetAutomata::isLangEmpty() const { return detForm->isLangEmpty(); }
bool NondetAutomata::recognize(const char* str, const int currRow, const int lettersRead, const int consequentEpsilons) const
{ return detForm->recognize(str); }
void NondetAutomata::showTransitions() const { detForm->showTransitions(); }
bool NondetAutomata::infinite() const { return detForm->infinite(); }
bool NondetAutomata::determined() const 
{
	for (int i = 0; i < numStates; ++i)
		for (int j = 0; j <= abcLen; ++j)
			if (transition[i][j].get_size() > 1)
				return false;
	return true;
}


void NondetAutomata::save(const char* str) { detForm->save(str); }
DetAutomata* NondetAutomata::get_detForm() { DetAutomata* a = new DetAutomata(*detForm); return a; }