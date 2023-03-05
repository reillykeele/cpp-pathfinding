#include "IOManager.h"

#include <fstream>

bool IOManager::readFileToBuffer(const std::string filePath, std::vector<unsigned char>& buffer)
{
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail())
	{
		perror(filePath.c_str());
		return false;
	}

	// seek to eof
	file.seekg(0, std::ios::end);

	// get the file size
	int fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// ignore the file's header bytes
	fileSize -= file.tellg();

	buffer.resize(fileSize);
	file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
	file.close();

	return true;
}
