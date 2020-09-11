#include "automataState.hpp"

State::State(String _name, bool _start, bool _final)
	: name(_name), startState(_start), finalState(_final) {}
State::State(const State& other) { copyState(other); }

State& State::operator = (const State& other)
{
	if (this != &other)
	{
		copyState(other);
	}
	return *this;
}

State::State(Vector<int>& vec, bool st, bool fin) : startState(st), finalState(fin), name("{")
{
	Sstream stream;
	String fromInt;
	fromInt.nullstring();
	for (int i = 0; i < vec.get_size(); ++i)
	{
		stream << vec[i];
		stream.intToStr(fromInt);
		name += fromInt;
		name+= ',';
	}
	name += '}';
}

Vector<int>* State::nameToVector()
{
	Vector<int>* vect = new Vector<int>;
	int len = name.get_len();
	Sstream stream;
	String str;
	for (int i = 0; i < len; ++i)
	{
		if ('0' <= name[i] && name[i] <= '9')
			str += name[i];
		if (name[i] == ',' || name[i] == '}')
		{
			int a;
			stream << str;
			stream.strToInt(a);
			(*vect) << a;
			str.nullstring();
		}
	}
	return vect;
}

int State::numInName()
{
	int counter = 0;
	for (int i = 0; i < name.get_len(); ++i)
	{
		if (name[i] = ',') ++counter;
	}
	return counter + 1;
}
void State::set_name(Vector<int>& vect)
{
	name = "{";
	Sstream stream;
	String str;
	for (int i = 0; i < vect.get_size(); ++i)
	{
			stream << vect[i];
			stream.intToStr(str);
			if (i > 0)
				name += ',';
			name += str;
	}
	name += '}';
}
void State::set_name(String str) { name = str; }
void State::setFinal(bool fin) { finalState = fin; }
void State::setStart(bool start) { startState = start; }

String State::get_name() const { return name; }
bool State::isFinal() const { return finalState; }
bool State::isStart() const { return startState; }