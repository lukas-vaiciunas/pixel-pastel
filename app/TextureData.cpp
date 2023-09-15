#include "TextureData.h"
#include <fstream>
#include <sstream>

TextureData::TextureData() :
	textures_()
{
	this->load_("data/textures.txt");
}

TextureData &TextureData::getInstance()
{
	static TextureData instance;
	return instance;
}

const sf::Texture &TextureData::getTexture(TextureId id) const
{
	return textures_.at(id);
}

void TextureData::load_(const std::string &filePath)
{
	std::ifstream in(filePath);

	if (!in.is_open())
	{
		fprintf(stderr, "Failed to open \"%s\" in TextureData::load_\n", filePath.c_str());
		return;
	}

	std::string line;
	std::stringstream ss;

	while (std::getline(in, line))
	{
		int textureId;
		std::string textureFilePath;

		ss.clear();
		ss.str(line);

		ss >> textureId >> textureFilePath;

		sf::Texture texture;
		texture.loadFromFile(textureFilePath);

		textures_.emplace(textureId, std::move(texture));
	}

	in.close();
}