#include "stringstream.hpp"
#include <cmath>
#include <cassert>

void Sstream::copy(const Sstream& other)
{
	numInts = other.numInts;
	numStrs = other.numStrs;
	ints = new int[numInts];
	strs = new String[numStrs];
	int max = (numInts > numStrs) ? numInts : numStrs;
	for (int i = 0; i < max; ++i)
	{
		if (i < numInts) ints[i] = other.ints[i];
		if (i < numStrs)strs[i] = other.strs[i];
	}
}

Sstream::Sstream() : numInts(0), numStrs(0), strs(nullptr), ints(nullptr) {}

Sstream::Sstream(const Sstream& other) : numInts(other.numInts), numStrs(other.numStrs)
{
	copy(other);
}

Sstream::Sstream(String& _str) : numStrs(1), numInts(0), ints(nullptr)
{
	strs = new String[1];
	strs[0] = _str;
}

Sstream::Sstream(const char* _str) : numStrs(1), numInts(0), ints(nullptr)
{
	String str(_str);
	strs = new String[1];
	strs[0] = _str;
}

Sstream::Sstream(int& _int) : numStrs(0), numInts(1), strs(nullptr)
{
	ints = new int[1];
	ints[0] = _int;
}

Sstream::Sstream(int _int) : numStrs(0), numInts(1), strs(nullptr)
{
	ints = new int[1];
	ints[0] = _int;
}

Sstream& Sstream::operator = (const Sstream& other)
{
	if (this != &other)
	{
		delete[] ints;
		delete[] strs;
		copy(other);
	}
	return *this;
}

Sstream& Sstream::operator << (int& num)
{
	int* newInts = new int[numInts + 1];
	for (int i = 0; i < numInts; ++i)
		newInts[i] = ints[i];
	newInts[numInts] = num;
	delete[] ints;
	ints = newInts;
	++numInts;
	return *this;
}



Sstream& Sstream::operator >> (int& num)
{
	num = ints[numInts - 1];
	--numInts;
	return *this;
}

Sstream& Sstream::operator << (String& _str)
{
	String* newStrs = new String[numStrs + 1];
	for (int i = 0; i < numStrs; ++i)
		newStrs[i] = strs[i];
	newStrs[numStrs] = _str;
	delete[] strs;
	strs = newStrs;
	++numStrs;
	return *this;
}

Sstream& Sstream::operator << (const char* _str)
{
	String str(_str);
	Sstream::operator<<(str);
	return *this;
}

Sstream& Sstream::operator >> (String& _str)
{
	_str = strs[numStrs - 1];
	--numStrs;
	return *this;
}

void Sstream::strToInt(int& _int)
{
	assert(numStrs > 0);
	_int = 0;
	int letters = strs[numStrs - 1].get_len();
	for (int i = 0; i < letters; ++i)
	{
		_int += ((strs[numStrs - 1])[letters - i - 1] - '0')*pow(10, i);
	}
}

void Sstream::intToStr(String& _str)
{
	int digits = log10(ints[numInts - 1]) + 1;
	assert(numInts > 0 && (_str.get_capacity() >= digits));
	_str.nullstring();
	if (ints[numInts - 1] == 0) { _str[0] = '0'; _str.set_length(1); }
	else
	{
		int foo;
		for (int i = 0; i < digits; ++i)
		{
			foo = ints[numInts - 1] / pow(10, i);
			_str[digits - i - 1] = foo % 10 + '0';
		}
		_str.set_length(digits);
	}	
	
}

void Sstream::visualizeStream()	
{
	std::cout << "Int values: ";
	for (int i = 0; i < numInts; ++i)
		std::cout << ints[i]<<" ";
	std::cout << std::endl << "String values: ";
	for (int i = 0; i < numStrs; ++i)
		std::cout << "\"" << strs[i] << "\" ";
	std::cout << std::endl;
}

Sstream::~Sstream()
{
	delete[] strs;
	delete[] ints;
}