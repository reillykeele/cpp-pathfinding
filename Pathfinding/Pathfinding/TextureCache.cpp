#include "TextureCache.h"

#include "Errors.h"
#include "ImageLoader.h"

TextureCache::TextureCache()
{
}

TextureCache::~TextureCache()
{
}

GLTexture TextureCache::getTexture(std::string texturePath)
{
	auto textureIter = _textureMap.find(texturePath);
	if (textureIter == _textureMap.end())
	{
		auto newTexture = ImageLoader::loadPNG(texturePath);
		_textureMap.insert(make_pair(texturePath, newTexture));

		return newTexture;
	}

	return textureIter->second;

}
