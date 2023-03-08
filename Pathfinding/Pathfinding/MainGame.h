#pragma once

#include <vector>

#include <SDLEngine/GLSLProgram.h>
#include <SDLEngine/Window.h>

#include "SDLEngine/Camera2D.h"
#include "SDLEngine/InputManager.h"
#include "SDLEngine/Sprite.h"
#include "SDLEngine/SpriteBatch.h"
#include "SDLEngine/Timing.h"

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

	SDLEngine::FPSLimiter _fpsLimiter;
	SDLEngine::Window _window;

	int _screenWidth;
	int _screenHeight;

	GameState _gameState;

	SDLEngine::GLSLProgram _colorProgram;

	SDLEngine::Camera2D _camera;

	SDLEngine::SpriteBatch _spriteBatch;

	SDLEngine::InputManager _inputManager;

	float _fps;
	float _maxFPS;

	float _time;
};

