#pragma once

#include <unordered_map>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/hash.hpp>
#include <GLM/vec2.hpp>

#include "Grid.h"



class Node
{
public:
	Node() : pos(-1), parentPos(-1), gCost(FLT_MAX), hCost(FLT_MAX), fCost(FLT_MAX)
	{
	}

	Node(glm::ivec2 pos) : pos(pos), parentPos(-1), gCost(FLT_MAX), hCost(FLT_MAX), fCost(FLT_MAX)
	{
	}

	Node(glm::ivec2 pos, glm::ivec2 parentPos, double g, double h, double f): pos(pos), parentPos(parentPos), gCost(g), hCost(h), fCost(f)
	{
	}

	glm::ivec2 pos;
	glm::ivec2 parentPos;

	double gCost;
	double hCost;
	double fCost;

	bool operator> (const Node& rhs) const
	{
		return fCost > rhs.fCost;
	}

	bool operator< (const Node& rhs) const
	{
		return fCost < rhs.fCost;
	}
};

class Pathfinding
{
public:
	static double heuristic(const glm::ivec2 pos, const glm::ivec2 dest);

	static std::vector<glm::ivec2> aStar(Grid& grid, glm::ivec2 startPos, glm::ivec2 destPos);

	static std::vector<glm::ivec2> makePath(Grid& grid, std::unordered_map<glm::ivec2, Node>& nodes, glm::ivec2 destPos);

};

