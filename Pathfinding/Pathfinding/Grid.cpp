#include "Grid.h"

#include <iostream>
#include <SDLEngine/ResourceManager.h>

const glm::ivec2 Grid::INVALID = glm::ivec2(-1, -1);

Grid::Grid() = default;

Grid::Grid(int width, int height):
	_width(width),
	_height(height),
	_position(0.0f),
	_map(height, std::vector<GridCell>(width))
{
}

Grid::~Grid() = default;

void Grid::init()
{
	initSpriteBatch();
}

void Grid::init(int width, int height)
{
	_width = width;
	_height = height;

	_map.resize(_height);
	for (int i = 0; i < _map.size(); i++)
		_map[i].resize(_width);

	initSpriteBatch();
}

void Grid::draw()
{
	_spriteBatch.renderBatch();
}

std::vector<GridCell> Grid::getNeighbours(glm::ivec2 pos)
{
	std::vector<GridCell> neighbours;

	for (int y = std::max(0, pos.y - 1); y <= std::min(_height - 1, pos.y + 1); y++)
	{
		for (int x = std::max(0, pos.x - 1); x <= std::min(_width - 1, pos.x + 1); x++)
		{
			if (x == pos.x && y == pos.y)
				continue;

			std::cout << "(" << x << ", " << y << ")" << std::endl;
			neighbours.emplace_back(_map[y][x]);
		}
	}

	return neighbours;
}

GridCell& Grid::get(glm::ivec2 pos)
{
	assert(pos.x >= 0 && pos.x < _width);
	assert(pos.y >= 0 && pos.y < _height);

	return _map[pos.y][pos.x];
}

bool Grid::isValid(glm::ivec2 pos)
{
	if (pos.x >= 0 && pos.x < _width &&
		pos.y >= 0 && pos.y < _height)
		return true;

	return false;
}

bool Grid::isWalkable(glm::ivec2 pos)
{
	if (isValid(pos) && get(pos).isWalkable)
		return true;

	return false;
}

glm::ivec2 Grid::convertWorldToGrid(glm::vec2 pos)
{
	glm::vec2 temp = (pos - _position);
	temp /= TILE_SIZE;

	if (isValid(temp))
	{
		std::cout << (int) temp.x << " " << (int) temp.y << std::endl;
		return temp;
	}

	std::cout << "off the grid" << std::endl;
	return INVALID;
}

void Grid::initSpriteBatch()
{
	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	SDLEngine::ColorRGBA8 color;

	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x <_width; x++)
		{
			glm::vec4 destRect(_position.x + x * TILE_SIZE, _position.y + y * TILE_SIZE, TILE_SIZE, TILE_SIZE);

			_spriteBatch.draw(destRect,
				uvRect,
				SDLEngine::ResourceManager::getTexture("Textures/64-square-to.png").id,
				0.0f,
				color);
		}
	}

	_spriteBatch.end();
}
