#include "LevelDot.h"
#include "SOIL.h"
#include "Utils.h"

using namespace std;

const string LevelDot::FILE_LEVEL_DOT_TEXTURE = "Data/dot.png";

GLuint LevelDot::_textureDot = 0;

LevelDot::LevelDot()
{
	init();
}

void LevelDot::init()
{
	if (0 == _textureDot) {
		Utils::loadTextureFromFile(_textureDot, FILE_LEVEL_DOT_TEXTURE);
	}
	glBindTexture(GL_TEXTURE_2D, _textureDot);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &_width);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &_height);
}

void LevelDot::draw()
{
	glBindTexture(GL_TEXTURE_2D, _textureDot);
	glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f); glVertex2d(-_width / 2, - _height / 2);
		glTexCoord2f(1.f, 0.f); glVertex2d(_width / 2, - _height / 2);
		glTexCoord2f(1.f, 1.f); glVertex2d(_width / 2, _height / 2);
		glTexCoord2f(0.f, 1.f); glVertex2d(-_width / 2, _height / 2);
	glEnd();
}

void LevelDot::onEat(void* data)
{
	
}