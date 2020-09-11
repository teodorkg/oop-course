#include <iostream>
#include "vector.hpp"
#include "string.hpp"
#include "automata.hpp"
#include "nondetAutomata.hpp"
#include "detAutomata.hpp"
#include "automataState.hpp"

void menuStart(DetAutomata*& currAuto);
void menuLoaded(DetAutomata*& currAuto, char&);

void enterTransition(int numStates, int abcLen, Vector<int>**& tr);
void enterStates(int numStates, State*& st);

static int autoNum;


void enterStates(int numStates, State*& st)
{
	for (int i = 0; i < numStates; ++i)
	{
		String name;
		bool start, final;
		std::cout << "\nState " << i << "\n" << "name: ";
		std::cin.ignore();
		std::cin >> name;
		st[i].set_name(name);
		std::cout << "start: ";
		std::cin >> start;
		st[i].setStart(start);
		std::cout << "final: ";
		std::cin >> final;
		st[i].setFinal(final);
	}
}

void enterTransition(int numStates, int abcLen, Vector<int>**& tr)
{
	std::cin.ignore();
	String str;
	tr = new Vector<int>*[numStates];
	for (int i = 0; i < numStates; ++i)
	{
		tr[i] = new Vector<int>[abcLen + 1];
		for (int j = 0; j < abcLen + 1; ++j)
		{			
			std::cin >> str;
			tr[i][j] << str;
		}
	}
}

void menuStart(DetAutomata*& currAuto)
{
	char input;

	std::cout << "==============================================\n";
	std::cout << " (o)pen automata								\n";
	std::cout << " (e)nter nondet automata						\n";
	std::cout << " (q)uite 										\n\n"; 

	std::cin >> input;
	if (input == 'e' || input=='o') menuLoaded(currAuto, input);	
}

void menuLoaded(DetAutomata*& currAuto, char& input)
{
	do
	{
		switch (input)
		{
		case 'm':
			std::cout << "==============================================\n";
			std::cout << " (o)pen automata								\n";
			std::cout << " (e)nter nondet automata						\n\n";
			std::cout << " (i)d											\n";
			std::cout << " (u)nite automata								\n";
			std::cout << " (c)oncat automata							\n";
			std::cout << " (r)ecognize									\n";
			std::cout << " (s)how transitions							\n";
			std::cout << " (S)ave          								\n";
			std::cout << " (E)mpty?										\n";
			std::cout << " (I)nifite?									\n";
			std::cout << " (q)uite 										\n\n"; break;
		case 'o':
		{
			String toOpen;
			std::cout << "\nEnter file name: ";
			std::cin.ignore();
			std::cin >> toOpen;
			DetAutomata* a = new DetAutomata(DetAutomata::open(toOpen));
			delete currAuto;
			currAuto = a;
			if (autoNum <= currAuto->get_id()) autoNum = currAuto->get_id() + 1;
			std::cout << "Successfully loaded!\n";
			break;
		}
		case 'e':
		{
			Vector<int>** tr;
			int numStates;
			std::cout << "\nEnter number of states: ";
			std::cin >> numStates;
			char abc[32];
			std::cout << "Enter alphabet: ";
			std::cin >> abc;
			enterTransition(numStates, String::strlength(abc), tr);
			State* st = new State[numStates];
			enterStates(numStates, st);
			NondetAutomata* nondetA = new NondetAutomata(abc, st, numStates, tr);
			delete currAuto;
			currAuto = nondetA->get_detForm();
			currAuto->set_id(autoNum++);
			break;
		}
		case 'i':
		{
			std::cout << "Current automata id: " << currAuto->get_id() << '\n';
			break;
		}
		case 'u':
		{
			String toLoad;
			std::cout << "\nFile of second automata: ";
			std::cin.ignore();
			std::cin >> toLoad;
			DetAutomata a(DetAutomata::open(toLoad));
			DetAutomata* united = new DetAutomata(currAuto->unite(a));
			delete currAuto;
			currAuto = united;
			break;
		}
		case 'c':
		{
			String toLoad;
			std::cout << "\nFile of second automata: ";
			std::cin.ignore();
			std::cin >> toLoad;
			DetAutomata a(DetAutomata::open(toLoad));
			DetAutomata* united = new DetAutomata(currAuto->concat(a));
			delete currAuto;
			currAuto = united;
			break;
		}
		case 'r':
		{
			String word;
			std::cout << "\nWord to check: ";
			std::cin.ignore();
			std::cin >> word;
			if (currAuto->recognize(word))
				std::cout << "Yes, the word is recognized.\n";
			else std::cout << "No, the word isn't recognized.\n";
			break;
		}
		case 's':
		{
			currAuto->showTransitions();
			break;
		}
		case 'S':
		{
			String str;
			std::cout << "File name: ";
			std::cin.ignore();
			std::cin >> str;
			currAuto->save(str);
			break;
		}
		case 'E':
		{
			if (currAuto->isLangEmpty())
				std::cout << "\nThe automata doesn't have a language.\n";
			else std::cout << "\nThe automata has a language.\n";
			break;
		}
		case 'I':
		{
			if (currAuto->infinite())
				std::cout << "\nThe automata has an infinite language.\n";
			else
				std::cout << "\nThe automata has a finite language.\n";
			break;
		}
		case 'q': return;

		}
	}while (std::cin >> input);
}


