#include "detAutomata.hpp"
#include <iostream>


DetAutomata::DetAutomata(const char* _alphabet, State* _states, int _numStates, int** _transition)
{
	abcLen = String::strlength(_alphabet);
	if (abcLen > 0)
	{
		alphabet = new char[abcLen + 1];
		for (int i = 0; i < abcLen; ++i)
			alphabet[i] = _alphabet[i];
		alphabet[abcLen] = '\0';
	}
	
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
}

DetAutomata::DetAutomata(const char* _abc, int _numStates, State* _states, Vector<int>** _transition)
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
				transition[0][j] << _transition[i][j][0];

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
}

DetAutomata::DetAutomata(const NondetAutomata& nondet)
{
	DetAutomata(*(nondet.detForm));
}

DetAutomata::DetAutomata(const DetAutomata& other) 
{ 
	copyAutomata(other); 
}

DetAutomata& DetAutomata::operator = (const DetAutomata& other)
{
	if (this != &other)
	{
		deleteAutomata();
		copyAutomata(other);
	}
	return *this;
}

DetAutomata::~DetAutomata() { deleteAutomata(); }

bool DetAutomata::recognize(const char* word, const int currRow, const int lettersRead, const int consequentEpsilons) const
{
	//std::cout << states[currRow].get_name() << std::endl;
	if (consequentEpsilons > numStates)
		return false;

	if (String::strlength(word) == lettersRead)
	{
		if (transition[currRow][abcLen + 1][0] >= 2 )
			return true;
		else if (transition[currRow][abcLen][0] >= 0 && transition[currRow][abcLen][0] != currRow)
			return recognize(word, transition[currRow][abcLen][0], lettersRead, consequentEpsilons + 1);
		else return false;
	}
	
	int currCol = findCharInAbc(word[lettersRead]);
	if (currCol != -1)
	{
		if (transition[currRow][currCol][0] >= 0 && recognize(word, transition[currRow][currCol][0], lettersRead + 1, 0))
			return true;
		if (transition[currRow][abcLen][0] >= 0 && transition[currRow][abcLen][0] != currRow)
			return recognize(word, transition[currRow][abcLen][0], lettersRead, consequentEpsilons + 1);
	}
	return false;
}

int DetAutomata::findCharInAbc(char a) const
{
	for (int i = 0; i < abcLen; ++i)
		if (alphabet[i] == a) return i;
	return -1;
}

void DetAutomata::showTransitions() const
{
	std::cout << "            ";
	for (int i = 0; i < abcLen; ++i)
	{
		std::cout << alphabet[i] <<" ";
	}
	std::cout << "$ " << "\n";
	for (int i = 0; i < numStates; ++i)
	{
		int len = states[i].get_name().get_len();
		std::cout << states[i].get_name();
		if (states[i].isFinal())
		{
			std::cout << " F";
			len += 2;
		}
		for (int j = len; j < 10; ++j)
			std::cout << " ";
		std::cout << "| ";
		for (int j = 0; j < abcLen + 1; ++j)
		{
			for (int k = 0; k < transition[i][j].get_size(); ++k)
			{
				if (k > 0) std::cout << ',';
				std::cout << transition[i][j][k];
			}
			std::cout << ' ';									
		}
		std::cout << std::endl;
	}
}

bool DetAutomata::infinite() const
{
	Vector<int> sequence;
	sequence << 0;
	Vector<bool> haveReadSmth;
	Vector<int> stateCounter(numStates);
	for (int i = 0; i < numStates; ++i) stateCounter << 0;
	return checkLoop(sequence, haveReadSmth, stateCounter, 0);
}


