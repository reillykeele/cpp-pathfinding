#pragma once

#include <vector>
#include <SDL/SDL.h>
#include <GL/glew.h>

#include "GLSLProgram.h"
#include "GLTexture.h"
#include "Sprite.h"

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

	SDL_Window* _window;

	int _screenWidth;
	int _screenHeight;

	GameState _gameState;

	std::vector<Sprite*> _sprites;
	// Sprite _sprite;

	GLSLProgram _colorProgram;

	float _time;
};

