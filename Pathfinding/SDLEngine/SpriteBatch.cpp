#include "SpriteBatch.h"

#include <algorithm>

namespace SDLEngine
{
	SpriteBatch::SpriteBatch(): _vbo(0), _vao(0)
	{
	}

	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init()
	{
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */)
	{
		_sortType = sortType;
		
		_renderBatches.clear();
		_glyphs.clear();
	} 

	void SpriteBatch::end()
	{
		_glyphPointers.resize(_glyphs.size());
		for (int i = 0; i < _glyphPointers.size(); i++)
			_glyphPointers[i] = &_glyphs[i];

		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color)
	{
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(_vao);

		for (const auto& renderBatch : _renderBatches)
		{
			glBindTexture(GL_TEXTURE_2D, renderBatch.texture);

			glDrawArrays(GL_TRIANGLES, renderBatch.offset, renderBatch.numVertices);
		}

		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches()
	{
		if (_glyphs.empty())
			return;

		std::vector<Vertex> vertices;
		vertices.resize(_glyphs.size() * 6);

		int offset = 0;
		int currVertex = 0;
		_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);
		vertices[currVertex++] = _glyphPointers[0]->topLeft;
		vertices[currVertex++] = _glyphPointers[0]->bottomLeft;
		vertices[currVertex++] = _glyphPointers[0]->bottomRight;
		vertices[currVertex++] = _glyphPointers[0]->bottomRight;
		vertices[currVertex++] = _glyphPointers[0]->topRight;
		vertices[currVertex++] = _glyphPointers[0]->topLeft;
		offset += 6;

		for (int currGlyph = 1; currGlyph < _glyphPointers.size(); currGlyph++)
		{
			if (_glyphPointers[currGlyph]->texture != _glyphPointers[currGlyph - 1]->texture)
				_renderBatches.emplace_back(offset, 6, _glyphPointers[currGlyph]->texture);
			else
				_renderBatches.back().numVertices += 6;

			vertices[currVertex++] = _glyphPointers[currGlyph]->topLeft;
			vertices[currVertex++] = _glyphPointers[currGlyph]->bottomLeft;
			vertices[currVertex++] = _glyphPointers[currGlyph]->bottomRight;
			vertices[currVertex++] = _glyphPointers[currGlyph]->bottomRight;
			vertices[currVertex++] = _glyphPointers[currGlyph]->topRight;
			vertices[currVertex++] = _glyphPointers[currGlyph]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		// orphan the buffer https://www.khronos.org/opengl/wiki/Buffer_Object_Streaming
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		// upload the data to the buffer
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray()
	{
		if (_vao == 0)
			glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		if (_vbo == 0)
			glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// Position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		// Color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		// UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyphs()
	{
		// use stable sort to retain order if equal
		switch (_sortType)
		{
			case GlyphSortType::FRONT_TO_BACK: 
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
				break;
			case GlyphSortType::BACK_TO_FRONT: 
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
				break;
			case GlyphSortType::TEXTURE: 
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
				break;
		}
		
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
	{
		return a->depth < b->depth;
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
	{
		return a->depth > b->depth;
	}

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
	{
		return a->texture < b->texture;
	}
}
