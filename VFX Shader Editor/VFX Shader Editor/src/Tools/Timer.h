#pragma once
#include "Globals.h"
#include "SDL/include/SDL.h"
class Timer
{
public:

	Timer();

	void Start();
	void Stop();

	void PauseTimer();

	Uint32 ReadTime();

private:
	bool running;
	bool isPaused = false;

	Uint32 started_at;
	Uint32 stopped_at;
};