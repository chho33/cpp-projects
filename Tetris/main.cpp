#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <time.h>


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

	std::string getBlockImgPath(std::string& color) {
		std::string pathBath = "images/";
		return pathBath + color + ".png";
	}

	void loadTexture(sf::Texture& texture, std::string&& color) {
		std::string filePath = getBlockImgPath(color);
		if (!texture.loadFromFile(filePath)) throw std::invalid_argument("File doesn't exist!");
	}

	void loadAllTexture() {
		loadTexture(blue, "blue");
		loadTexture(green, "green");
		loadTexture(light_blue, "light_blue");
		loadTexture(orange, "orange");
		loadTexture(purple, "purple");
		loadTexture(red, "red");
		loadTexture(yellow, "yellow");
	}

	std::unordered_map<BlockType, sf::Sprite> data;

public:
	Sprites() {
		loadAllTexture();
		data = {
			{ BlockType::O, sf::Sprite(blue) },
			{ BlockType::I, sf::Sprite(green) },
			{ BlockType::J, sf::Sprite(light_blue) },
			{ BlockType::L, sf::Sprite(orange) },
			{ BlockType::Z, sf::Sprite(purple) },
			{ BlockType::S, sf::Sprite(red) },
			{ BlockType::T, sf::Sprite(yellow) },
		};
	}

	std::unordered_map<BlockType, sf::Sprite> getData() { return data; }
};

//std::unordered_map<BlockType, sf::Sprite> SpriteMap = Sprites().getData();


class Block {
private:
	sf::Vector2i grid;

protected:
	std::vector<std::unordered_map<int, std::vector<int>>> shapes;
	std::unordered_map<int, std::vector<int>> shape;
	BlockType type;
	int shapeIndex = 0;

	void initShape() {
		shapeIndex = rand() % shapes.size();
		shape = shapes[shapeIndex];
	}

public:
	Block(BlockType type, const sf::Vector2i &grid) : grid(grid), type(type) {

	}

	sf::Vector2i getGrid() {
		return grid;
	}

	void setGrid(int x, int y) {
		grid = sf::Vector2i(x, y);
	}

	void increGridY(int amount) {
		grid.y = grid.y + amount;
	}

	void increGridX(int amount) {
		grid.x = grid.x + amount;
	}

	std::unordered_map<int, std::vector<int>> getShape() {
		return shape;
	}

	BlockType getType() {
		return type;
	}

	void rotate() {
		shapeIndex = (shapeIndex + 1) % shapes.size();
		shape = shapes[shapeIndex];
	}

	void rollBackRotate() {
		shapeIndex = (shapeIndex - 1) % shapes.size();
		shape = shapes[shapeIndex];
	}

	//auto blockFactory(BlockType type, const sf::Vector2i &grid)->std::unique_ptr<Block>;

	virtual void initShapes() = 0;

};


class O : public Block {
private:
	void initShapes() {
		// { {col1: rows, ... }, ... }
		shapes = {
			{{0, {-1, 0}}, {-1, {-1, 0}}}
		};
	};
public:
	O(BlockType type, const sf::Vector2i& grid) : Block(type, grid) {
		initShapes();
		initShape();
	}
};


class I : public Block {
private:
	void initShapes () {
		shapes = {
			{ {0, {-1, 0, 1, 2}} },
			{ {-2, {-1}}, {-1, {-1}}, {0, {-1}}, {1, {-1}} }
		};
	};

public:
	I(BlockType type, const sf::Vector2i& grid) : Block(type, grid) {
		initShapes();
		initShape();
	}
};


class J : public Block {
private:
	void initShapes() {
		shapes = {
			{ {-1, {1}}, {0, {-1, 0, 1}} },
			{ {-1, {-1}}, {0, {-1}}, {1, {-1, 0}} },
			{ {0, {-1, 0, 1}}, {1, {-1}} },
			{ {-1, {-1, 0}}, {0, {0}}, {1, {0}} }
		};
	};

public:
	J(BlockType type, const sf::Vector2i& grid) : Block(type, grid) {
		initShapes();
		initShape();
	}
};


