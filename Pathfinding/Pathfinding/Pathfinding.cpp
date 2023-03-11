#include "Pathfinding.h"

#include <iostream>
#include <stack>

double Pathfinding::calculateH(const glm::ivec2 pos, const Node& dest)
{
	double h =
		sqrt((pos.x - dest.pos.x) * (pos.x - dest.pos.x) +
			(pos.y - dest.pos.y) * (pos.y - dest.pos.y));


	return h;
}

std::vector<glm::ivec2> Pathfinding::aStar(Grid& grid, glm::ivec2 startPos, glm::ivec2 destPos)
{
	std::vector<glm::ivec2> empty;
	if (grid.isValid(destPos) == false) {
		std::cout << "Destination is an obstacle" << std::endl;
		return empty;
	}
	if (startPos == destPos) {
		std::cout << "You are the destination" << std::endl;
		return empty;
	}

	std::vector<std::vector<bool>> closedList(grid.getHeight(), std::vector<bool>(grid.getWidth()));

	//Initialize whole map
	std::vector<std::vector<Node>> allMap(grid.getHeight(), std::vector<Node>(grid.getWidth()));
	for (int y = 0; y < grid.getHeight(); y++) 
	{
		for (int x = 0; x < grid.getWidth(); x++) 
		{
		    allMap[y][x].fCost = FLT_MAX;
		    allMap[y][x].gCost = FLT_MAX;
		    allMap[y][x].hCost = FLT_MAX;
		    allMap[y][x].parentPos = glm::ivec2(-1);
			allMap[y][x].pos = glm::ivec2(x, y);
		
		    closedList[y][x] = false;
		}
	}
		
	//Initialize our starting list
	allMap[startPos.y][startPos.x].fCost = 0.0;
	allMap[startPos.y][startPos.x].gCost = 0.0;
	allMap[startPos.y][startPos.x].hCost = 0.0;
	allMap[startPos.y][startPos.x].parentPos = glm::ivec2(startPos.x, startPos.y);
		
	std::vector<Node> openList;
	openList.emplace_back(allMap[startPos.y][startPos.x]);

	glm::ivec2 pos(startPos.x, startPos.y);
	bool destinationFound = false;
		
	while (!openList.empty() && openList.size() < grid.getSize()) 
	{
		Node node;
		do 
		{
		    float temp = FLT_MAX;
		    std::vector<Node>::iterator itNode;
		    for (auto it = openList.begin(); it != openList.end(); it = next(it)) 
			{
		        Node n = *it;
		        if (n.fCost < temp) {
		            temp = n.fCost;
		            itNode = it;
		        }
		    }

		    node = *itNode;
		    openList.erase(itNode);
		}
		while (grid.isValid(node.pos) == false);

		pos = node.pos;
		closedList[pos.y][pos.x] = true;
		
		// For each neighbour starting from North-West to South-East
		for (int newX = -1; newX <= 1; newX++) 
		{
		    for (int newY = -1; newY <= 1; newY++) 
			{
				glm::ivec2 newPos(pos.x + newX, pos.y + newY);
		    	double gNew, hNew, fNew;

		    	if (grid.isValid(newPos)) 
				{
					// Destination found - make path
		    		if (newPos == destPos)
		            {
		                allMap[newPos.y][newPos.x].parentPos = pos;
		                destinationFound = true;

						return makePath(allMap, destPos);
		            }

					// If we haven't explored it yet
		    		if (closedList[newPos.y][newPos.x] == false)
		            {
			            gNew = node.gCost + 1.0;
			            hNew = calculateH(newPos, destPos);
			            fNew = gNew + hNew;

			            // Check if this path is better than the one already present
			            if (allMap[newPos.y][newPos.x].fCost == FLT_MAX ||
				            allMap[newPos.y][newPos.x].fCost > fNew)
			            {
				            // Update the details of this neighbour node
				            allMap[newPos.y][newPos.x].fCost = fNew;
				            allMap[newPos.y][newPos.x].gCost = gNew;
				            allMap[newPos.y][newPos.x].hCost = hNew;
							allMap[newPos.y][newPos.x].parentPos = pos;

			            	openList.emplace_back(allMap[newPos.y][newPos.x]);
			            }
		            }
		        }
		    }
		}
	}

	if (destinationFound == false) {
		std::cout << "Destination not found" << std::endl;
		return empty;
	}

	return empty;
}

std::vector<glm::ivec2> Pathfinding::makePath(std::vector<std::vector<Node>> map, glm::ivec2 destPos)
{
	try 
	{
		glm::ivec2 pos(destPos);

		std::stack<glm::ivec2> path;
		std::vector<glm::ivec2> usablePath;

		while (map[pos.y][pos.x].parentPos != pos &&
			   map[pos.y][pos.x].pos.x != -1 &&
			   map[pos.y][pos.x].pos.y != -1)
		{
			path.push(map[pos.y][pos.x].pos);

			pos = glm::ivec2(map[pos.y][pos.x].parentPos);

		}
		path.push(map[pos.y][pos.x].pos);

		while (!path.empty()) {
			auto top = path.top();
			path.pop();
			usablePath.emplace_back(top);
		}

		return usablePath;
	}
	catch (const std::exception& e) 
	{
		std::cout << e.what() << std::endl;
	}

	return {};
}
