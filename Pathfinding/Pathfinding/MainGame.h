#pragma once

#include <SDLEngine/Camera2D.h>
#include <SDLEngine/GLSLProgram.h>
#include <SDLEngine/InputManager.h>
#include <SDLEngine/SpriteBatch.h>
#include <SDLEngine/Timing.h>
#include <SDLEngine/Window.h>

#include "Grid.h"

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

	void update(const float deltaTime);

	void draw();

	void drawGame();

	int _screenWidth;
	int _screenHeight;

	float _fps;
	float _maxFPS;

	SDLEngine::FPSLimiter _fpsLimiter;
	SDLEngine::Window _window;

	SDLEngine::GLSLProgram _colorProgram;

	SDLEngine::SpriteBatch _spriteBatch;

	SDLEngine::Camera2D _camera;
	SDLEngine::InputManager _inputManager;

	GameState _gameState;

	// Pathfinding
	Grid _grid;
	std::vector<glm::ivec2> _path;

	bool _isPathCalculated;

	glm::ivec2 _startPos;
	glm::ivec2 _endPos;

	// static const SDLEngine::ColorRGBA8 BACKGROUND_COLOR;
	// static const SDLEngine::ColorRGBA8 GRID_COLOR;
	// static const SDLEngine::ColorRGBA8 START_COLOR;
	// static const SDLEngine::ColorRGBA8 END_COLOR;
	// static const SDLEngine::ColorRGBA8 PATH_COLOR;
};

