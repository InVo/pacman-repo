#pragma once

#include "Object.h"
/**
	Base class for all object on the game level
*/
class LevelObject: public Object
{
public:
	LevelObject();
	~LevelObject();

	int getCoordinateX() const;
	void setCoordinateX(int coordX);

	int getCoordinateY() const;
	void setCoordinateY(int coordY);

private:
	//Game coordinates for this object
	int _coordinateX;
	int _coordinateY;
};