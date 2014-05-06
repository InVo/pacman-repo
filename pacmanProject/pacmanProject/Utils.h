#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <string>

class Utils
{
public:
	static int showMessage(LPCWSTR text);
	static bool loadTextureFromFile(GLuint& texture, const std::string& textureFile);
};