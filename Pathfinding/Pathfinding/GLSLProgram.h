#pragma once

#include <string>
#include <GL/glew.h>

class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();

	void compileShaders(const std::string& vertShaderFilepath, const std::string& fragShaderFilepath);

	void linkShaders();

	void addAttribute(const std::string& attributeName);

	void use();
	void unuse();

private:
	void compileShader(const std::string& filePath, GLuint shaderId);

	int _numAttributes;

	GLuint _programID;
	GLuint _vertShaderID;
	GLuint _fragShaderID;
};

