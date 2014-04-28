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
	// Number of level blocks in both dimensions
	static const unsigned int LEVEL_WIDTH;
	static const unsigned int LEVEL_HEIGHT;

	Level();
	~Level();
	bool init();
	virtual void draw();
protected:
	void createLevelTexture(GLuint& texture, std::string& texturePath);
	void loadLevelData();
private:
	float _blockXStep;
	float _blockYStep;
	// Vector of all blocks on the level map
	std::vector<std::vector<LevelBlock*>> _levelBlocks;
};