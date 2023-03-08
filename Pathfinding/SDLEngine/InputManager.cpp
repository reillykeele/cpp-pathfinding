#include "InputManager.h"

SDLEngine::InputManager::InputManager(): _mouseCoords(0)
{
}

SDLEngine::InputManager::~InputManager()
{
}

void SDLEngine::InputManager::pressKey(unsigned int keyID)
{
	_keyMap[keyID] = true;
}

void SDLEngine::InputManager::releaseKey(unsigned int keyID)
{
	_keyMap[keyID] = false;
}

bool SDLEngine::InputManager::isKeyPressed(unsigned int keyID)
{
	auto keyIter = _keyMap.find(keyID);
	if (keyIter != _keyMap.end())
		return keyIter->second;

	return false;
}

glm::vec2 SDLEngine::InputManager::getMouseCoords() const { return _mouseCoords; }

void SDLEngine::InputManager::setMouseCoords(float x, float y)
{
	_mouseCoords.x = x;
	_mouseCoords.y = y;
}
