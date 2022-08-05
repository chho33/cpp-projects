#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iostream>
#include "header.h"

// Class Brush

Brush::Brush(BrushArgs&& args): args(args) {}

int Brush::getSize() {
	return brushSizeMap[args.brushSize];
}

void Brush::setSize(BrushSize size) {
	args.brushSize = size;
}

sf::Color Brush::getColor() {
	return brushColorMap[args.brushColor];
}

void Brush::setColor(BrushColor color) {
	args.brushColor = color;
}

void Brush::setPosition(int x, int y) {
	args.posX = x;
	args.posY = y;
}

std::unique_ptr<sf::Shape> Brush::getBrushShapePtr(sf::Vector2f&& pos) {
	std::unique_ptr<sf::Shape> ptr;

	switch (args.brushShape) {
	case BrushShape::Circle:
		ptr = std::make_unique<sf::CircleShape>(getSize());
		break;
	case BrushShape::Square:
		ptr = std::make_unique<sf::RectangleShape>(sf::Vector2f(static_cast<float>(getSize() * 2), static_cast<float>(getSize() * 2)));
		break;
	}

	ptr->setFillColor(getColor());
	ptr->setPosition(pos);

	return ptr;
}

void Brush::setShape(BrushShape shape) {
	args.brushShape = shape;
}


// Class Canvas

void Canvas::loadBackground() {
	backgroundTexture.loadFromFile(args.bgFile);
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setPosition(args.bgStartX, args.bgStartY);
}

Canvas::Canvas(CanvasArgs&& args): args(args) {
	loadBackground();
	drawAreaOffset = sf::Vector2f((float) args.drawAreaWidthOffset, (float) args.drawAreaHeightOffset);
}

sf::Sprite Canvas::getBackground() {
	return backgroundSprite;
}

sf::Sprite Canvas::getDrawAreaSprite() {
	return drawAreaSprite;
}

sf::Vector2f Canvas::getDrawAreaOffset() {
	return drawAreaOffset;
}

int Canvas::getDrawAreaWidthOffset() {
	return args.drawAreaWidthOffset;
}

int Canvas::getDrawAreaHeightOffset() {
	return args.drawAreaHeightOffset;
}

void Canvas::windowSetup(sf::RenderWindow& window) {
	window.create(sf::VideoMode(args.windowWidth, args.windowHeight), "Paint");
}

void Canvas::drawAreaSetup(sf::RenderTexture& drawArea) {
	drawArea.create(args.drawAreaWidth, args.drawAreaHeight);
	drawArea.clear(sf::Color::White);
	drawArea.display();

	drawAreaSprite.setTexture(drawArea.getTexture());
	drawAreaSprite.setPosition(args.drawAreaWidthOffset, args.drawAreaHeightOffset);
}

int Canvas::getWindowWidth() {
	return args.windowWidth;
}

int Canvas::getWindowHeight() {
	return args.windowHeight;
}


// Class Swatch

Swatch::Swatch(SwatchArgs&& args) {
	for (int i = 0; i < (int)BrushColor::Size; i++) {
		sf::RectangleShape shape({ args.swatchWidth, args.swatchHeight });
		shape.setOutlineColor(sf::Color(230, 230, 230));
		shape.setOutlineThickness(2);
		shape.setFillColor(brushColorMap[static_cast<BrushColor>(i)]);
		shape.setPosition(args.swatchWidth * (i+1), args.swatchMargin);
		colors.emplace_back(shape);
		positions.emplace_back(std::make_pair(shape.getGlobalBounds().left, shape.getGlobalBounds().top));
	}

	unitWidth = colors[0].getGlobalBounds().width;
	unitHeight = colors[0].getGlobalBounds().height;
}

std::vector<sf::RectangleShape> Swatch::getColors() {
	return colors;
}

std::vector<std::pair<int, int>> Swatch::getPositions() {
	return positions;
}

int Swatch::getUnitWidth() {
	return unitWidth;
}

int Swatch::getUnitHeight() {
	return unitHeight;
}


// Class Control


Control::Control(Canvas&& canvas, Brush&& brush, Swatch&& swatch): canvas(canvas), brush(brush), swatch(swatch) {
}

void Control::saveImage(std::string&& filePath, sf::RenderTexture& drawArea) {
	drawArea.getTexture().copyToImage().saveToFile(filePath);
};

sf::Vector2f Control::getCursorPos(sf::RenderWindow& window) {
	sf::Vector2f pos = (sf::Vector2f) sf::Mouse::getPosition(window);
	pos.x -= brush.getSize();
	pos.y -= brush.getSize();
	return pos;
}

sf::Vector2f Control::getCursorPos(sf::RenderWindow& window, sf::Vector2f&& offset) {
	sf::Vector2f pos = getCursorPos(window);
	pos.x -= offset.x;
	pos.y -= offset.y;
	return pos;
}

