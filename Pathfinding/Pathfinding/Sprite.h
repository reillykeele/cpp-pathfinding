#pragma once

#include <string>
#include <GL/glew.h>

#include "GLTexture.h"

class Sprite
{
public:
	Sprite();
	~Sprite();

	void init(float x, float y, float width, float height, std::string texturePath);

	void draw();

private:
	int _x;
	int _y;

	float _width;
	float _height;

	GLuint _vboID;

	GLTexture _texture;
};

