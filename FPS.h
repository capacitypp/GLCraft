#ifndef ___Class_FPS
#define ___Class_FPS

#include "global.h"
#include "Timer.h"

class FPS {
	int cnt;
	double fps;
	Timer timer;
public:
	FPS(void) : cnt(0), fps(IDEAL_FPS) {}
	void update(void) {
		cnt++;
		if (cnt > IDEAL_FPS) {
			fps = (IDEAL_FPS) / timer.compute();
			cnt = 0;
		}
	}
	double get(void) const { return fps; }
};

#endif
