#include "Canvas.h"
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

	this->init_();

	const unsigned int numColors = size_.x * size_.y;

	for (unsigned int i = 0; i < numColors; ++i)
	{
		const unsigned int bufferBase = i * 4;
		const unsigned int vertexBase = i * 6;

		this->setColor_(
			vertexBase,
			sf::Color(
				buffer[bufferBase],
				buffer[bufferBase + 1],
				buffer[bufferBase + 2],
				buffer[bufferBase + 3]));
	}
}

void Canvas::save(const std::string &filePath)
{
	std::vector<unsigned char> buffer(size_.x * size_.y * 4, 0);

	for (size_t y = 0; y < size_.y; ++y)
	{
		for (size_t x = 0; x < size_.x; ++x)
		{
			const size_t base = x + y * size_.x;
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
	this->setColor_(this->getBase_(position), color);
}

const sf::Color &Canvas::getColor(const sf::Vector2u &position) const
{
	return vertices_[this->getBase_(position)].color;
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
	vertices_.clear();
	transparencyVertices_.clear();

	const sf::Color fillColor = sf::Color(255, 255, 255, 0);

	for (unsigned int y = 0; y < size_.y; ++y)
	{
		for (unsigned int x = 0; x < size_.x; ++x)
		{
			float cellSize = static_cast<float>(cellSize_);

			const sf::Vector2f position(
				x * cellSize,
				y * cellSize);

			this->addQuad_(
				vertices_,
				position,
				sf::Vector2f(position.x + cellSize, position.y + cellSize),
				fillColor);

			// Transparency

			transparencyVertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x,
						position.y),
					sf::Vector2f(0.0f, 0.0f)));

			transparencyVertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x + cellSize,
						position.y),
					sf::Vector2f(cellSize, 0.0f)));

			transparencyVertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x,
						position.y + cellSize),
					sf::Vector2f(0.0f, cellSize)));

			transparencyVertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x,
						position.y + cellSize),
					sf::Vector2f(0.0f, cellSize)));

			transparencyVertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x + cellSize,
						position.y),
					sf::Vector2f(cellSize, 0.0f)));

			transparencyVertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x + cellSize,
						position.y + cellSize),
					sf::Vector2f(cellSize, cellSize)));
		}
	}

	this->initOutline_();
	this->initGrid_();
}

void Canvas::initOutline_()
{
	outlineVertices_.clear();

	const sf::Color color(0, 0, 0);
	const float thickness = 1.0f;
	unsigned int width = size_.x * cellSize_;
	unsigned int height = size_.y * cellSize_;
	
	// Top

	this->addQuad_(
		outlineVertices_,
		sf::Vector2f(-thickness, -thickness),
		sf::Vector2f(width + thickness, thickness),
		color);

	// Bottom

	this->addQuad_(
		outlineVertices_,
		sf::Vector2f(-thickness, height - thickness),
		sf::Vector2f(width + thickness, height + thickness),
		color);

	// Left

	this->addQuad_(
		outlineVertices_,
		sf::Vector2f(-thickness, thickness),
		sf::Vector2f(thickness, height - thickness),
		color);

	// Right

	this->addQuad_(
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
		this->addQuad_(
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
		this->addQuad_(
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

void Canvas::addQuad_(
	sf::VertexArray &vertices,
	const sf::Vector2f &minPosition,
	const sf::Vector2f &maxPosition,
	const sf::Color &color)
{
	vertices.append(
		sf::Vertex(
			minPosition,
			color));

	vertices.append(
		sf::Vertex(
			sf::Vector2f(
				maxPosition.x,
				minPosition.y),
			color));

	vertices.append(
		sf::Vertex(
			sf::Vector2f(
				minPosition.x,
				maxPosition.y),
			color));

	vertices.append(
		sf::Vertex(
			sf::Vector2f(
				minPosition.x,
				maxPosition.y),
			color));

	vertices.append(
		sf::Vertex(
			sf::Vector2f(
				maxPosition.x,
				minPosition.y),
			color));

	vertices.append(
		sf::Vertex(
			maxPosition,
			color));
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

unsigned int Canvas::getBase_(const sf::Vector2u &position) const
{
	const unsigned int width = size_.x * 6;

	return position.x * 6 + position.y * width;
}