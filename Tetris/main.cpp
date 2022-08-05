#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <time.h>
#include "header.h"

// Class Sprites

Sprites::Sprites() {
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

std::string Sprites::getBlockImgPath(std::string& color) {
	std::string pathBath = "images/";
	return pathBath + color + ".png";
}

void Sprites::loadTexture(sf::Texture& texture, std::string&& color) {
	std::string filePath = getBlockImgPath(color);
	if (!texture.loadFromFile(filePath)) throw std::invalid_argument("File doesn't exist!");
}

void Sprites::loadAllTexture() {
	loadTexture(blue, "blue");
	loadTexture(green, "green");
	loadTexture(light_blue, "light_blue");
	loadTexture(orange, "orange");
	loadTexture(purple, "purple");
	loadTexture(red, "red");
	loadTexture(yellow, "yellow");
}

std::unordered_map<BlockType, sf::Sprite> Sprites::getData() { return data; }


// Class Block

void Block::initShape() {
	shapeIndex = rand() % shapes.size();
	shape = shapes[shapeIndex];
}

Block::Block(BlockType type, const sf::Vector2i &grid) : grid(grid), type(type) {

}

sf::Vector2i Block::getGrid() {
	return grid;
}

void Block::setGrid(int x, int y) {
	grid = sf::Vector2i(x, y);
}

void Block::increGridY(int amount) {
	grid.y = grid.y + amount;
}

void Block::increGridX(int amount) {
	grid.x = grid.x + amount;
}

std::unordered_map<int, std::vector<int>> Block::getShape() {
	return shape;
}

BlockType Block::getType() {
	return type;
}

void Block::rotate() {
	shapeIndex = (shapeIndex + 1) % shapes.size();
	shape = shapes[shapeIndex];
}

void Block::rollBackRotate() {
	shapeIndex = (shapeIndex - 1) % shapes.size();
	shape = shapes[shapeIndex];
}



void O::initShapes() {
	// { {col1: rows, ... }, ... }
	shapes = {
		{{0, {-1, 0}}, {-1, {-1, 0}}}
	};
}

O::O(BlockType type, const sf::Vector2i& grid) : Block(type, grid) {
	initShapes();
	initShape();
}


void I::initShapes () {
	shapes = {
		{ {0, {-1, 0, 1, 2}} },
		{ {-2, {-1}}, {-1, {-1}}, {0, {-1}}, {1, {-1}} }
	};
};

I::I(BlockType type, const sf::Vector2i& grid) : Block(type, grid) {
	initShapes();
	initShape();
}


void J::initShapes() {
	shapes = {
		{ {-1, {1}}, {0, {-1, 0, 1}} },
		{ {-1, {-1}}, {0, {-1}}, {1, {-1, 0}} },
		{ {0, {-1, 0, 1}}, {1, {-1}} },
		{ {-1, {-1, 0}}, {0, {0}}, {1, {0}} }
	};
};

J::J(BlockType type, const sf::Vector2i& grid) : Block(type, grid) {
	initShapes();
	initShape();
}


void L::initShapes() {
	shapes = {
	{ {-1, {-1}}, {0, {-1, 0, 1}} },
	{ {-1, {-1, 0}}, {0, {-1}}, {1, {-1}} },
	{ {0, {-1, 0, 1}}, {1, {1}} },
	{ {-1, {0}}, {0, {0}}, {1, {-1, 0}} }
	};
};

L::L(BlockType type, const sf::Vector2i& grid) : Block(type, grid) {
	initShapes();
	initShape();
}


void Z::initShapes() {
	shapes = {
		{ {-1, {0, 1}}, {0, {-1, 0}} },
		{ {-1, {-1}}, {0, {-1, 0}}, {1, {0}} }
	};
};

Z::Z(BlockType type, const sf::Vector2i& grid) : Block(type, grid) {
	initShapes();
	initShape();
}


void S::initShapes() {
	shapes = {
		{ {-1, {-1, 0}}, {0, {0, 1}} },
		{ {-1, {0}}, {0, {-1, 0}}, {1, {-1}} }
	};
};

S::S(BlockType type, const sf::Vector2i& grid) : Block(type, grid) {
	initShapes();
	initShape();
}


void T::initShapes() {
	shapes = {
		{ {-1, {0}}, {0, {-1, 0, 1}} },
		{ {-1, {-1}}, {0, {-1, 0}}, {1, {-1}} },
		{ {0, {-1, 0, 1}}, {1, {0}} },
		{ {-1, {0}}, {0, {-1, 0}}, {1, {0}} }
	};
};

T::T(BlockType type, const sf::Vector2i& grid) : Block(type, grid) {
	initShapes();
	initShape();
}


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


// Class Board

std::string Board::getBlockImgPath(std::string& color) {
	std::string pathBath = "images/";
	return pathBath + color + ".png";
}

void Board::loadTexture(sf::Texture& texture, std::string&& color) {
	std::string filePathx = getBlockImgPath(color);
	if (!texture.loadFromFile(getBlockImgPath(color))) throw std::invalid_argument("File doesn't exist!");
}

void Board::loadAllTexture() {
	loadTexture(blue, "blue");
	loadTexture(green, "green");
	loadTexture(light_blue, "light_blue");
	loadTexture(orange, "orange");
	loadTexture(purple, "purple");
	loadTexture(red, "red");
	loadTexture(yellow, "yellow");
}

void Board::setBackground(const std::string& backgroundFilePath) {
	if (!backgroundTexture.loadFromFile(backgroundFilePath)) throw std::invalid_argument("File doesn't exist!");
	backgroundTexture.setRepeated(true);
	backgroundSprite = sf::Sprite(backgroundTexture, sf::IntRect(0, 0, width * getGridSize().x, height * getGridSize().y));
}

Board::Board(int width, int height, const std::string& backgroundFilePath) : width(width), height(height) {
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

sf::Vector2u Board::getGridSize() {
	return blue.getSize();
}

int Board::getWidth() {
	return width;
}

int Board::getHeight() {
	return height;
}

sf::Sprite Board::getBgSprite() {
	return backgroundSprite;
}

BlockType Board::getFilledType(int row, int col) {
	return filled[row][col];
}

BlockType Board::getFilledType(sf::Vector2i& blockGrid) {
	return filled[blockGrid.y][blockGrid.x];
}

void Board::setFilledType(int row, int col, BlockType type) {
	filled[row][col] = type;
}

void Board::setFilledType(sf::Vector2i& blockGrid, BlockType type) {
	filled[blockGrid.y][blockGrid.x] = type;
}

void Board::setShapeFilledType(const std::unique_ptr<Block>& block_ptr) {
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

void Board::cleanRowFilled(int row) {
	for (int col = 0; col < width; col++) {
		setFilledType(row, col, BlockType::None);
	}
	setRowFilledSize(row, 0);
}

void Board::cleanFilled() {
	for (int row = 0; row < getHeight(); row++) {
		cleanRowFilled(row);
	}
}

bool Board::rowIsFull(int row) {
	return filledSizePerRow[row] == width;
}

bool Board::rowIsEmpty(int row) {
	return filledSizePerRow[row] == 0;
}

int Board::getRowFilledSize(int row) {
	return filledSizePerRow[row];
}

void Board::setRowFilledSize(int row, int size) {
	filledSizePerRow[row] = size;
}

void Board::increRowFilledSize(int row, int amount) {
	filledSizePerRow[row] += amount;
}

int Board::getButtom(int col) {
	return curButtom[col];
}

void Board::setButtom(int row, int col) {
	curButtom[col] = row;
}

void Board::resetButtom() {
	for (int col = 0; col < getWidth(); col++) curButtom[col] = getHeight();
}

void Board::copyRow(int sourceRow, int targetRow) {
	for (int col = 0; col < getWidth(); col++) {
		setFilledType(targetRow, col, getFilledType(sourceRow, col));
	}
	setRowFilledSize(targetRow, getRowFilledSize(sourceRow));
}

bool Board::collideWithBoundary(int row, int col) {
	if (row >= getHeight() ||
		col < 0 ||
		col >= getWidth() ||
		(row > -1 && getFilledType(row, col) != BlockType::None)
		) return true;
	return false;
}

bool Board::shapeCollideWithBoundary(const std::unique_ptr<Block>& block_ptr) {
	for (auto& colRows : block_ptr->getShape()) {
		int deltaCol = colRows.first;
		for (auto& deltaRow : colRows.second) {
			int curRow = block_ptr->getGrid().y + deltaRow, curCol = block_ptr->getGrid().x + deltaCol;
			if (collideWithBoundary(curRow, curCol)) return true;
		}
	}
	return false;
}

sf::Vector2f Board::gridToPosition(unsigned int row, unsigned int col) {
	return sf::Vector2f((float)col * getGridSize().x, (float)row * getGridSize().y);
}

void Board::drawFilled(sf::RenderWindow& window) {
	for (int row = 0; row < getHeight(); row++) {
		for (int col = 0; col < getWidth(); col++) {
			if (getFilledType(row, col) == BlockType::None) continue;
			SpriteMap[getFilledType(row, col)].setPosition(sf::Vector2f(gridToPosition(row, col)));
			window.draw(SpriteMap[getFilledType(row, col)]);
		}
	}
}

void Board::drawBlock(std::unique_ptr<Block>& block_ptr, sf::RenderWindow& window) {
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

void Board::FindButtoms(std::unique_ptr<Block>& block_ptr) {
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

void Board::eliminateRowsIfNeeded() {
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


// Class Game

Game::Game(Board& board, float frameFreq) : board(board), frameFreq(frameFreq) {
	initGrid = sf::Vector2i(board.getWidth() / 2, 0);
	frameFreq = frameFreq;
}

auto Game::genRandomBlock(sf::Vector2i& grid) {
	srand(time(NULL));
	BlockType blockType = (BlockType)(rand() % ((int)BlockType::Size-1) + 1);
	return blockFactory(blockType, grid);
}

void Game::resetBlockAction() {
	action = Action::Hold;
}
 
void Game::setBlockAction() {
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

void Game::setSpeed() {
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

void Game::handleEvent(sf::RenderWindow& window) {
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

void Game::timeToFallDown(sf::Clock& clock) {
	if (clock.getElapsedTime().asSeconds() >= frameFreq) {
		action = Action::Down;
		clock.restart();
	}
};

void Game::doAction(std::unique_ptr<Block>& block_ptr) {
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

void Game::rollBackAction(std::unique_ptr<Block>& block_ptr) {
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

bool Game::Paused() {
	return paused;
}

void Game::togglePause() {
	paused = !paused;
}

bool Game::Dead() {
	return dead;
}

void Game::setDead() {
	dead = true;
}

void Game::toggleDead() {
	if (dead) {
		dead = !dead;
		resetGame();
	}
}

bool Game::checkIfDead(std::unique_ptr<Block>& block_ptr) {
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

void Game::renderDead(sf::RenderWindow& window) {
	while (Dead()) {
		window.clear(sf::Color::Red);
		window.display();
		handleEvent(window);
	}
}

void Game::resetGame() {
	block_ptr = genRandomBlock(initGrid);
	board.cleanFilled();
	board.resetButtom();
}

void Game::sleep(float second) {
	sf::Clock timer;
	while (timer.getElapsedTime().asSeconds() < second) {};
}

void Game::run() {
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


int main() {
	const int width = 10, height = 20;
	class Board board(width, height, "images/background.png");

	float frameFreq = 0.5f;
	class Game game(board, frameFreq);

	game.run();
}