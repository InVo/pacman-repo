#include "Level.h"
#include "SOIL.h"

#include <iostream>
#include <fstream>
#include <string>

#include "Utils.h"
#include "LevelBlock.h"
#include "LevelDot.h"

using namespace std;

const unsigned int Level::LEVEL_WIDTH = 20;
const unsigned int Level::LEVEL_HEIGHT = 20;

const float Level::LEVEL_START_X = 0.f;
const float Level::LEVEL_START_Y = 200.f;

const float Level::LEVEL_STEP_X = 32.f;
const float Level::LEVEL_STEP_Y = 32.f;

Level::Level():
	_blockXStep(0),
	_blockYStep(0),
	_startPositionX(LEVEL_START_X),
	_startPositionY(LEVEL_START_Y)
{
	init();
}

Level::~Level()
{

}

bool Level::init()
{
	loadLevelData();
	initLevelObjects();
	return true;
}

void Level::initLevelObjects()
{
	for(unsigned int i = 0, iMax = _levelObjects.size(); i < iMax; ++i) {
		auto objectsRow = _levelObjects.at(i);
		for(unsigned int j = 0, jMax = objectsRow.size(); j < jMax; ++j) {
			auto levelObject = objectsRow.at(j);
			if (levelObject) {
				levelObject->setCoordinateX(j);
				levelObject->setCoordinateY(i);
				if (LevelObject::LEVEL_OBJECT_BLOCK == levelObject->getObjectType()) {
					createBlock(levelObject, i, j);
				}
				addChild(levelObject);
				levelObject->setPositionY(_startPositionY + i * LEVEL_STEP_Y);
				levelObject->setPositionX(_startPositionX + j * LEVEL_STEP_X);
			}
		}
	}
}

