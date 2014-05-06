#include "LevelBlock.h"
#include "SOIL.h"
#include "Utils.h"

using namespace std;

const string LevelBlock::FILE_LEVEL_BLOCK_SQUARE = "Data/square.png";
const string LevelBlock::FILE_LEVEL_BLOCK_DEAD_END = "Data/dead_end.png";
const string LevelBlock::FILE_LEVEL_BLOCK_LINEAR = "Data/line.png";
const string LevelBlock::FILE_LEVEL_BLOCK_T_TYPE = "Data/t_type.png";
const string LevelBlock::FILE_LEVEL_BLOCK_CORNER = "Data/corner.png";
const string LevelBlock::FILE_LEVEL_BLOCK_CROSS = "Data/cross.png";

GLuint LevelBlock::_textureSquare = 0;
GLuint LevelBlock::_textureDeadEnd = 0;
GLuint LevelBlock::_textureLinear = 0;
GLuint LevelBlock::_textureTType = 0;
GLuint LevelBlock::_textureCorner = 0;
GLuint LevelBlock::_textureCross = 0;

LevelBlock::LevelBlock():
LevelObject(),
_texture(0)
{
	_objectType = LEVEL_OBJECT_BLOCK;
}

void LevelBlock::create(LevelBlockType blockType, Orientation orientation)
{
	setOrientation(orientation);
	setBlockType(blockType);
	std::string textureFile;
	switch(blockType) {
	case LEVEL_BLOCK_TYPE_SQUARE:
		if (0 == _textureSquare) {
			Utils::loadTextureFromFile(_textureSquare, FILE_LEVEL_BLOCK_SQUARE);
		}
		_texture = _textureSquare;
		break;
	case LEVEL_BLOCK_TYPE_DEAD_END:
		if (0 == _textureDeadEnd) {
			Utils::loadTextureFromFile(_textureDeadEnd, FILE_LEVEL_BLOCK_DEAD_END);
		}
		_texture = _textureDeadEnd;
		break;
	case LEVEL_BLOCK_TYPE_LINEAR:
		if (0 == _textureLinear) {
			Utils::loadTextureFromFile(_textureLinear, FILE_LEVEL_BLOCK_LINEAR);
		}
		_texture = _textureLinear;
		break;
	case LEVEL_BLOCK_TYPE_T_TYPE:
		if (0 == _textureTType) {
			Utils::loadTextureFromFile(_textureTType, FILE_LEVEL_BLOCK_T_TYPE);
		}
		_texture = _textureTType;
		break;
	case LEVEL_BLOCK_TYPE_CORNER:
		if (0 == _textureCorner) {
			Utils::loadTextureFromFile(_textureCorner, FILE_LEVEL_BLOCK_CORNER);
		}
		_texture = _textureCorner;
		break;
	case LEVEL_BLOCK_TYPE_CROSS:
		if (0 == _textureCross) {
			Utils::loadTextureFromFile(_textureCross, FILE_LEVEL_BLOCK_CROSS);
		}
		_texture = _textureCross;
		break;
	default:
		break;
	}
	if (0 == _texture) {
		Utils::showMessage(L"LevelBlock::create ERROR - texture was not loaded");
		return;
	}
	glBindTexture(GL_TEXTURE_2D, _texture);
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
		setRotationZ(-90.f);
		break;
	case ORIENTATION_RIGHT:
		setRotationZ(90.f);
		break;
	case ORIENTATION_BOTTOM:
		setRotationZ(180.f);
		break;
	}
}