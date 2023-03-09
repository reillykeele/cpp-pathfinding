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

		for (auto& glyph : _glyphs)
			delete glyph;

		_renderBatches.clear();
		_glyphs.clear();
	} 

	void SpriteBatch::end()
	{
		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color)
	{
		Glyph* newGlyph = new Glyph;
		newGlyph->texture = texture;
		newGlyph->depth = depth;

		newGlyph->topLeft.color = color;
		newGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		newGlyph->bottomLeft.color = color;
		newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
		newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

		newGlyph->bottomRight.color = color;
		newGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		newGlyph->topRight.color = color;
		newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		_glyphs.push_back(newGlyph);
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
		_renderBatches.emplace_back(offset, 6, _glyphs[0]->texture);
		vertices[currVertex++] = _glyphs[0]->topLeft;
		vertices[currVertex++] = _glyphs[0]->bottomLeft;
		vertices[currVertex++] = _glyphs[0]->bottomRight;
		vertices[currVertex++] = _glyphs[0]->bottomRight;
		vertices[currVertex++] = _glyphs[0]->topRight;
		vertices[currVertex++] = _glyphs[0]->topLeft;
		offset += 6;

		for (int currGlyph = 1; currGlyph < _glyphs.size(); currGlyph++)
		{
			if (_glyphs[currGlyph]->texture != _glyphs[currGlyph - 1]->texture)
				_renderBatches.emplace_back(offset, 6, _glyphs[currGlyph]->texture);
			else
				_renderBatches.back().numVertices += 6;

			vertices[currVertex++] = _glyphs[currGlyph]->topLeft;
			vertices[currVertex++] = _glyphs[currGlyph]->bottomLeft;
			vertices[currVertex++] = _glyphs[currGlyph]->bottomRight;
			vertices[currVertex++] = _glyphs[currGlyph]->bottomRight;
			vertices[currVertex++] = _glyphs[currGlyph]->topRight;
			vertices[currVertex++] = _glyphs[currGlyph]->topLeft;
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
				std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
				break;
			case GlyphSortType::BACK_TO_FRONT: 
				std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
				break;
			case GlyphSortType::TEXTURE: 
				std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
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
