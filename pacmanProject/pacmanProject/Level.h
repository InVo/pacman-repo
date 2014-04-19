#pragma once

#include "Object.h"
#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>

/* Level layer with walls
*/
class Level: public Object
{
public:
	Level();
	~Level();
	bool init();
	virtual void draw();
protected:
	void createLevelTexture(GLuint& texture, std::string& texturePath);
private:
	std::string _lineTexturePath; // Line type wall texture
	std::string _cornerTexturePath; // corner type wall texture
	std::string _tTypeTexturePath; // T-type wall texture
	std::string _crossTexturePath; // cross type wall texture
	GLuint _lineTexture;
	GLuint _cornerTexture;
	GLuint _tTypeTexture;
	GLuint _crossTexture;

};