class L : public Block {
private:
	void initShapes() {
		shapes = {
		{ {-1, {-1}}, {0, {-1, 0, 1}} },
		{ {-1, {-1, 0}}, {0, {-1}}, {1, {-1}} },
		{ {0, {-1, 0, 1}}, {1, {1}} },
		{ {-1, {0}}, {0, {0}}, {1, {-1, 0}} }
		};
	};

public:
	L(BlockType type, const sf::Vector2i& grid) : Block(type, grid) {
		initShapes();
		initShape();
	}
};


class Z : public Block {
private:
	void initShapes() {
		shapes = {
			{ {-1, {0, 1}}, {0, {-1, 0}} },
			{ {-1, {-1}}, {0, {-1, 0}}, {1, {0}} }
		};
	};

public:
	Z(BlockType type, const sf::Vector2i& grid) : Block(type, grid) {
		initShapes();
		initShape();
	}
};


class S : public Block {
private:
	void initShapes() {
		shapes = {
			{ {-1, {-1, 0}}, {0, {0, 1}} },
			{ {-1, {0}}, {0, {-1, 0}}, {1, {-1}} }
		};
	};

public:
	S(BlockType type, const sf::Vector2i& grid) : Block(type, grid) {
		initShapes();
		initShape();
	}
};


class T : public Block {
private:
	void initShapes() {
		shapes = {
			{ {-1, {0}}, {0, {-1, 0, 1}} },
			{ {-1, {-1}}, {0, {-1, 0}}, {1, {-1}} },
			{ {0, {-1, 0, 1}}, {1, {0}} },
			{ {-1, {0}}, {0, {-1, 0}}, {1, {0}} }
		};
	};

public:
	T(BlockType type, const sf::Vector2i& grid) : Block(type, grid) {
		initShapes();
		initShape();
	}
};


auto blockFactory(BlockType type, const sf::Vector2i& grid) -> std::unique_ptr<Block>
{
	switch (type)
	{
	case BlockType::O:
		return std::make_unique<O>(type, grid);
	case BlockType::I:
		return std::make_unique<I>(type, grid);
	case BlockType::J:
		return std::make_unique<J>(type, grid);
	case BlockType::L:
		return std::make_unique<L>(type, grid);
	case BlockType::Z:
		return std::make_unique<Z>(type, grid);
	case BlockType::S:
		return std::make_unique<S>(type, grid);
	case BlockType::T:
		return std::make_unique<T>(type, grid);
	default:
		return nullptr;
	}
}


class Board {
private:
	int width, height;
	sf::Sprite backgroundSprite;
	std::vector<std::vector<BlockType>> filled;
	std::vector<int> curButtom;
	std::vector<int> filledSizePerRow;
	sf::Texture blue, green, light_blue, orange, purple, red, yellow, backgroundTexture;
	std::unordered_map<BlockType, sf::Sprite> SpriteMap;

	std::string getBlockImgPath(std::string& color) {
		std::string pathBath = "images/";
		return pathBath + color + ".png";
	}

	void loadTexture(sf::Texture& texture, std::string&& color) {
		std::string filePath = getBlockImgPath(color);
		if (!texture.loadFromFile(filePath)) throw std::invalid_argument("File doesn't exist!");
	}

	void loadAllTexture() {
		loadTexture(blue, "blue");
		loadTexture(green, "green");
		loadTexture(light_blue, "light_blue");
		loadTexture(orange, "orange");
		loadTexture(purple, "purple");
		loadTexture(red, "red");
		loadTexture(yellow, "yellow");
	}

