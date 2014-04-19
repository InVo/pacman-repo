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
	_lineTexturePath = "Data/Glass.bmp";
	_cornerTexturePath = "Data/line.jpg";
	_tTypeTexturePath = "Data/line.jpg";
	_crossTexturePath = "Data/line.jpg";
	createLevelTexture(_lineTexture, _lineTexturePath);
	createLevelTexture(_cornerTexture, _cornerTexturePath);
	createLevelTexture(_tTypeTexture, _tTypeTexturePath);
	createLevelTexture(_crossTexture, _crossTexturePath);
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
	glTranslatef(0.f, 0.f, -20.f);
	glBindTexture(GL_TEXTURE_2D, _lineTexture);
	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, 1.0f);     // Нормаль указывает на наблюдателя
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad		
	glEnd();
}