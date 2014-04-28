#include "Level.h"
#include "SOIL.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const unsigned int Level::LEVEL_WIDTH = 20;
const unsigned int Level::LEVEL_HEIGHT = 20;

Level::Level():
	_blockXStep(0),
	_blockYStep(0)
{
	init();
}

Level::~Level()
{

}

bool Level::init()
{
	loadLevelData();
	LevelBlock* block = new LevelBlock();
	block->create(LevelBlock::LEVEL_BLOCK_TYPE_LINEAR, LevelBlock::ORIENTATION_TOP);
	addChild(block);
	LevelBlock* block2 = new LevelBlock();
	block2->create(LevelBlock::LEVEL_BLOCK_TYPE_LINEAR, LevelBlock::ORIENTATION_LEFT);
	block2->setPositionX(block->getPositionX() + block->getWidth());
	block2->setPositionY(block->getPositionY());
	addChild(block2);
	return true;
}

void Level::createLevelTexture(GLuint& texture, std::string& texturePath)
{
	texture = SOIL_load_OGL_texture(texturePath.c_str(),SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Level::draw()
{

}

void Level::loadLevelData()
{
	_levelBlocks = vector<vector<LevelBlock*>>(LEVEL_HEIGHT);

	string fileName = "Data/data.txt";
	ifstream file;
	file.open(fileName);
	string line;
	unsigned int lineNumber = 0;
	if (file.is_open()) {
		while(getline(file, line)) {
			for(unsigned int i = 0, iMax = line.length(); i < iMax; i++) {
				char c = line[i];
				switch(c) {
					case '*':
						break;
					default:
						break;
				}
			}
		}
	}
	file.close();
}