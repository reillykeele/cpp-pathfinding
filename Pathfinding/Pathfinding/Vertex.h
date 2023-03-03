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

struct Vertex
{
	Position position;

	Color color;
};