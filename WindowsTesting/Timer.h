#pragma once
#include <chrono>

class Timer {
public:
	Timer();
	float mark();
	float peek();
private:
	std::chrono::time_point<std::chrono::system_clock> lastTime, now;
};
