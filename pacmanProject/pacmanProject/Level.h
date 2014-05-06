#pragma once

#include "Object.h"
#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "LevelObject.h"

/* Level layer with walls
*/
class Level: public Object
{
public:
	// Number of level blocks in both dimensions
	static const unsigned int LEVEL_WIDTH;
	static const unsigned int LEVEL_HEIGHT;

	static const float LEVEL_START_X;
	static const float LEVEL_START_Y;

	static const float LEVEL_STEP_X;
	static const float LEVEL_STEP_Y;

	Level();
	~Level();
	bool init();
	virtual void draw();
	void createBlock(LevelObject* levelObject, int row, int column);
protected:
	void loadLevelData();
	void initLevelObjects();
private:
	float _blockXStep;
	float _blockYStep;
	float _startPositionX;
	float _startPositionY;
	// Vector of all blocks on the level map
	std::vector<std::vector<LevelObject*>> _levelObjects;
};