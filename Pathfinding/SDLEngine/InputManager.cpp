#include "InputManager.h"

namespace SDLEngine
{

	InputManager::InputManager() : _mouseCoords(0)
	{
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::update()
	{
		for (auto& it : _keyMap)
		{
			_prevKeyMap[it.first] = it.second;
		}
	}

	void InputManager::pressKey(unsigned int keyID)
	{
		_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}

	bool InputManager::isKeyDown(unsigned int keyID)
	{
		auto keyIter = _keyMap.find(keyID);
		if (keyIter != _keyMap.end())
			return keyIter->second;

		return false;
	}

	bool InputManager::isKeyPressed(unsigned keyID)
	{
		if (isKeyDown(keyID) && wasKeyDown(keyID) == false)
			return true;

		return false;
	}

	glm::vec2 InputManager::getMouseCoords() const { return _mouseCoords; }

	void InputManager::setMouseCoords(float x, float y)
	{
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}

	bool InputManager::wasKeyDown(unsigned keyID)
	{
		auto keyIter = _prevKeyMap.find(keyID);
		if (keyIter != _prevKeyMap.end())
			return keyIter->second;

		return false;
	}
}