	void setBackground(const std::string& backgroundFilePath) {
		if (!backgroundTexture.loadFromFile(backgroundFilePath)) throw std::invalid_argument("File doesn't exist!");
		backgroundTexture.setRepeated(true);
		backgroundSprite = sf::Sprite(backgroundTexture, sf::IntRect(0, 0, width * getGridSize().x, height * getGridSize().y));
	}

public:
	Board(int width, int height, const std::string& backgroundFilePath) : width(width), height(height) {
		filled = std::vector<std::vector<BlockType>>(height, std::vector<BlockType>(width, BlockType::None));
		curButtom = std::vector<int>(width, height);
		filledSizePerRow = std::vector<int>(height, 0);
		loadAllTexture();
		SpriteMap = {
			{ BlockType::O, sf::Sprite(blue) },
			{ BlockType::I, sf::Sprite(green) },
			{ BlockType::J, sf::Sprite(light_blue) },
			{ BlockType::L, sf::Sprite(orange) },
			{ BlockType::Z, sf::Sprite(purple) },
			{ BlockType::S, sf::Sprite(red) },
			{ BlockType::T, sf::Sprite(yellow) },
		};
		setBackground(backgroundFilePath);
	}

	sf::Vector2u getGridSize() {
		return blue.getSize();
	}

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

	sf::Sprite getBgSprite() {
		return backgroundSprite;
	}

	BlockType getFilledType(int row, int col) {
		return filled[row][col];
	}

	BlockType getFilledType(sf::Vector2i& blockGrid) {
		return filled[blockGrid.y][blockGrid.x];
	}

	void setFilledType(int row, int col, BlockType type) {
		filled[row][col] = type;
	}

	void setFilledType(sf::Vector2i& blockGrid, BlockType type) {
		filled[blockGrid.y][blockGrid.x] = type;
	}

	void setShapeFilledType(const std::unique_ptr<Block>& block_ptr) {
		sf::Vector2i curGrid = block_ptr->getGrid();

		for (auto &colRows : block_ptr->getShape()) {
			int deltaCol = colRows.first;
			for (auto& deltaRow : colRows.second) {
				int curRow = curGrid.y + deltaRow, curCol = curGrid.x + deltaCol;
				if (curRow > -1) {
					if (getFilledType(curRow, curCol) == BlockType::None) setFilledType(curRow, curCol, block_ptr->getType());
					if (curRow < getButtom(curCol)) setButtom(curRow, curCol);
					increRowFilledSize(curRow, 1);
				}
			}
		}
	}

	void cleanRowFilled(int row) {
		for (int col = 0; col < width; col++) {
			setFilledType(row, col, BlockType::None);
		}
		setRowFilledSize(row, 0);
	}

	void cleanFilled() {
		for (int row = 0; row < getHeight(); row++) {
			cleanRowFilled(row);
		}
	}

	bool rowIsFull(int row) {
		return filledSizePerRow[row] == width;
	}

	bool rowIsEmpty(int row) {
		return filledSizePerRow[row] == 0;
	}

	int getRowFilledSize(int row) {
		return filledSizePerRow[row];
	}

	void setRowFilledSize(int row, int size) {
		filledSizePerRow[row] = size;
	}

	void increRowFilledSize(int row, int amount) {
		filledSizePerRow[row] += amount;
	}

	int getButtom(int col) {
		return curButtom[col];
	}

	void setButtom(int row, int col) {
		curButtom[col] = row;
	}

	void resetButtom() {
		for (int col = 0; col < getWidth(); col++) curButtom[col] = getHeight();
	}

	void copyRow(int sourceRow, int targetRow) {
		for (int col = 0; col < getWidth(); col++) {
			setFilledType(targetRow, col, getFilledType(sourceRow, col));
		}
		setRowFilledSize(targetRow, getRowFilledSize(sourceRow));
	}

	bool collideWithBoundary(int row, int col) {
		if (row >= getHeight() ||
			col < 0 ||
			col >= getWidth() ||
			(row > -1 && getFilledType(row, col) != BlockType::None)
			) return true;
		return false;
	}

