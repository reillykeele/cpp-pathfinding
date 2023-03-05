#pragma once

#include <Gl/glew.h>

struct Position
{
	float x;
	float y;
};

struct Color {
	GLbyte r;
	GLbyte g;
	GLbyte b;
	GLbyte a;
};

struct UV
{
	float u;
	float v;
};

struct Vertex
{
	Position position;

	Color color;

	UV uv;

	void setPosition(const float x, const float y)
	{
		position.x = x;
		position.y = y;
	}

	void setColor(const GLubyte r, const GLubyte g, const GLubyte b, const GLubyte a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}

	void setUV(const float u, const float v)
	{
		uv.u = u;
		uv.v = v;
	}
};