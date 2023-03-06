#pragma once
#include "TextureCache.h"
namespace SDLEngine
{

	static class ResourceManager
	{
	public:
		static GLTexture getTexture(std::string texturePath);

	private:
		static TextureCache _textureCache;
	};

}