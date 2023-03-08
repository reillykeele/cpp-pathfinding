#pragma once

namespace SDLEngine
{

	class FPSLimiter
	{
	public:
		FPSLimiter();
		~FPSLimiter();

		void init(float maxFPS);

		void setMaxFPS(float maxFPS);

		/**
		 * \brief Called at the start of the game loop.		 
		 */
		void begin();

		/**
		 * \brief Called at the end of the game loop.
		 * \return The current FPS as a float.
		 */
		float end();

	private:

		void calculateFPS();

		float _fps;
		float _maxFPS;
		float _frameTime;

		unsigned int _startTicks;
	};


}
