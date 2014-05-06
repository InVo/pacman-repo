#include "Utils.h"
#include "SOIL.h"

int Utils::showMessage(LPCWSTR text)
{
	MessageBox( nullptr, text, L"Message", MB_OK | MB_ICONINFORMATION );
	return 0;
}

bool Utils::loadTextureFromFile(GLuint& texture, const std::string& textureFile)
{
	texture = SOIL_load_OGL_texture(textureFile.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);
	if (0 == texture) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	return true;
}