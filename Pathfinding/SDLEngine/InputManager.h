#pragma once
#include <unordered_map>
#include <GLM/vec2.hpp>

namespace SDLEngine
{


	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		bool isKeyPressed(unsigned int keyID);

		glm::vec2 getMouseCoords() const;
		void setMouseCoords(float x, float y);

	private:
		std::unordered_map<unsigned int, bool> _keyMap;

		glm::vec2 _mouseCoords;
	};

}
