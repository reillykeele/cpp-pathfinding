#include "MainGame.h"

#include <iostream>

#include <SDLEngine/ResourceManager.h>
#include <SDLEngine/SDLEngine.h>
#include <SDLEngine/Sprite.h>

#include "Pathfinding.h"

MainGame::MainGame() :
	_screenWidth(640),
	_screenHeight(480),
	_fps(0),
	_maxFPS(120.0f),
	_gameState(GameState::PLAY),

	_grid(20, 15),
	_isPathCalculated(false),
	_startPos(-1),
	_endPos(-1)
{
	_camera.init(_screenWidth, _screenHeight);
	_camera.setPosition(glm::vec2(_screenWidth / 2, _screenHeight / 2));
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();

	_grid.init();

	gameLoop();
}

void MainGame::initSystems()
{
	SDLEngine::init();

	_window.create("Pathfinding", _screenWidth, _screenHeight, 0);
	_window.setClearColor(41, 40, 49, 255);

	initShaders();

	_fpsLimiter.init(_maxFPS);
	_spriteBatch.init();
}

void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertPosition");
	_colorProgram.addAttribute("vertColor");
	_colorProgram.addAttribute("vertUV");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop()
{
	const float DESIRED_FPS = 60.0f;
	const int MAX_TIMESTEPS = 6;
	const float MS_PER_SEC = 1000.0f;
	const float DESIRED_FRAMETIME = MS_PER_SEC / DESIRED_FPS;
	const float MAX_DELTATIME = 1.0f;

	float prevTicks = SDL_GetTicks64();

	while(_gameState != GameState::EXIT)
	{
		_fpsLimiter.begin();

		float newTicks = SDL_GetTicks64();
		float frameTime = newTicks - prevTicks;
		prevTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		// Update engine components
		_camera.update();
		_inputManager.update();

		// Get input
		processInput();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_TIMESTEPS)
		{
			const float deltaTime = std::min(totalDeltaTime, MAX_DELTATIME);

			// Run game logic
			update(deltaTime);

			totalDeltaTime -= deltaTime;
			i++;
		}

		// Render
		draw();

		_fps = _fpsLimiter.end();

		// Print the FPS every 10 frames.
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10)
		{
			// std::cout << _fps << std::endl;
			frameCounter = 0;
		}

	}
}

void MainGame::processInput()
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
		switch (ev.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;

			case SDL_KEYDOWN:
				_inputManager.pressKey(ev.key.keysym.sym);
				break;
			case SDL_KEYUP:
				_inputManager.releaseKey(ev.key.keysym.sym);
				break;

			case SDL_MOUSEMOTION:
				_inputManager.setMouseCoords(ev.motion.x, ev.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				_inputManager.pressKey(ev.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				_inputManager.releaseKey(ev.button.button);
				break;
		}
	}
}

void MainGame::update(const float deltaTime)
{
	if (_inputManager.isKeyPressed(SDLK_SPACE))
	{
		auto mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);

		auto gridPos = _grid.convertWorldToGrid(mouseCoords);
		if (gridPos != Grid::INVALID)
		{
			// Toggle the grid's walkability at gridPos
			bool isWalkable = _grid.isWalkable(gridPos);
			_grid.setWalkable(gridPos, !isWalkable);

			_isPathCalculated = false;
		}
	}

	if (_inputManager.isKeyPressed(SDLK_m))
	{
		if (_grid.getMovementType() == FOUR_DIRECTIONAL)
			_grid.setMovementType(EIGHT_DIRECTIONAL);
		else if (_grid.getMovementType() == EIGHT_DIRECTIONAL)
			_grid.setMovementType(FOUR_DIRECTIONAL);

		_isPathCalculated = false;
	}

	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		auto mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);

		auto gridPos = _grid.convertWorldToGrid(mouseCoords);
		if (gridPos != Grid::INVALID)
		{
			// Set the path's start position
			_startPos = gridPos;
			_isPathCalculated = false;
		}
	}

	if (_inputManager.isKeyPressed(SDL_BUTTON_RIGHT))
	{
		auto mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);

		auto gridPos = _grid.convertWorldToGrid(mouseCoords);
		if (gridPos != Grid::INVALID)
		{
			// Set the path's end position
			_endPos = gridPos;
			_isPathCalculated = false;
		}
	}

	if (_isPathCalculated == false)
	{
		_isPathCalculated = true;
		if (_startPos == Grid::INVALID || _endPos == Grid::INVALID)
			return;

		_path.clear();
		_path  = Pathfinding::aStar(_grid, _startPos, _endPos);
	}
}

void MainGame::draw()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();

	glActiveTexture(GL_TEXTURE0);

	const auto textureLocation = _colorProgram.getUniformLocation("sampler");
	glUniform1i(textureLocation, 0);

	// Set the camera matrix
	const auto pLocation = _colorProgram.getUniformLocation("P");
	auto cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_grid.draw();
	drawGame();

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	_window.swap();
}

void MainGame::drawGame()
{
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	auto texture = SDLEngine::ResourceManager::getTexture("Textures/64-square-to.png");

	// draw path
	SDLEngine::ColorRGBA8 pathColor(74, 122, 150, 255);
	for (auto p : _path)
	{
		glm::vec4 destRect(p.x * _grid.TILE_SIZE, p.y * _grid.TILE_SIZE, _grid.TILE_SIZE, _grid.TILE_SIZE);
		_spriteBatch.draw(destRect, uvRect, texture.id, 0.0f, pathColor);
	}

	// draw start point
	if (_startPos != Grid::INVALID)
	{
		SDLEngine::ColorRGBA8 startColor(238, 134, 149, 255);
		glm::vec4 startDestRect(_startPos.x * _grid.TILE_SIZE, _startPos.y * _grid.TILE_SIZE, _grid.TILE_SIZE, _grid.TILE_SIZE);
		_spriteBatch.draw(startDestRect, uvRect, texture.id, 0.0f, startColor);
	}

	// draw end point
	if (_endPos != Grid::INVALID)
	{
		SDLEngine::ColorRGBA8 endColor(137, 238, 134, 255);
		glm::vec4 endDestRect(_endPos.x * _grid.TILE_SIZE, _endPos.y * _grid.TILE_SIZE, _grid.TILE_SIZE, _grid.TILE_SIZE);
		_spriteBatch.draw(endDestRect, uvRect, texture.id, 0.0f, endColor);
	}

	_spriteBatch.end();
	_spriteBatch.renderBatch();
}
