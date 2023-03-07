#pragma once

#include <vector>

#include <SDLEngine/GLSLProgram.h>
#include <SDLEngine/Window.h>

#include "SDLEngine/Camera2D.h"
#include "SDLEngine/Sprite.h"
#include "SDLEngine/SpriteBatch.h"

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

	SDLEngine::GLSLProgram _colorProgram;

	SDLEngine::Camera2D _camera;

	SDLEngine::SpriteBatch _spriteBatch;

	float _fps;
	float _maxFPS;
	float _frameTime;

	float _time;
};

