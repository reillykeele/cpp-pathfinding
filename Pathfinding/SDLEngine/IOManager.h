#pragma once

#include <string>
#include <vector>

namespace SDLEngine
{
	static class IOManager
	{
	public:
		static bool readFileToBuffer(const std::string filePath, std::vector<unsigned char>& buffer);
	};

}