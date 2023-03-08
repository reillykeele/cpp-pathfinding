#include "Timing.h"

#include <algorithm>
#include <SDL/SDL_timer.h>

namespace SDLEngine
{

	FPSLimiter::FPSLimiter()
	{
	}

	FPSLimiter::~FPSLimiter()
	{
	}

	void FPSLimiter::init(float maxFPS)
	{
		setMaxFPS(maxFPS);
	}

	void FPSLimiter::setMaxFPS(float maxFPS) { _maxFPS = maxFPS; }

	void FPSLimiter::begin()
	{
		_startTicks = SDL_GetTicks64();
	}

	float FPSLimiter::end()
	{
		calculateFPS();

		float frameTicks = SDL_GetTicks64() - _startTicks;
		if (1000.0f / _maxFPS > frameTicks)
		{
			SDL_Delay(1000.0f / _maxFPS - frameTicks);
		}

		return _fps;;
	}

	void FPSLimiter::calculateFPS()
	{
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currFrame = 0;

		static float prevTicks = SDL_GetTicks64();

		float currTicks;
		currTicks = SDL_GetTicks64();

		_frameTime = currTicks - prevTicks;
		frameTimes[currFrame % NUM_SAMPLES] = _frameTime;

		prevTicks = currTicks;

		currFrame++;
		int count = std::min(currFrame, NUM_SAMPLES);

		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++)
			frameTimeAverage += frameTimes[i];

		frameTimeAverage /= count;

		if (frameTimeAverage > 0)
			_fps = 1000.0f / frameTimeAverage;
		else
			_fps = 60.0f;
	}
}


