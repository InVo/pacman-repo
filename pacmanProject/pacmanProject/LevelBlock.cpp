#include "LevelBlock.h"
#include "SOIL.h"

using namespace std;

const string LevelBlock::FILE_LEVEL_BLOCK_LINEAR = "Data/line.png";
const string LevelBlock::FILE_LEVEL_BLOCK_T_TYPE = "Data/t_type.png";
const string LevelBlock::FILE_LEVEL_BLOCK_CORNER = "Data/corner.png";
const string LevelBlock::FILE_LEVEL_BLOCK_CROSS = "Data/cross.png";

LevelBlock::LevelBlock()
{

}

void LevelBlock::create(LevelBlockType blockType, Orientation orientation)
{
	setOrientation(orientation);
	setBlockType(blockType);
	std::string textureFile;
	switch(blockType) {
	case LEVEL_BLOCK_TYPE_LINEAR:
		textureFile = FILE_LEVEL_BLOCK_LINEAR;
		break;
	case LEVEL_BLOCK_TYPE_T_TYPE:
		textureFile = FILE_LEVEL_BLOCK_T_TYPE;
		break;
	case LEVEL_BLOCK_TYPE_CORNER:
		textureFile = FILE_LEVEL_BLOCK_CORNER;
		break;
	case LEVEL_BLOCK_TYPE_CROSS:
		textureFile = FILE_LEVEL_BLOCK_CROSS;
		break;
	default:
		break;
	}
	_texture = SOIL_load_OGL_texture(textureFile.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &_width);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &_height);
}

void LevelBlock::draw()
{
	glBindTexture(GL_TEXTURE_2D, _texture);
	glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f); glVertex2d(-_width / 2, - _height / 2);
		glTexCoord2f(1.f, 0.f); glVertex2d(_width / 2, - _height / 2);
		glTexCoord2f(1.f, 1.f); glVertex2d(_width / 2, _height / 2);
		glTexCoord2f(0.f, 1.f); glVertex2d(-_width / 2, _height / 2);
	glEnd();
}

void LevelBlock::setRotationZ(float value)
{
	Object::setRotationZ(value);
}

LevelBlock::LevelBlockType LevelBlock::getBlockType() const 
{
	return _blockType;
}

void LevelBlock::setBlockType(LevelBlock::LevelBlockType value)
{
	_blockType = value;
}

LevelBlock::Orientation LevelBlock::getOrientation() const
{
	return _orientation;
}

void LevelBlock::setOrientation(LevelBlock::Orientation value)
{
	_orientation = value;
	switch (_orientation) {
	case ORIENTATION_TOP:
		setRotationZ(0.f);
		break;
	case ORIENTATION_LEFT:
		setRotationZ(90.f);
		break;
	case ORIENTATION_RIGHT:
		setRotationZ(-90.f);
		break;
	case ORIENTATION_BOTTOM:
		setRotationZ(180.f);
		break;
	}
}


int LevelBlock::getCoordinateX() const 
{
	return _coordinateX;
}

void LevelBlock::setCoordinateX(int value)
{
	_coordinateX = value;
}

int LevelBlock::getCoordinateY() const 
{
	return _coordinateY;
}

void LevelBlock::setCoordinateY(int value)
{
	_coordinateY = value;
}