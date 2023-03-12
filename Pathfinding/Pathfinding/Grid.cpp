#include "Grid.h"

#include <SDLEngine/ResourceManager.h>

const glm::ivec2 Grid::INVALID = glm::ivec2(-1, -1);

Grid::Grid() = default;

Grid::Grid(int width, int height):
	_width(width),
	_height(height),
	_shouldRedraw(true),
	_position(0.0f),
	_map(height, std::vector<GridCell>(width)),
	_movementType(EIGHT_DIRECTIONAL)
{
	for (int y = 0; y < _map.size(); y++)
		for (int x = 0; x < _map[y].size(); x++)
			_map[y][x].pos = glm::ivec2(x, y);
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
	for (int y = 0; y < _map.size(); y++)
	{
		_map[y].resize(_width);

		for (int x = 0; x < _map[y].size(); x++)
			_map[y][x].pos = glm::ivec2(x, y);
	}

	initSpriteBatch();
}

void Grid::draw()
{
	if (_shouldRedraw)
		initSpriteBatch();

	_spriteBatch.renderBatch();
}

std::vector<GridCell> Grid::getNeighbours(glm::ivec2 pos)
{
	std::vector<GridCell> neighbours;

	for (int y = std::max(0, pos.y - 1); y <= std::min(_height - 1, pos.y + 1); y++)
	{
		for (int x = std::max(0, pos.x - 1); x <= std::min(_width - 1, pos.x + 1); x++)
		{
			if (x == pos.x && y == pos.y ||
				(_movementType == MovementType::FOUR_DIRECTIONAL && x != pos.x && y != pos.y))
				continue;

			neighbours.emplace_back(_map[y][x]);
		}
	}

	return neighbours;
}

std::vector<GridCell> Grid::getWalkableNeighbours(glm::ivec2 pos)
{
	std::vector<GridCell> neighbours;

	for (int y = std::max(0, pos.y - 1); y <= std::min(_height - 1, pos.y + 1); y++)
	{
		for (int x = std::max(0, pos.x - 1); x <= std::min(_width - 1, pos.x + 1); x++)
		{
			if ((x == pos.x && y == pos.y) ||
				(_movementType == MovementType::FOUR_DIRECTIONAL && x != pos.x && y != pos.y) ||
				isWalkable(x, y) == false)
				continue;


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

void Grid::clear()
{
	_movementType = EIGHT_DIRECTIONAL;

	for (int y = 0; y < _map.size(); y++)
		for (int x = 0; x < _map[y].size(); x++)
			_map[y][x].isWalkable = true;

	_shouldRedraw = true;
}

void Grid::setWalkable(glm::ivec2 pos, bool isWalkable)
{
	GridCell& cell = get(pos);

	if (cell.isWalkable != isWalkable)
	{
		cell.isWalkable = isWalkable;
		_shouldRedraw = true;
	}
}

double Grid::getCost(const glm::ivec2 a, const glm::ivec2 b) const
{
	if (_movementType == FOUR_DIRECTIONAL)
		return std::abs(a.x - b.x) + std::abs(a.y - b.y);

	return std::sqrt(std::abs(a.x - b.x) + std::abs(a.y - b.y));
}

bool Grid::isValid(const glm::ivec2 pos) const
{
	if (pos.x >= 0 && pos.x < _width &&
		pos.y >= 0 && pos.y < _height)
		return true;

	return false;
}

bool Grid::isWalkable(const glm::ivec2 pos) const
{
	if (isValid(pos) && _map[pos.y][pos.x].isWalkable)
		return true;

	return false;
}

glm::ivec2 Grid::convertWorldToGrid(glm::vec2 pos)
{
	glm::vec2 temp = (pos - _position);
	temp /= TILE_SIZE;

	if (isValid(temp))
		return temp;

	return INVALID;
}

void Grid::initSpriteBatch()
{
	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	SDLEngine::ColorRGBA8 color(51, 63, 88, 255);

	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x <_width; x++)
		{
			if (isWalkable(x, y) == false)
				continue;

			glm::vec4 destRect(_position.x + x * TILE_SIZE, _position.y + y * TILE_SIZE, TILE_SIZE, TILE_SIZE);

			_spriteBatch.draw(destRect,
				uvRect,
				SDLEngine::ResourceManager::getTexture("Textures/64-square-to.png").id,
				0.0f,
				color);
		}
	}

	_spriteBatch.end();

	_shouldRedraw = false;
}
