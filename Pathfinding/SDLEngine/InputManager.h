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

		void update();

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		/**
		 * \brief Check if a key is held down. 
		 * \param keyID SDL key type.
		 * \return Returns true if the SDL key keyID is held down. 
		 */
		bool isKeyDown(unsigned int keyID);


		/**
		 * \brief Check if a key was just pressed this frame.
		 * \param keyID SDL key type.
		 * \return Returns true if the SDL key keyID was just pressed.
		 */
		bool isKeyPressed(unsigned int keyID);

		glm::vec2 getMouseCoords() const;
		void setMouseCoords(float x, float y);

	private:
		bool wasKeyDown(unsigned int keyID);

		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _prevKeyMap;

		glm::vec2 _mouseCoords;
	};

}
