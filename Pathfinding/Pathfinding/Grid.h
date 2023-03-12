#pragma once
#include <vector>
#include <GLM/vec2.hpp>

#include <SDLEngine/SpriteBatch.h>

enum MovementType { FOUR_DIRECTIONAL, EIGHT_DIRECTIONAL };

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
	std::vector<GridCell> getWalkableNeighbours(glm::ivec2 pos);

	GridCell& get(glm::ivec2 pos);

	void clear();

	void setWalkable(glm::ivec2 pos, bool isWalkable);

	double getCost(const glm::ivec2 a, const glm::ivec2 b) const;

	bool isValid(const int x, const int y) const { return isValid(glm::vec2(x, y)); }
	bool isValid(const glm::ivec2 pos) const;

	bool isWalkable(const int x, const int y) const { return isWalkable(glm::vec2(x, y)); }
	bool isWalkable(const glm::ivec2 pos) const;

	glm::ivec2 convertWorldToGrid(glm::vec2 pos);

	int getWidth() const { return _width; }
	int getHeight() const { return _height; }
	int getSize() const { return _width * _height; }

	void setPosition(glm::vec2 pos) { _position = pos; }

	MovementType getMovementType() const { return _movementType; }
	void setMovementType(MovementType move) { _movementType = move;  }

	static const int TILE_SIZE = 32;
	static const glm::ivec2 INVALID;

private:
	void initSpriteBatch();

	int _width;
	int _height;

	bool _shouldRedraw;

	glm::vec2 _position;

	std::vector<std::vector<GridCell>> _map;

	SDLEngine::SpriteBatch _spriteBatch;

	MovementType _movementType;
};