	bool shapeCollideWithBoundary(const std::unique_ptr<Block>& block_ptr) {
		for (auto& colRows : block_ptr->getShape()) {
			int deltaCol = colRows.first;
			for (auto& deltaRow : colRows.second) {
				int curRow = block_ptr->getGrid().y + deltaRow, curCol = block_ptr->getGrid().x + deltaCol;
				if (collideWithBoundary(curRow, curCol)) return true;
			}
		}
		return false;
	}

	sf::Vector2f gridToPosition(unsigned int row, unsigned int col) {
		return sf::Vector2f((float)col * getGridSize().x, (float)row * getGridSize().y);
	}

	void drawFilled(sf::RenderWindow& window) {
		for (int row = 0; row < getHeight(); row++) {
			for (int col = 0; col < getWidth(); col++) {
				if (getFilledType(row, col) == BlockType::None) continue;
				SpriteMap[getFilledType(row, col)].setPosition(sf::Vector2f(gridToPosition(row, col)));
				window.draw(SpriteMap[getFilledType(row, col)]);
			}
		}
	}

	void drawBlock(std::unique_ptr<Block>& block_ptr, sf::RenderWindow& window) {
		sf::Vector2i curGrid = block_ptr->getGrid();

		for (auto& colRows : block_ptr->getShape()) {
			int deltaCol = colRows.first;
			for (auto& deltaRow : colRows.second) {
				int curRow = curGrid.y + deltaRow, curCol = curGrid.x + deltaCol;
				SpriteMap[block_ptr->getType()].setPosition(sf::Vector2f(gridToPosition(curRow, curCol)));
				window.draw(SpriteMap[block_ptr->getType()]);
			}
		}
	}

	void FindButtoms(std::unique_ptr<Block>& block_ptr) {
		int distance = getHeight();
		for (auto& colRows : block_ptr->getShape()) {
			int colDelta = colRows.first;
			int curCol = block_ptr->getGrid().x + colDelta;
			for (auto& rowDelta : colRows.second) {
				int curRow = block_ptr->getGrid().y + rowDelta;
				int curDist = getButtom(curCol) - 1 - curRow;
				if (curDist < distance) distance = curDist;
			}
		}

		block_ptr->increGridY(distance);
	}

	void eliminateRowsIfNeeded() {
		int moveSteps = 0;
		bool started = false;
		for (int row = getHeight(); row > 0; row--) {

			if (rowIsFull(row - 1)) {
				cleanRowFilled(row - 1);
				moveSteps++;
			}

			if ((row >= getHeight() && moveSteps) || (!started && moveSteps)) {
				started = true;
				continue;
			}
			else if (!started) continue;

			int rowToCopy = row - moveSteps;
			if (rowToCopy < 0) break;

			copyRow(rowToCopy, row);
		}

		// update current buttoms
		if (moveSteps) {
			for (int col = 0; col < getWidth(); col++) {
				int nextButtomRow = getButtom(col) + moveSteps;
				while (nextButtomRow < getHeight() && getFilledType(nextButtomRow, col) == BlockType::None) nextButtomRow++;
				setButtom(nextButtomRow, col);
			}
		}

		// clean above
		for (int row = 0; row < moveSteps; row++) {
			cleanRowFilled(row);
		}
	}
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
	Game(Board& board, float frameFreq) : board(board), frameFreq(frameFreq) {
		initGrid = sf::Vector2i(board.getWidth() / 2, 0);
		frameFreq = frameFreq;
	}

	auto genRandomBlock(sf::Vector2i &grid) {
		srand(time(NULL));
		BlockType blockType = (BlockType)(rand() % ((int)BlockType::Size-1) + 1);
		return blockFactory(blockType, grid);
	}

	void resetBlockAction() {
		action = Action::Hold;
	}
 
