#pragma once

#include "Object.h"
/**
	Base class for all object on the game level
*/
class LevelObject: public Object
{
public:
	enum LevelObjectType {
		LEVEL_OBJECT_NONE,
		LEVEL_OBJECT_BLOCK
	};

	LevelObject();
	~LevelObject();

	int getCoordinateX() const;
	void setCoordinateX(int coordX);

	int getCoordinateY() const;
	void setCoordinateY(int coordY);

	virtual void render();
	LevelObjectType getObjectType();
protected:
	//Game coordinates for this object
	int _coordinateX;
	int _coordinateY;
	LevelObjectType _objectType;
};