bool DetAutomata::checkLoop(Vector<int> sequence, Vector<bool> haveReadSmth, Vector<int> stateCounter, const int& row) const
{
	//sequence.printVector();
	//haveReadSmth.printVector();
	//std::cout << row << std::endl;
	if (transition[row][abcLen + 1][0] >= 2 && checkSequence(sequence, haveReadSmth))
		return true;
	for (int i = 0; i <= abcLen; ++i)
	{
		const int rowValueAtI = transition[row][i][0];
		if (rowValueAtI >= 0 && stateCounter[rowValueAtI] < 2)
		{
			//std::cout << row << alphabet[i] << std::endl;
			Vector<int> stateCounterCopy(stateCounter);
			stateCounterCopy[rowValueAtI] ++;

			Vector<int> sequenceCopy(sequence);
			sequenceCopy << rowValueAtI;

			Vector<bool> readSmth(haveReadSmth);
			if (i != abcLen) readSmth << 1;
			else readSmth << 0;

			if (checkLoop(sequenceCopy, readSmth, stateCounterCopy, transition[row][i][0]))
				return true;
		}
	}
	return false;
}

bool DetAutomata::checkSequence(Vector<int>& sequence, Vector<bool>& haveReadSmth) const
{
	int end = 0;
	bool metFinal;
	bool readSmth;
	end = sequence.get_size();
	for (int j = 0; j < end; ++j)
	{
		metFinal = false;
		readSmth = false;
		for (int k = j + 1; k < end; ++k)
		{
			const int endSeq = sequence.get_size();
			for (int i = k; i < endSeq; ++i)
				if (transition[sequence[i]][abcLen + 1][0] >= 2)
					metFinal = true;
			if (haveReadSmth[k - 1] == true)
				readSmth = true;
			if (sequence[j] == sequence[k] && metFinal && readSmth)
				return true;
		}
	}
	
	return false;
}

bool DetAutomata::isLangEmpty() const
{
	Vector<bool> stateVisited(numStates);
	for (int i = 0; i < numStates; ++i)
		stateVisited << false;
	return !(hasLanguage(0, stateVisited));
}

bool DetAutomata::hasLanguage(const int row, Vector<bool> stateVisited) const
{
	Vector<bool> copyStateVis(stateVisited);
	copyStateVis[row] = true;

	if (transition[row][abcLen + 1][0] >= 2)
		return true;

	for (int i = 0; i <= abcLen; ++i)
	{
		
		if (transition[row][i][0] >= 0 &&
			copyStateVis[transition[row][i][0]] == 0 &&
			hasLanguage(transition[row][i][0], copyStateVis))
				return true;
	}
	return false;
}


DetAutomata DetAutomata::concat(const DetAutomata& other)
{
	NondetAutomata result(nullptr,nullptr, 0, nullptr);
	result.combineAutomatas(*this, other);

	int startStatesCount = 0;
	int currStateIndex;
	for (int i = 0; i < result.numStates; ++i)
	{
		currStateIndex = result.transition[i][result.abcLen + 1][0];
		if (currStateIndex == 0 || currStateIndex == 3)
			startStatesCount++;

		if (startStatesCount >= 2)											//Converts second automata start states to normal or final
		{
			if (currStateIndex == 0)
				result.transition[i][result.abcLen + 1][0] = 1;
			else if(currStateIndex == 3)
				result.transition[i][result.abcLen + 1][0] = 2;
		}

		if (currStateIndex >= 2 && startStatesCount < 2)					//Adds Epsilon transitions from the First's finals to the Second's start
		{
			for (int j = i + 1; j < result.numStates; ++j)
			{
				currStateIndex = result.transition[j][result.abcLen + 1][0];
				if (currStateIndex == 0 || currStateIndex == 3)
				{
					if (result.transition[i][result.abcLen][0] == -1)
						result.transition[i][result.abcLen][0] = j;
					else 
						result.transition[i][result.abcLen] << j;
				}
			}
		}	
	}

	State* newStates = new State[result.numStates];
	String name;
	Sstream toName;
	newStates[0].setStart(true);

	for (int i = 0; i < result.numStates; ++i)
	{
		toName << i;
		toName.intToStr(name);
		newStates[i].set_name(name);
		newStates[i].setFinal(result.transition[i][result.abcLen + 1][0] >= 2);
		name.nullstring();
	}
	result.states = newStates;

	result.determine();
	return (*result.detForm);
}

