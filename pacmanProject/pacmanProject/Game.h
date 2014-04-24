#pragma once

#include "Object.h"
#include "Level.h"

/* Main game class. Other game layers (menu, level, characters)
are all childed to the object of this class
*/
class Game: public Object
{
public:
	static const float LEVEL_Z_DEPTH;
	Game();
	~Game();
	void render();
private:
	Level* _level;
};