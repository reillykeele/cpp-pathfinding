#include "Agent.h"

Agent::Agent() : _position(0)
{
}

Agent::Agent(Grid* grid, glm::vec2 pos): _grid(grid), _position(pos)
{
}

Agent::~Agent()
{
}

void Agent::findPath(glm::vec2 endPos)
{
	std::vector<GridCell> frontier;

}
