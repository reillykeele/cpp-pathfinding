#pragma once

#include <Gl/glew.h>

namespace SDLEngine
{

	struct Position
	{
		Position() : x(0), y(0) { }
		Position(float x, float y) : x(x), y(y) { }

		float x;
		float y;
	};

	struct ColorRGBA8
	{
		ColorRGBA8() : r(255), g(255), b(255), a(255) { }
		ColorRGBA8(GLbyte r, GLbyte g, GLbyte b, GLbyte a) : r(r), g(g), b(b), a(a) { }

		GLbyte r;
		GLbyte g;
		GLbyte b;
		GLbyte a;
	};

	struct UV
	{
		UV() : u(0), v(0) { }
		UV(float u, float v) : u(u), v(v) { }

		float u;
		float v;
	};

	struct Vertex
	{
		Position position;

		ColorRGBA8 color;

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

}