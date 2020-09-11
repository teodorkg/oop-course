#ifndef STRINGSTREAM_HPP
#define STRINGSTREAM_HPP
#include <iostream>
#include "string.hpp"

class Sstream
{
private:
	int numInts;
	int numStrs;
	String* strs;
	int* ints;
	void copy(const Sstream&);
public:
	Sstream();
	Sstream(const Sstream&);
	Sstream(String&);
	Sstream(const char*);
	Sstream(int&);
	Sstream(int);
	Sstream& operator =(const Sstream&);
	Sstream& operator << (int&);
	Sstream& operator >> (int&);
	Sstream& operator << (String&);
	Sstream& operator << (const char*);
	Sstream& operator >> (String&);
	void strToInt(int&);
	void intToStr(String&);	
	void visualizeStream();
	~Sstream();
};
#endif