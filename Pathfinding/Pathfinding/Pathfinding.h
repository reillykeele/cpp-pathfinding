#pragma once
#include <vector>
#include <GLM/vec2.hpp>

#include "Grid.h"

class Node
{
public:
	Node() : pos(-1), parentPos(-1), gCost(0), hCost(0), fCost(0)
	{
	}

	Node(glm::ivec2 pos) : pos(pos), parentPos(-1), gCost(0), hCost(0), fCost(0)
	{
	}

	Node(glm::ivec2 pos, glm::ivec2 parentPos, float g, float h, float f): pos(pos), parentPos(parentPos), gCost(g), hCost(h), fCost(f)
	{
	}

	glm::ivec2 pos;
	glm::ivec2 parentPos;

	float gCost;
	float hCost;
	float fCost;

	bool operator< (const Node& rhs) const
	{
		return fCost < rhs.fCost;
	}
};

class Pathfinding
{
public:
	static double calculateH(const glm::ivec2 pos, const Node& dest);

    static std::vector<glm::ivec2> aStar(Grid& grid, glm::ivec2 startPos, glm::ivec2 destPos);

	static std::vector<glm::ivec2> makePath(std::vector<std::vector<Node>> map, glm::ivec2 destPos);

};

