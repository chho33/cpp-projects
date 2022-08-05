#pragma once

enum class BlockType {
	None,
	O,
	I,
	L,
	J,
	Z,
	S,
	T,
	Size = T
};


enum class Action {
	Hold,
	Down,
	Left,
	Right,
	Rotate,
	ToButtom,
	Pause
};


class Sprites {
private:
	sf::Texture blue, green, light_blue, orange, purple, red, yellow;

	std::string getBlockImgPath(std::string& color);

	void loadTexture(sf::Texture& texture, std::string&& color);

	void loadAllTexture();

	std::unordered_map<BlockType, sf::Sprite> data;

public:
	Sprites();

	std::unordered_map<BlockType, sf::Sprite> getData();
};


class Block {
private:
	sf::Vector2i grid;

protected:
	std::vector<std::unordered_map<int, std::vector<int>>> shapes;

	std::unordered_map<int, std::vector<int>> shape;

	BlockType type;

	int shapeIndex = 0;

	void initShape();

public:
	virtual void initShapes() = 0;

	Block(BlockType type, const sf::Vector2i& grid);

	sf::Vector2i getGrid();

	void setGrid(int x, int y);

	void increGridY(int amount);

	void increGridX(int amount);

	std::unordered_map<int, std::vector<int>> getShape();

	BlockType getType();

	void rotate();

	void rollBackRotate();

};


class O : public Block {
private:
	void initShapes();
public:
	O(BlockType type, const sf::Vector2i& grid);
};


class I : public Block {
private:
	void initShapes();

public:
	I(BlockType type, const sf::Vector2i& grid);
};


class J : public Block {
private:
	void initShapes();

public:
	J(BlockType type, const sf::Vector2i& grid);
};


class L : public Block {
private:
	void initShapes();

public:
	L(BlockType type, const sf::Vector2i& grid);
};


class Z : public Block {
private:
	void initShapes();

public:
	Z(BlockType type, const sf::Vector2i& grid);
};


class S : public Block {
private:
	void initShapes();

public:
	S(BlockType type, const sf::Vector2i& grid);
};


class T : public Block {
private:
	void initShapes();

public:
	T(BlockType type, const sf::Vector2i& grid);
};


class Board {
private:
	int width, height;
	sf::Sprite backgroundSprite;
	std::vector<std::vector<BlockType>> filled;
	std::vector<int> curButtom;
	std::vector<int> filledSizePerRow;
	sf::Texture blue, green, light_blue, orange, purple, red, yellow, backgroundTexture;
	std::unordered_map<BlockType, sf::Sprite> SpriteMap;

	std::string getBlockImgPath(std::string& color);

	void loadTexture(sf::Texture& texture, std::string&& color);

	void loadAllTexture();

	void setBackground(const std::string& backgroundFilePath);

public:
	Board(int width, int height, const std::string& backgroundFilePath);

	sf::Vector2u getGridSize();

	int getWidth();

	int getHeight();

	sf::Sprite getBgSprite();

	BlockType getFilledType(int row, int col);

	BlockType getFilledType(sf::Vector2i& blockGrid);

	void setFilledType(int row, int col, BlockType type);

	void setFilledType(sf::Vector2i& blockGrid, BlockType type);

	void setShapeFilledType(const std::unique_ptr<Block>& block_ptr);

	void cleanRowFilled(int row);

	void cleanFilled();

	bool rowIsFull(int row);

	bool rowIsEmpty(int row);

	int getRowFilledSize(int row);

	void setRowFilledSize(int row, int size);

	void increRowFilledSize(int row, int amount);

	int getButtom(int col);

	void setButtom(int row, int col);

	void resetButtom();

	void copyRow(int sourceRow, int targetRow);

	bool collideWithBoundary(int row, int col);

	bool shapeCollideWithBoundary(const std::unique_ptr<Block>& block_ptr);

	sf::Vector2f gridToPosition(unsigned int row, unsigned int col);

	void drawFilled(sf::RenderWindow& window);

	void drawBlock(std::unique_ptr<Block>& block_ptr, sf::RenderWindow& window);

	void FindButtoms(std::unique_ptr<Block>& block_ptr);

	void eliminateRowsIfNeeded();
};


class Game {
private:
	Board board;
	sf::Vector2i initGrid;
	Action action = Action::Hold;
	float frameFreq;
	bool paused = false;
	bool dead = false;
	std::unique_ptr<Block> block_ptr;

public:
	Game(Board& board, float frameFreq);

	auto genRandomBlock(sf::Vector2i& grid);

	void resetBlockAction();

	void setBlockAction();

	void setSpeed();

	void handleEvent(sf::RenderWindow& window);

	void timeToFallDown(sf::Clock& clock);

	void doAction(std::unique_ptr<Block>& block_ptr);

	void rollBackAction(std::unique_ptr<Block>& block_ptr);

	bool Paused();

	void togglePause();

	bool Dead();

	void setDead();

	void toggleDead();

	bool checkIfDead(std::unique_ptr<Block>& block_ptr);

	void renderDead(sf::RenderWindow& window);

	void resetGame();

	void sleep(float second);

	void run();
};