#include "Canvas.h"
#include "FreeFunctions.h"
#include "LodePNG.h"
#include <SFML/Graphics/RenderTarget.hpp>

Canvas::Canvas(const sf::Vector2u &size) :
	sf::Drawable(),
	outlineVertices_(sf::PrimitiveType::Triangles),
	transparencyVertices_(sf::PrimitiveType::Triangles),
	vertices_(sf::PrimitiveType::Triangles),
	gridVertices_(sf::PrimitiveType::Triangles),
	transparencyTexture_(),
	size_(size),
	cellSize_(8),
	isGridActive_(false)
{
	if (!transparencyTexture_.loadFromFile("./assets/textures/transparency.png"))
	{
		fprintf(stderr, "Failed to open \"./assets/textures/transparency.png\" in Canvas::Canvas\n");
	}

	this->init_();
}

void Canvas::draw(
	sf::RenderTarget &target,
	sf::RenderStates states) const
{
	sf::RenderStates transparencyStates = states;
	transparencyStates.texture = &transparencyTexture_;

	target.draw(outlineVertices_, states);
	target.draw(transparencyVertices_, transparencyStates);
	target.draw(vertices_, states);

	if (isGridActive_)
	{
		target.draw(gridVertices_, states);
	}
}

void Canvas::load(const std::string &filePath)
{
	std::vector<unsigned char> buffer;

	lodepng::decode(buffer, size_.x, size_.y, filePath);

	this->init_(buffer);
}

void Canvas::save(const std::string &filePath)
{
	std::vector<unsigned char> buffer(size_.x * size_.y * 4, 0);

	for (unsigned int y = 0; y < size_.y; ++y)
	{
		for (unsigned int x = 0; x < size_.x; ++x)
		{
			const size_t base = this->spatialHash_(x, y);
			const size_t bufferBase = base * 4;
			const size_t vertexBase = base * 6;

			buffer[bufferBase] = vertices_[vertexBase].color.r;
			buffer[bufferBase + 1] = vertices_[vertexBase].color.g;
			buffer[bufferBase + 2] = vertices_[vertexBase].color.b;
			buffer[bufferBase + 3] = vertices_[vertexBase].color.a;
		}
	}

	lodepng::encode(filePath, buffer, size_.x, size_.y);
}

void Canvas::toggleGrid()
{
	isGridActive_ = !isGridActive_;
}

void Canvas::fill(
	const sf::Vector2u &position,
	const sf::Color &color)
{
	std::queue<sf::Vector2u> queue;
	std::unordered_set<unsigned int> visitedHashes;

	const sf::Color replaceColor = vertices_[static_cast<size_t>(this->spatialHash_(position)) * 6].color;

	queue.push(position);
	visitedHashes.emplace(this->spatialHash_(position));

	while (!queue.empty())
	{
		sf::Vector2u curr = queue.front();
		queue.pop();

		this->setColor(curr, color);

		if (curr.x > 0)
		{
			this->fillHelper_(curr.x - 1, curr.y, replaceColor, queue, visitedHashes);
		}
		if (curr.x < size_.x - 1)
		{
			this->fillHelper_(curr.x + 1, curr.y, replaceColor, queue, visitedHashes);
		}
		if (curr.y > 0)
		{
			this->fillHelper_(curr.x, curr.y - 1, replaceColor, queue, visitedHashes);
		}
		if (curr.y < size_.y - 1)
		{
			this->fillHelper_(curr.x, curr.y + 1, replaceColor, queue, visitedHashes);
		}
	}
}

void Canvas::erase(const sf::Vector2u &position)
{
	this->setColor(
		position,
		sf::Color(255, 255, 255, 0));
}

void Canvas::setColor(
	const sf::Vector2u &position,
	const sf::Color &color)
{
	this->setColor_(this->spatialHash_(position) * 6, color);
}

const sf::Color &Canvas::getColor(const sf::Vector2u &position) const
{
	return vertices_[static_cast<size_t>(this->spatialHash_(position)) * 6].color;
}

const sf::Vector2u &Canvas::getSize() const
{
	return size_;
}

unsigned int Canvas::getCellSize() const
{
	return cellSize_;
}

void Canvas::init_()
{
	this->initCanvas_();
	this->initOutline_();
	this->initGrid_();
}

void Canvas::init_(const std::vector<unsigned char> &colors)
{
	this->initCanvas_(colors);
	this->initOutline_();
	this->initGrid_();
}

void Canvas::initCanvas_()
{
	vertices_.clear();
	transparencyVertices_.clear();

	const sf::Color fillColor = sf::Color(255, 255, 255, 0);

	for (unsigned int y = 0; y < size_.y; ++y)
	{
		for (unsigned int x = 0; x < size_.x; ++x)
		{
			this->initCanvasHelper_(x, y, fillColor);
		}
	}
}