	void setBlockAction() {
		if (!Paused()) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				action = Action::Left;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				action = Action::Right;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				action = Action::Down;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				action = Action::Rotate;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				action = Action::ToButtom;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
			action = Action::Pause;
		}
	};

	void setSpeed() {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
			frameFreq = 0.5;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
			frameFreq = 0.3;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
			frameFreq = 0.1;
		}
	};

	void handleEvent(sf::RenderWindow &window) {
		sf::Event evt;
		if (window.pollEvent(evt)) {
			switch (evt.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				setBlockAction();
				setSpeed();
				toggleDead();
				break;
			}
		}
	};

	void timeToFallDown(sf::Clock& clock) {
		if (clock.getElapsedTime().asSeconds() >= frameFreq) {
			action = Action::Down;
			clock.restart();
		}
	};

	void doAction(std::unique_ptr<Block>& block_ptr) {
		if (action == Action::Down) block_ptr->increGridY(1);
		else if (action == Action::Left) {
			block_ptr->increGridX(-1);
		}
		else if (action == Action::Right) {
			block_ptr->increGridX(1);
		}
		else if (action == Action::Rotate) {
			block_ptr->rotate();
		}
		else if (action == Action::ToButtom) {
			board.FindButtoms(block_ptr);
		}
		else if (action == Action::Pause) {
			togglePause();
		}
	};

	void rollBackAction(std::unique_ptr<Block> &block_ptr) {
		if (action == Action::Down) block_ptr->increGridY(-1);
		else if (action == Action::Left) {
			block_ptr->increGridX(1);
		}
		else if (action == Action::Right) {
			block_ptr->increGridX(-1);
		}
		else if (action == Action::Rotate) {
			block_ptr->rollBackRotate();
		}
	}

	bool Paused() {
		return paused;
	}

	void togglePause() {
		paused = !paused;
	}

	bool Dead() {
		return dead;
	}

	void setDead() {
		dead = true;
	}

	void toggleDead() {
		if (dead) {
			dead = !dead;
			resetGame();
		}
	}

	bool checkIfDead(std::unique_ptr<Block> &block_ptr) {
		for (auto& colRows : block_ptr->getShape()) {
			int deltaCol = colRows.first;
			for (auto& deltaRow : colRows.second) {
				int curRow = block_ptr->getGrid().y + deltaRow, curCol = block_ptr->getGrid().x + deltaCol;
				if (curRow < 0) {
					setDead();
					return true;
				}
			}
		}
		return false;
	}

	void renderDead(sf::RenderWindow& window) {
		while (Dead()) {
			window.clear(sf::Color::Red);
			window.display();
			handleEvent(window);
		}
	}

	void resetGame() {
		block_ptr = genRandomBlock(initGrid);
		board.cleanFilled();
		board.resetButtom();
	}

	void sleep(float second) {
		sf::Clock timer;
		while (timer.getElapsedTime().asSeconds() < second) {};
	}

	void run() {
		sf::Vector2u windowSize = sf::Vector2u(board.getGridSize().x * board.getWidth(), board.getGridSize().y * board.getHeight());
		sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Tetris");
		block_ptr = genRandomBlock(initGrid);
		sf::Clock clock;

		while (window.isOpen()) {

			window.clear();
			window.draw(board.getBgSprite());
			board.drawFilled(window);

			if (Dead()) {
				window.display();
				sleep(0.5);
				renderDead(window);
				continue;
			}

			resetBlockAction();
			handleEvent(window);
			timeToFallDown(clock);
			doAction(block_ptr);

			if (Paused()) {
				clock.restart();
				continue;
			}

			bool collided = board.shapeCollideWithBoundary(block_ptr);
			if (!collided) {
				board.drawBlock(block_ptr, window);
			}
			else {
				rollBackAction(block_ptr);
				if (action == Action::Down) {
					board.setShapeFilledType(block_ptr);
					board.eliminateRowsIfNeeded();
					if (checkIfDead(block_ptr)) continue;
					block_ptr = genRandomBlock(initGrid);
				}
			}

			window.display();
		}
	}
};

int main() {
	const int width = 10, height = 20;
	class Board board = Board(width, height, "images/background.png");

	float frameFreq = 0.5f;
	class Game game = Game(board, frameFreq);

	game.run();
}