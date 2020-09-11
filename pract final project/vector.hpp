#pragma once
#include "string.hpp"
#include "stringstream.hpp"
#include <assert.h>
#include <iostream>

template<typename T>
class Vector
{
private:

	T* vect;
	int size;
	int capacity;

	void copyValues(const Vector<T> &other)
	{
		for (int i = 0; i < other.size; ++i)
			vect[i] = other.vect[i];
	}
	void copyVector(const Vector<T> &other)
	{
		capacity = other.capacity;
		size = other.size;
		vect = new T[capacity];
		copyValues(other);
	}
	void sizeUp()
	{
		Vector<T> v(capacity + 4);
		v.size = size;
		v.copyValues(*this);

		delete[] vect;
		vect = v.vect;
		capacity += 4;
		v.vect = nullptr;
	}

public:
	Vector() : size(0), capacity(1) { vect = new T[capacity]; }

	Vector(int _capacity) : size(0), capacity(_capacity) { vect = new T[capacity]; }

	Vector(const Vector<T>& other) { copyVector(other); }

	Vector& operator=(const Vector<T>& other)
	{
		if (this != &other)
		{
			delete[] vect;
			copyVector(other);
		}
		return *this;
	}

	T& operator[](int i)
	{
		assert(i <= capacity);
		return vect[i];
	}

	void operator+=(const Vector<T>& other)
	{
		capacity += other.capacity;
		Vector<T> v(capacity);
		v.copyValues(*this);
		delete[] vect;
		for (int i = size; i < size + other.size; ++i)
		{
			v[i] = other.vect[i - size];
		}
		size += other.size;
	}

	~Vector() { delete[] vect; }
	int get_size() const { return size; }
	int get_capacity() const { return capacity; }

	void addAt(int index, T newMem)
	{
		if (index > size)
		{
			std::cout << "Wrong index.\n";
			return;
		}
		if (size + 1 <= capacity)
		{
			for (int i = size - 1; i >= index; --i)
				vect[i + 1] = vect[i];
			vect[index] = newMem;
			++size;
		}
		else
		{
			sizeUp();
			addAt(index, newMem);
		}
	}
	void removeAt(int index)
	{
		assert(index >= 0);
		if (index < size - 1)
		{
			for (int i = index + 1; i < size; ++i)
				vect[i - 1] = vect[i];

		}
		--size;
	}

	void add(T newMem) { addAt(size, newMem); }

	void printVector()
	{
		for (int i = 0; i < size; ++i)
			std::cout << vect[i] << " ";
		std::cout << "\nsize: " << size << "\ncapacity: " << capacity;
		std::cout << "\n";
	}

	void printValues()
	{
		for (int i = 0; i < size; ++i)
		{
			if (i > 0) std::cout << ',';
			std::cout << vect[i];
		}
	}
	Vector& operator << (T newMem)
	{
		add(newMem);
		return *this;
	}
	Vector& operator << (Vector<T>& addVec)
	{
		int len = addVec.get_size();
		for (int i = 0; i < len; ++i)
			(*this) << (addVec[i]);
		return *this;
	}
	Vector& operator << (String& strToVec)
	{
		Sstream stream;
		String buff;
		bool negative = false;
		int len = strToVec.get_len();
		for (int i = 0; i <= len; ++i)
		{
			if (i == len || strToVec[i] == ',')
			{
				int a;				
				stream << buff;
				stream.strToInt(a);
				if (negative) a *= -1;
				(*this) << a;

				buff.nullstring();
				negative = false;
			}
			else if (strToVec[i] == '-')
				negative = true;
			else buff += strToVec[i];
		}

		return *this;
	}

	Vector& operator >> (int &b)
	{
		size--;
		b = vect[size];
		return *this;
	}

	bool operator == (Vector& other)		//Compares them as a set!!!
	{
		for (int i = 0; i < other.size; ++i)
			if (!contains(other.vect[i]))
				return false;
		for (int i = 0; i < size; ++i)
			if (!other.contains(vect[i]))
				return false;
		return true;
	}

	bool contains(T checkMem)
	{
		for (int i = 0; i < size; ++i)
			if (vect[i] == checkMem)
				return true;
		return false;
	}

	Vector& empty() { size = 0; return *this; }
};