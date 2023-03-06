#pragma once

#include <vector>

#include <SDLEngine/GLSLProgram.h>
#include <SDLEngine/Window.h>

#include "SDLEngine/Sprite.h"

enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();
	void calculateFPS();

	SDLEngine::Window _window;

	int _screenWidth;
	int _screenHeight;

	GameState _gameState;

	std::vector<SDLEngine::Sprite*> _sprites;
	// Sprite _sprite;

	SDLEngine::GLSLProgram _colorProgram;

	float _fps;
	float _maxFPS;
	float _frameTime;

	float _time;
};

