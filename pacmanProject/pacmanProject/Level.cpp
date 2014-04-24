#include "Level.h"
#include "SOIL.h"

Level::Level()
{
	init();
}

Level::~Level()
{

}

bool Level::init()
{
	LevelBlock* block = new LevelBlock();
	block->create(LevelBlock::LEVEL_BLOCK_TYPE_LINEAR, LevelBlock::ORIENTATION_TOP);
	addChild(block);
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