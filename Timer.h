#ifndef ___Class_Timer
#define ___Class_Timer

#include <chrono>

class Timer {
	std::chrono::system_clock::time_point start;
public:
	Timer();
	double compute(void);
};

#endif
