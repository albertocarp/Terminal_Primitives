#pragma once
#include "TestCase.h"
template<typename Test>
class Benchmark
{
	Test test;
public:
	Benchmark();
	void registerTest(Test & test);
	~Benchmark();
};

template<typename T>
Benchmark<T>::Benchmark()
{
}

template<typename Test>
void Benchmark<Test>::registerTest(Test & test)
{
}


template<typename T>
Benchmark<T>::~Benchmark()
{
}