DetAutomata DetAutomata::unite(const DetAutomata& other)
{
	NondetAutomata result(nullptr, nullptr, 0, nullptr);
	result.combineAutomatas(*this, other);

	int startStatesCount = 0;
	int currStateIndex;
	
			
																			//Adds Epsilon transitions from the This start to the Other start
	int startEpsilonCell = result.transition[0][result.abcLen][0];
	if(startEpsilonCell==-1) result.transition[0][result.abcLen][0] = this->numStates;
	else result.transition[0][result.abcLen] << this->numStates;
	startStatesCount++;
	for (int i = 1; i < result.numStates; ++i)
	{
		currStateIndex = result.transition[i][result.abcLen + 1][0];
		if (currStateIndex == 0 || currStateIndex == 3)
			startStatesCount++;

		if (startStatesCount >= 2)											//Converts Other automata start states to normal or final
		{
			if (currStateIndex == 0)
				result.transition[i][result.abcLen + 1][0] = 1;
			else if (currStateIndex == 3)
				result.transition[i][result.abcLen + 1][0] = 2;
		}
	}

	State* newStates = new State[result.numStates];
	String name;
	Sstream toName;
	newStates[0].setStart(true);

	for (int i = 0; i < result.numStates; ++i)
	{
		toName << i;
		toName.intToStr(name);
		newStates[i].set_name(name);
		newStates[i].setFinal(result.transition[i][result.abcLen + 1][0] >= 2);
		name.nullstring();
	}
	result.states = newStates;


	result.determine();

	return (*result.detForm);
}


void DetAutomata::copyAutomata(const DetAutomata& other)
{
	id = other.id;

	abcLen = other.abcLen;
	alphabet = new char[abcLen + 1];
	for (int i = 0; i < abcLen; ++i)
		alphabet[i] = other.alphabet[i];
	alphabet[abcLen] = 0;

	numStates = other.numStates;
	transition = new Vector<int>*[numStates];
	states = new State[numStates];
	int k = 0;
	for (int i = 0; i < numStates; ++i)
	{
		states[i] = other.states[i];
		transition[i] = new Vector<int>[abcLen + 2];
		for (int j = 0; j < abcLen + 2; ++j)
			transition[i][j] = other.transition[i][j];
	}
}

void DetAutomata::save(const char* name)
{
	std::ofstream os(name, std::ios::trunc);
	os << "id:" << id << "\n";
	os << "num states:" << numStates << "\n";
	os << alphabet << "\n";
	for (int i = 0; i < numStates; ++i)
	{
		os << states[i].get_name() << " ";
		for (int j = 0; j <= abcLen + 1; ++j)
		{
			if (j > 0)os << " ";
			os << transition[i][j][0];
		}
		os << "\n";
	}
	os.close();
}
DetAutomata DetAutomata::open(const char* file)
{
	int id, numStates, abcLen;
	char abc[16];
	std::ifstream is(file);
	is.seekg(3);

	is >> id;
	is.ignore();

	is.seekg(11 + is.tellg());
	is >> numStates;
	is.ignore();

	is >> abc;
	abcLen = String::strlength(abc);
	is.ignore();

	State* stts = new State[numStates];
	char sttName[16];
	int** transition = new int*[numStates];
	for (int i = 0; i < numStates; ++i)
	{
		is >> sttName;
		stts[i].set_name(sttName); 

		transition[i] = new int[abcLen + 2];
		for (int j = 0; j < abcLen + 2; ++j)
		{
			is.ignore();
			is >> transition[i][j];
		}
		is.ignore();

		int ind = transition[i][abcLen + 1];
		if (ind >= 2) stts[i].setFinal(true);
		if (ind == 0 || ind == 3) stts[i].setStart(true);
	}
	is.close();
	DetAutomata a(abc, stts, numStates, transition);
	a.set_id(id);
	return a;
}

bool DetAutomata::determined() const { return true; }

void DetAutomata::deleteAutomata()
{
	delete[] alphabet;
	delete[] states;

	for (int i = 0; i < numStates; ++i)
	{
		if (transition[i] != 0)
			delete[] transition[i];
	}
	delete[] transition;

}

