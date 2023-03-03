#include "GLSLProgram.h"
#include "Errors.h"

#include <fstream>
#include <vector>

GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertShaderID(0), _fragShaderID(0)
{
	
}

GLSLProgram::~GLSLProgram()
{
}

void GLSLProgram::compileShaders(const std::string& vertShaderFilepath, const std::string& fragShaderFilepath)
{
	_programID = glCreateProgram();

	_vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertShaderID == 0)
		fatalError("Failed to create vertex shader.");

	_fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragShaderID == 0)
		fatalError("Failed to create fragment shader.");

	compileShader(vertShaderFilepath, _vertShaderID);
	compileShader(fragShaderFilepath, _fragShaderID);
}

void GLSLProgram::linkShaders()
{
	glAttachShader(_programID, _vertShaderID);
	glAttachShader(_programID, _fragShaderID);

	glLinkProgram(_programID);

	// Check for linking errors
	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, errorLog.data());

		glDeleteProgram(_programID);

		glDeleteShader(_vertShaderID);
		glDeleteShader(_fragShaderID);

		std::printf("%s\n", errorLog.data());
		fatalError("Failed to link shaders.");
	}

	// Always detach shaders after a successful link
	glDetachShader(_programID, _vertShaderID);
	glDetachShader(_programID, _fragShaderID);
	glDeleteShader(_vertShaderID);
	glDeleteShader(_fragShaderID);
}

void GLSLProgram::addAttribute(const std::string& attributeName)
{
	glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
}

GLuint GLSLProgram::getUniformLocation(const std::string& uniformName)
{
	auto location = glGetUniformLocation(_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX)
		fatalError("Failed to find uniform '" + uniformName + "' in shader.");

	return location;
}

void GLSLProgram::use()
{
	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; ++i)
		glEnableVertexAttribArray(i);
}

void GLSLProgram::unuse()
{
	glUseProgram(0);
	for (int i = 0; i < _numAttributes; ++i)
		glDisableVertexAttribArray(i);
}

void GLSLProgram::compileShader(const std::string& filePath, GLuint shaderId)
{
	std::ifstream shaderFile(filePath);
	if (shaderFile.fail())
		fatalError("Failed to open '" + filePath + "'.");

	std::string shaderFileContents = "";
	std::string line;

	while (std::getline(shaderFile, line))
		shaderFileContents += line + '\n';

	shaderFile.close();

	const char* shaderFileContentsPtr = shaderFileContents.c_str();
	glShaderSource(shaderId, 1, &shaderFileContentsPtr, nullptr);

	glCompileShader(shaderId);

	// Check for compile errors
	GLint isCompiled = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, errorLog.data());

		glDeleteShader(shaderId);

		std::printf("%s\n", errorLog.data());
		fatalError("Failed to compile " + filePath +" shader.");
	}
}
