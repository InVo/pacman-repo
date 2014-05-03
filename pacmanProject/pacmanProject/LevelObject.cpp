#include "LevelObject.h"

LevelObject::LevelObject():
	_coordinateX(0),
	_coordinateY(0)
{

}

LevelObject::~LevelObject()
{

}

int LevelObject::getCoordinateX() const 
{
	return _coordinateX;
}

void LevelObject::setCoordinateX(int value)
{
	_coordinateX = value;
}

int LevelObject::getCoordinateY() const 
{
	return _coordinateY;
}

void LevelObject::setCoordinateY(int value)
{
	_coordinateY = value;
}

LevelObject::LevelObjectType LevelObject::getObjectType()
{
	return _objectType;
}

void LevelObject::render()
{
	Object::render();
}