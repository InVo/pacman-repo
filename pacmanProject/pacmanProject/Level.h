#pragma once

#include "Object.h"
#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "LevelBlock.h"

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
	float _blockXStep;
	float _blockYStep;
	// Vector of all blocks on the level map
	std::vector<LevelBlock*> _levelBlocks;

};