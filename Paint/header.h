#pragma once

enum class BrushColor {
	Black,
	Red,
	Green,
	Blue,
	Yellow,
	Magenta,
	Cyan,
	Size
};

std::unordered_map<BrushColor, sf::Color> brushColorMap{
	{BrushColor::Black, sf::Color::Black},
	{BrushColor::Red, sf::Color::Red},
	{BrushColor::Green, sf::Color::Green},
	{BrushColor::Blue, sf::Color::Blue},
	{BrushColor::Yellow, sf::Color::Yellow},
	{BrushColor::Magenta, sf::Color::Magenta},
	{BrushColor::Cyan, sf::Color::Cyan}
};

enum class BrushSize {
	Small,
	Medium,
	Large
};


enum class BrushShape {
	Circle,
	Square
};


struct BrushArgs {
	BrushColor brushColor;
	BrushShape brushShape;
	BrushSize brushSize;
	int posX;
	int posY;
};

class Brush {
private:
	BrushArgs args;

	std::unordered_map<BrushSize, int> brushSizeMap{
		{BrushSize::Small, 5},
		{BrushSize::Medium, 8},
		{BrushSize::Large, 12}
	};

public:
	Brush(BrushArgs&& args);

	int getSize();

	void setSize(BrushSize size);

	sf::Color getColor();

	void setColor(BrushColor color);

	void setPosition(int x, int y);

	std::unique_ptr<sf::Shape> getBrushShapePtr(sf::Vector2f&& pos);

	void setShape(BrushShape shape);
};


struct CanvasArgs {
	int windowWidth;
	int windowHeight;
	int drawAreaWidth;
	int drawAreaHeight;
	int drawAreaWidthOffset;
	int drawAreaHeightOffset;
	int bgStartX;
	int bgStartY;
	std::string bgFile;
};

class Canvas {
private:
	CanvasArgs args;
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	sf::Sprite drawAreaSprite;
	sf::Vector2f drawAreaOffset;

	void loadBackground();

public:
	Canvas(CanvasArgs&& args);

	sf::Sprite getBackground();

	sf::Sprite getDrawAreaSprite();

	sf::Vector2f getDrawAreaOffset();

	int getDrawAreaWidthOffset();

	int getDrawAreaHeightOffset();

	void windowSetup(sf::RenderWindow& window);

	void drawAreaSetup(sf::RenderTexture& drawArea);

	int getWindowWidth();

	int getWindowHeight();
};


struct SwatchArgs {
	float swatchWidth;
	float swatchHeight;
	float swatchMargin;
};

class Swatch {
private:
	std::vector<sf::RectangleShape> colors;
	std::vector<std::pair<int, int>> positions;
	int unitWidth, unitHeight;

public:
	Swatch(SwatchArgs&& args);

	std::vector<sf::RectangleShape> getColors();

	std::vector<std::pair<int, int>> getPositions();

	int getUnitWidth();

	int getUnitHeight();
};


class Control {
private:
	Canvas canvas;
	Brush brush;
	Swatch swatch;

public:
	Control(Canvas&& canvas, Brush&& brush, Swatch&& swatch);

	void saveImage(std::string&& filePath, sf::RenderTexture& drawArea);

	sf::Vector2f getCursorPos(sf::RenderWindow& window);

	sf::Vector2f getCursorPos(sf::RenderWindow& window, sf::Vector2f&& offset);

	void setTextToCenter(sf::Text& text);

	void promptText(std::string&& text, sf::RenderWindow& window);

	int findColorIndex(sf::Vector2i&& mousePos);

	void handleEvent(sf::RenderWindow& window, sf::RenderTexture& drawArea);

	void handleCanvasDrawing(sf::RenderWindow& window, sf::RenderTexture& drawArea);

	void drawSwatch(sf::RenderWindow& window);

	void handleWindowDrawing(sf::RenderWindow& window);

	int run();
};