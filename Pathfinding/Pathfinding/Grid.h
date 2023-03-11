#pragma once
#include <vector>
#include <GLM/vec2.hpp>

#include <SDLEngine/SpriteBatch.h>

class GridCell
{
public:
	GridCell(): pos() { }
	GridCell(int x, int y): pos(x, y) { }
	GridCell(glm::ivec2 pos) : pos(pos) { }

	glm::ivec2 pos;
	bool isWalkable = true;
};

class Grid
{
public:
	Grid();
	Grid(int width, int height);
	~Grid();

	void init();
	void init(int width, int height);

	void draw();

	std::vector<GridCell> getNeighbours(glm::ivec2 pos);

	GridCell& get(glm::ivec2 pos);

	bool isValid(int x, int y) { return isValid(glm::vec2(x, y)); }
	bool isValid(glm::ivec2 pos);

	bool isWalkable(int x, int y) { return isWalkable(glm::vec2(x, y)); }
	bool isWalkable(glm::ivec2 pos);

	glm::ivec2 convertWorldToGrid(glm::vec2 pos);

	int getWidth() const { return _width; }
	int getHeight() const { return _height; }
	int getSize() const { return _width * _height; }

	void setPosition(glm::vec2 pos) { _position = pos; }

	static const int TILE_SIZE = 32;
	static const glm::ivec2 INVALID;

private:
	void initSpriteBatch();

	int _width;
	int _height;

	glm::vec2 _position;

	std::vector<std::vector<GridCell>> _map;

	SDLEngine::SpriteBatch _spriteBatch;
};

