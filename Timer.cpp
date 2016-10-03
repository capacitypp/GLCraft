#include <iostream>
#include <chrono>

#include "Timer.h"

using namespace std;

Timer::Timer()
{
	start = chrono::system_clock::now();
}

double Timer::compute(void)
{
	chrono::system_clock::time_point now = chrono::system_clock::now();
	auto dur = now - start;
	double msec = chrono::duration_cast<chrono::milliseconds>(dur).count() / 1000.0;
	start = now;
	return msec;
}