void Control::setTextToCenter(sf::Text& text) {
	sf::FloatRect textBound = text.getGlobalBounds();
	text.setPosition(static_cast<float>((canvas.getWindowWidth() - textBound.width) / 2),
		static_cast<float>((canvas.getWindowHeight() - textBound.height) / 2));
}

void Control::promptText(std::string&& text, sf::RenderWindow& window) {
	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) throw std::invalid_argument("font file doesn't exist!");

	int alpha = 255;
	sf::Text displayText(text, font);

	displayText.setCharacterSize(50);
	displayText.setStyle(sf::Text::Bold);
	displayText.setFillColor(sf::Color(0, 0, 0, alpha));
	setTextToCenter(displayText);

	float showTime = 0.5f;
	while (alpha--) {
		sf::sleep(sf::seconds(showTime / 255));
		window.clear(sf::Color::White);
		window.draw(displayText);
		window.display();
		displayText.setFillColor(sf::Color(0, 0, 0, alpha));
	}
}

int Control::findColorIndex(sf::Vector2i&& mousePos) {
	std::vector<std::pair<int, int>> positions = swatch.getPositions();

	if (mousePos.y < positions[0].second ||
		mousePos.y > positions[0].second + swatch.getUnitHeight() ||
		mousePos.x > positions.back().first + swatch.getUnitWidth()) return -1;

	int left = 0, right = positions.size() - 1;

	while (left <= right) {
		int mid = (right - left) / 2 + left;
		if (mousePos.x >= positions[mid].first && mousePos.x <= positions[mid].first + swatch.getUnitWidth()) return mid;
		else if (mousePos.x > positions[mid].first) left = mid + 1;
		else right = mid - 1;
	}

	return right;
}

void Control::handleEvent(sf::RenderWindow& window, sf::RenderTexture& drawArea) {
	sf::Event event;

	if (window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code) {
			case sf::Keyboard::S:
				saveImage("test.png", drawArea);
				promptText("Image Saved!", window);
				break;
			case sf::Keyboard::Num1:
				brush.setSize(BrushSize::Small);
				break;
			case sf::Keyboard::Num2:
				brush.setSize(BrushSize::Medium);
				break;
			case sf::Keyboard::Num3:
				brush.setSize(BrushSize::Large);
				break;
			case sf::Keyboard::R:
				brush.setShape(BrushShape::Square);
				break;
			case sf::Keyboard::C:
				brush.setShape(BrushShape::Circle);
				break;
			case sf::Keyboard::Return:
				drawArea.clear(sf::Color::White);
				break;
			}
		case sf::Event::MouseButtonPressed:
		{
			int idx = findColorIndex(sf::Mouse::getPosition(window));
			if (idx != -1) {
				brush.setColor(static_cast<BrushColor>(findColorIndex(sf::Mouse::getPosition(window))));
			}
		}
		break;
		}
	}
}

void Control::handleCanvasDrawing(sf::RenderWindow& window, sf::RenderTexture& drawArea) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		drawArea.draw(*brush.getBrushShapePtr(getCursorPos(window, canvas.getDrawAreaOffset())));
	}
}

void Control::drawSwatch(sf::RenderWindow& window) {
	for (const auto& c : swatch.getColors()) {
		window.draw(c);
	}
}

void Control::handleWindowDrawing(sf::RenderWindow& window) {
	window.clear(sf::Color::White);
	drawSwatch(window);
	window.draw(canvas.getBackground());
	window.draw(canvas.getDrawAreaSprite());
	window.draw(*brush.getBrushShapePtr(getCursorPos(window)));
	window.display();
}

int Control::run() {

	sf::RenderWindow window;
	canvas.windowSetup(window);
	//window.setFramerateLimit(30);

	sf::RenderTexture drawArea;
	canvas.drawAreaSetup(drawArea);
		
	while (window.isOpen()) {
		handleEvent(window, drawArea);
		handleCanvasDrawing(window, drawArea);
		handleWindowDrawing(window);
	}

	return EXIT_SUCCESS;
}


int main() {
	CanvasArgs cargs;
	cargs.windowWidth = 642;
	cargs.windowHeight = 640;
	cargs.drawAreaWidth = 600;
	cargs.drawAreaHeight = 560;
	cargs.drawAreaWidthOffset = 20;
	cargs.drawAreaHeightOffset = 60;
	cargs.bgStartX = 0;
	cargs.bgStartY = 40;
	cargs.bgFile = "background.png";

	Canvas canvas{ std::move(cargs) };


	BrushArgs bargs;
	bargs.posX = 10;
	bargs.posY = 10;
	bargs.brushColor = BrushColor::Black;
	bargs.brushShape = BrushShape::Circle;
	bargs.brushSize = BrushSize::Medium;

	Brush brush{ std::move(bargs) };


	SwatchArgs sargs;
	sargs.swatchWidth = 25.f;
	sargs.swatchHeight = 25.f;
	sargs.swatchMargin = 10.f;

	Swatch swatch{ std::move(sargs) };


	Control control{ std::move(canvas), std::move(brush), std::move(swatch) };

	control.run();
}