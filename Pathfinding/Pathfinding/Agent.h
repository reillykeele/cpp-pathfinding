#pragma once
#include <GLM/vec2.hpp>

#include "Grid.h"

class Agent
{
public:
	Agent();
	Agent(Grid* grid, glm::vec2 pos);
	~Agent();

	void findPath(glm::vec2 endPos);

private:
	glm::vec2 _position;

	Grid* _grid;
};

