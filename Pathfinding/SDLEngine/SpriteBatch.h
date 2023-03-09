#pragma once

#include <vector>
#include <GL/glew.h>
#include <GLM/glm.hpp>

#include "Vertex.h"

namespace SDLEngine
{
	enum class GlyphSortType
	{
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	struct Glyph
	{
		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	};

	class RenderBatch
	{
	public:
		RenderBatch(GLuint offset, GLuint numVertices, GLuint texture):
			offset(offset),
			numVertices(numVertices),
			texture(texture)
		{
		}

		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	class SpriteBatch
	{
	public:

		SpriteBatch();
		~ SpriteBatch();

		void init();

		/**
		 * \brief Indicates the start of the batch.
		 */
		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);

		/**
		 * \brief Indicates the end of the batch.
		 */
		void end();

		/**
		 * \brief Adds to the batch. Called in-between begin() and end().
		 */
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);

		/**
		 * \brief Draw the batch to the screen. Called at after end().
		 */
		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GLuint _vbo;
		GLuint _vao;

		GlyphSortType _sortType;

		std::vector<Glyph*> _glyphs;
		std::vector<RenderBatch> _renderBatches;
	};

}
