#include "Timer.h"
#include <chrono>

Timer::Timer() {
	lastTime = std::chrono::system_clock::now();
}

float Timer::mark() {
	now = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedSeconds = now - lastTime;
	lastTime = now;

	return elapsedSeconds.count();
}


float Timer::peek() {
	now = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedSeconds = now - lastTime;

	return elapsedSeconds.count();
}


