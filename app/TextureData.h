#pragma once

#include "TextureId.h"
#include <SFML/Graphics/Texture.hpp>
#include <unordered_map>

class TextureData
{
public:
	TextureData(const TextureData &) = delete;
	TextureData(TextureData &&) = delete;

	TextureData &operator=(const TextureData &) = delete;
	TextureData &operator=(TextureData &&) = delete;

	static TextureData &getInstance();

	const sf::Texture &getTexture(TextureId id) const;
private:
	std::unordered_map<TextureId, sf::Texture> textures_;

	TextureData();
	~TextureData() {}

	void load_(const std::string &filePath);
};