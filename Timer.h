#pragma once
#include "Config.h"

class Timer {
private:
	LONGLONG countsPerSecond;
	double secondsByCount;

	LONGLONG prevTime;
	LONGLONG currTime;
	LONGLONG deltaTime;

	LONGLONG deltaStopTime = 0;
public:
	Timer();
	void Reset();
	double Tick();//time in seconds
	void StopTimer();
	void RestartTimer();
	LONGLONG GetCurrentCount();
	double operator-(LONGLONG lastTime);
};