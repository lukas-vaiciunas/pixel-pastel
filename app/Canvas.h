#pragma once

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <queue>
#include <unordered_set>

class Canvas : public sf::Drawable
{
public:
	Canvas(const sf::Vector2u &size);

	void draw(
		sf::RenderTarget &target,
		sf::RenderStates states) const override;

	void load(const std::string &filePath);
	void save(const std::string &filePath);

	void toggleGrid();

	void fill(
		const sf::Vector2u &position,
		const sf::Color &color);

	void erase(const sf::Vector2u &position);

	void setColor(
		const sf::Vector2u &position,
		const sf::Color &color);

	const sf::Color &getColor(const sf::Vector2u &position) const;
	const sf::Vector2u &getSize() const;
	unsigned int getCellSize() const;
private:
	sf::VertexArray outlineVertices_;
	sf::VertexArray transparencyVertices_;
	sf::VertexArray vertices_;
	sf::VertexArray gridVertices_;

	sf::Texture transparencyTexture_;

	sf::Vector2u size_;

	unsigned int cellSize_;

	bool isGridActive_;

	void init_();
	void init_(const std::vector<unsigned char> &colors);
	void initCanvas_();
	void initCanvas_(const std::vector<unsigned char> &colors);
	void initCanvasHelper_(unsigned int x, unsigned int y, const sf::Color &color);
	void initOutline_();
	void initGrid_();

	void setColor_(
		unsigned int base,
		const sf::Color &color);

	void fillHelper_(
		unsigned int x, unsigned int y,
		const sf::Color &replaceColor,
		std::queue<sf::Vector2u> &queue,
		std::unordered_set<unsigned int> &visitedHashes) const;

	unsigned int spatialHash_(const sf::Vector2u &position) const;
	unsigned int spatialHash_(unsigned int x, unsigned int y) const;
};