#include "Pathfinding.h"

#include <queue>
#include <stack>
#include <unordered_map>

double Pathfinding::heuristic(const glm::ivec2 pos, const glm::ivec2 dest)
{
	return std::abs(pos.x - dest.x) + std::abs(pos.y - dest.y);
}

std::vector<glm::ivec2> Pathfinding::aStar(Grid& grid, glm::ivec2 startPos, glm::ivec2 destPos)
{
	std::vector<glm::ivec2> empty;
	if (grid.isValid(destPos) == false || startPos == destPos)
		return empty;

	std::priority_queue<Node, std::vector<Node>, std::greater<>> frontier;
	std::unordered_map<glm::ivec2, Node> closed;

	Node startNode = Node(startPos, glm::ivec2(-1), 0, 0, 0);
	frontier.push(startNode);
	closed[startPos] = startNode;

	while (frontier.empty() == false)
	{
		auto curr = frontier.top();
		frontier.pop();

		if (curr.pos == destPos)
			// TODO fix
			return makePath(grid, closed, destPos);

		auto neighbours = grid.getWalkableNeighbours(curr.pos);
		for (auto n : neighbours)
		{
			double gNew = curr.gCost + grid.getCost(curr.pos, n.pos);

			// If we haven't explored it yet OR
			// if we found a shorter path
			auto it = closed.find(n.pos);
			if (it == closed.end() ||
				gNew < it->second.gCost)
			{
				double fNew = gNew + heuristic(n.pos, destPos);

				closed[n.pos] = Node(n.pos, curr.pos, gNew, 0.0, fNew);
				frontier.emplace(n.pos, curr.pos, gNew, 0.0, fNew);
			}
		}
	}

	return empty;
}

std::vector<glm::ivec2> Pathfinding::makePath(Grid& grid, std::unordered_map<glm::ivec2, Node>& nodes, glm::ivec2 destPos)
{
	std::stack<glm::ivec2> path;
	std::vector<glm::ivec2> usablePath;

	Node curr = nodes[destPos];
	while (curr.parentPos != Grid::INVALID)
	{
		path.push(curr.pos);

		curr = nodes[curr.parentPos];
	}

	while (!path.empty()) {
		auto top = path.top();
		path.pop();
		usablePath.emplace_back(top);
	}

	return usablePath;
}
