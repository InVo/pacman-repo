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