void Level::createBlock(LevelObject* levelObject, int row, int column)
{
	auto levelBlock = (LevelBlock*) levelObject;
	// Default type and orientation
	LevelBlock::LevelBlockType blockType = LevelBlock::LEVEL_BLOCK_TYPE_SQUARE;
	LevelBlock::Orientation orientation = LevelBlock::ORIENTATION_TOP;
	LevelObject* nearObject = nullptr;
	
	unsigned int _realLevelHeight = _levelObjects.size();
	if (_realLevelHeight == 0) {
		Utils::showMessage(L"Level file is empty.");
		return;
	}

	unsigned int _realLevelWidth = _levelObjects.at(0).size();

	// Number of blocks near this one
	unsigned int numberOfNeighbours = 0;

	// Number of vertical and horizontal blocks near this one
	unsigned int numOfVert = 0;
	unsigned int numOfHor = 0;

	auto haveTop = false, 
		 haveBottom = false, 
		 haveLeft = false, 
		 haveRight = false;

	LevelObject::LevelObjectType objectType;

	// LEFT
	if (0 < column) {
		auto levelRow = _levelObjects.at(row);
		auto levelObject = levelRow.at(column - 1);
		objectType = levelObject ? levelObject->getObjectType() : LevelObject::LEVEL_OBJECT_NONE;
		if (LevelObject::LEVEL_OBJECT_BLOCK == objectType) {
			++numberOfNeighbours;
			++numOfHor;
			haveLeft = true;
		}
	}
	
	// RIGHT
	if (_realLevelWidth - 1 > column) {
		auto levelRow = _levelObjects.at(row);
		auto levelObject = levelRow.at(column + 1);
		objectType = levelObject ? levelObject->getObjectType() : LevelObject::LEVEL_OBJECT_NONE;
		if (LevelObject::LEVEL_OBJECT_BLOCK == objectType) {
			++numberOfNeighbours;
			++numOfHor;
			haveRight = true;
		}
	}

	// TOP
	if (0 < row) {
		auto levelRow = _levelObjects.at(row - 1);
		auto levelObject = levelRow.at(column);
		objectType = levelObject ? levelObject->getObjectType() : LevelObject::LEVEL_OBJECT_NONE;
		if (LevelObject::LEVEL_OBJECT_BLOCK == objectType) {
			++numberOfNeighbours;
			++numOfVert;
			haveTop = true;
		}
	}

	// BOTTOM
	if (_realLevelHeight - 1 > row) {
		auto levelRow = _levelObjects.at(row + 1);
		auto levelObject = levelRow.at(column);
		objectType = levelObject ? levelObject->getObjectType() : LevelObject::LEVEL_OBJECT_NONE;
		if (LevelObject::LEVEL_OBJECT_BLOCK == objectType) {
			++numberOfNeighbours;
			++numOfVert;
			haveBottom = true;
		}
	}

	// SQUARE
	if (0 == numberOfNeighbours) {
		blockType = LevelBlock::LEVEL_BLOCK_TYPE_SQUARE;
		orientation = LevelBlock::ORIENTATION_TOP;
	}

	// DEAD END
	if (1 == numberOfNeighbours) {
		blockType = LevelBlock::LEVEL_BLOCK_TYPE_DEAD_END;
		if (haveLeft) orientation = LevelBlock::ORIENTATION_RIGHT;
		else if (haveRight) orientation = LevelBlock::ORIENTATION_LEFT;
		else if (haveTop) orientation = LevelBlock::ORIENTATION_BOTTOM;
		else if (haveBottom) orientation = LevelBlock::ORIENTATION_TOP;
	}

	// LINE
	if (2 == numberOfNeighbours && numOfVert != numOfHor) {
		blockType = LevelBlock::LEVEL_BLOCK_TYPE_LINEAR;
		if (haveLeft) orientation = LevelBlock::ORIENTATION_RIGHT;
		else if (haveTop) orientation = LevelBlock::ORIENTATION_TOP;
	}

	// CORNER
	if (2 == numberOfNeighbours && numOfVert == numOfHor) {
		blockType = LevelBlock::LEVEL_BLOCK_TYPE_CORNER;
		if (haveLeft && haveTop) orientation = LevelBlock::ORIENTATION_TOP;
		else if (haveRight && haveTop) orientation = LevelBlock::ORIENTATION_RIGHT;
		else if (haveRight && haveBottom) orientation = LevelBlock::ORIENTATION_BOTTOM;
		else if (haveLeft && haveBottom) orientation = LevelBlock::ORIENTATION_LEFT;
	}

	// T-TYPE
	if (3 == numberOfNeighbours) {
		blockType = LevelBlock::LEVEL_BLOCK_TYPE_T_TYPE;
		if (numOfVert > numOfHor) {
			if (haveLeft) orientation = LevelBlock::ORIENTATION_LEFT;
			else if (haveRight) orientation = LevelBlock::ORIENTATION_RIGHT;
		} else {
			if (haveTop) orientation = LevelBlock::ORIENTATION_TOP;
			else if (haveBottom) orientation = LevelBlock::ORIENTATION_BOTTOM;
		}
	}

	if (4 == numberOfNeighbours) {
		blockType = LevelBlock::LEVEL_BLOCK_TYPE_CROSS;
	}

	levelBlock->create(blockType, orientation);
}

void Level::draw()
{

}

void Level::loadLevelData()
{
//	_levelObjects.resize(LEVEL_HEIGHT);

	string fileName = "Data/data.txt";
	ifstream file;
	file.open(fileName);
	string line;
	unsigned int lineNumber = 0;
	unsigned int maxLineLength = 0;
	if (file.is_open()) {
		while(getline(file, line)) {
			_levelObjects.push_back(vector<LevelObject*>());
			for(unsigned int i = 0, iMax = line.length(); i < iMax; i++) {
				char c = line[i];
				auto& blockRow = _levelObjects.at(lineNumber);
				switch(c) {
					case '*':
						blockRow.push_back(new LevelBlock());
						break;
					case '.':
						blockRow.push_back(new LevelDot());
						break;
					default:
						blockRow.push_back(nullptr);
						break;
				}
			}
			++lineNumber;
		}
	}
	file.close();
}