void Canvas::initCanvas_(const std::vector<unsigned char> &colors)
{
	vertices_.clear();
	transparencyVertices_.clear();

	for (unsigned int y = 0; y < size_.y; ++y)
	{
		for (unsigned int x = 0; x < size_.x; ++x)
		{
			const unsigned int colorBase = this->spatialHash_(x, y) * 4;

			this->initCanvasHelper_(
				x, y,
				sf::Color(
					colors[colorBase],
					colors[colorBase + 1],
					colors[colorBase + 2],
					colors[colorBase + 3]));
		}
	}
}

void Canvas::initCanvasHelper_(unsigned int x, unsigned int y, const sf::Color &color)
{
	const float cellSize = static_cast<float>(cellSize_);

	const sf::Vector2f pixelPosition(
		x * cellSize,
		y * cellSize);

	FreeFunctions::addQuad(
		vertices_,
		pixelPosition,
		sf::Vector2f(pixelPosition.x + cellSize, pixelPosition.y + cellSize),
		color);

	// Transparency

	transparencyVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				pixelPosition.x,
				pixelPosition.y),
			sf::Vector2f(0.0f, 0.0f)));

	transparencyVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				pixelPosition.x + cellSize,
				pixelPosition.y),
			sf::Vector2f(cellSize, 0.0f)));

	transparencyVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				pixelPosition.x,
				pixelPosition.y + cellSize),
			sf::Vector2f(0.0f, cellSize)));

	transparencyVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				pixelPosition.x,
				pixelPosition.y + cellSize),
			sf::Vector2f(0.0f, cellSize)));

	transparencyVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				pixelPosition.x + cellSize,
				pixelPosition.y),
			sf::Vector2f(cellSize, 0.0f)));

	transparencyVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				pixelPosition.x + cellSize,
				pixelPosition.y + cellSize),
			sf::Vector2f(cellSize, cellSize)));
}

void Canvas::initOutline_()
{
	outlineVertices_.clear();

	const sf::Color color(0, 0, 0);
	const float thickness = 1.0f;
	unsigned int width = size_.x * cellSize_;
	unsigned int height = size_.y * cellSize_;
	
	// Top

	FreeFunctions::addQuad(
		outlineVertices_,
		sf::Vector2f(-thickness, -thickness),
		sf::Vector2f(width + thickness, thickness),
		color);

	// Bottom

	FreeFunctions::addQuad(
		outlineVertices_,
		sf::Vector2f(-thickness, height - thickness),
		sf::Vector2f(width + thickness, height + thickness),
		color);

	// Left

	FreeFunctions::addQuad(
		outlineVertices_,
		sf::Vector2f(-thickness, thickness),
		sf::Vector2f(thickness, height - thickness),
		color);

	// Right

	FreeFunctions::addQuad(
		outlineVertices_,
		sf::Vector2f(width - thickness, thickness),
		sf::Vector2f(width + thickness, height - thickness),
		color);
}

void Canvas::initGrid_()
{
	gridVertices_.clear();

	const sf::Color fillColor(0, 0, 0);
	const float thickness = 1.0f;
	const float halfThickness = thickness * 0.5f;
	const unsigned int cellWidth = 1;
	const unsigned int cellHeight = 1;

	sf::Vector2f gridPixelSize(
		static_cast<float>(size_.x * cellSize_),
		static_cast<float>(size_.y * cellSize_));

	for (unsigned int x = cellWidth; x < size_.x; x += cellWidth)
	{
		FreeFunctions::addQuad(
			gridVertices_,
			sf::Vector2f(
				x * cellSize_ - halfThickness,
				0.0f),
			sf::Vector2f(
				x * cellSize_ + halfThickness,
				gridPixelSize.y),
			fillColor);
	}

	for (unsigned int y = cellHeight; y < size_.y; y += cellHeight)
	{
		FreeFunctions::addQuad(
			gridVertices_,
			sf::Vector2f(
				0.0f,
				y * cellSize_ - halfThickness),
			sf::Vector2f(
				gridPixelSize.x,
				y * cellSize_ + halfThickness),
			fillColor);
	}
}

void Canvas::setColor_(
	unsigned int base,
	const sf::Color &color)
{
	if (vertices_[base].color == color)
	{
		return;
	}

	for (unsigned int i = base; i < base + 6; ++i)
	{
		vertices_[i].color = color;
	}
}

void Canvas::fillHelper_(
	unsigned int x, unsigned int y,
	const sf::Color &replaceColor,
	std::queue<sf::Vector2u> &queue,
	std::unordered_set<unsigned int> &visitedHashes) const
{
	unsigned int hash = this->spatialHash_(x, y);

	if (visitedHashes.find(hash) == visitedHashes.end())
	{
		const sf::Vector2u next(x, y);

		if (vertices_[static_cast<size_t>(hash) * 6].color == replaceColor)
		{
			queue.push(next);
			visitedHashes.emplace(hash);
		}
	}
}

unsigned int Canvas::spatialHash_(const sf::Vector2u &position) const
{
	return this->spatialHash_(position.x, position.y);
}

unsigned int Canvas::spatialHash_(unsigned int x, unsigned int y) const
{
	return x + y * size_.x;
}