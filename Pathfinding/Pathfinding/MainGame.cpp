#include "MainGame.h"

#include <iostream>

#include <SDLEngine/ResourceManager.h>
#include <SDLEngine/SDLEngine.h>
#include <SDLEngine/Sprite.h>

#include "Pathfinding.h"

// const SDLEngine::ColorRGBA8 MainGame::BACKGROUND_COLOR = SDLEngine::ColorRGBA8(255, 246, 211, 255);
// const SDLEngine::ColorRGBA8 MainGame::GRID_COLOR       = SDLEngine::ColorRGBA8(255, 246, 211, 255);
// const SDLEngine::ColorRGBA8 MainGame::START_COLOR	   = SDLEngine::ColorRGBA8(235, 107, 111, 255);
// const SDLEngine::ColorRGBA8 MainGame::END_COLOR		   = SDLEngine::ColorRGBA8(124,  63,  88, 255);
// const SDLEngine::ColorRGBA8 MainGame::PATH_COLOR	   = SDLEngine::ColorRGBA8(249, 168, 117, 255);

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
	// static const float CAMERA_SPEED = 2.0f;
	// static const float SCALE_SPEED = 0.1f;
	//
	// if (_inputManager.isKeyDown(SDLK_w))
	// 	_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED * deltaTime));
	//
	// if (_inputManager.isKeyDown(SDLK_s))
	// 	_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED * deltaTime));
	//
	// if (_inputManager.isKeyDown(SDLK_d))
	// 	_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED * deltaTime, 0.0f));
	//
	// if (_inputManager.isKeyDown(SDLK_a))
	// 	_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED * deltaTime, 0.0f));
	//
	// if (_inputManager.isKeyDown(SDLK_e))
	// 	_camera.setScale(_camera.getScale() + SCALE_SPEED * deltaTime);
	//
	// if (_inputManager.isKeyDown(SDLK_q))
	// 	_camera.setScale(std::max(_camera.getScale() - SCALE_SPEED * deltaTime, 0.01f));

	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		auto mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;

		auto gridPos = _grid.convertWorldToGrid(mouseCoords);
		if (gridPos != Grid::INVALID)
		{
			_startPos = gridPos;
			_isPathCalculated = false;
		}
	}

	if (_inputManager.isKeyPressed(SDL_BUTTON_RIGHT))
	{
		auto mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;

		auto gridPos = _grid.convertWorldToGrid(mouseCoords);
		if (gridPos != Grid::INVALID)
		{
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
