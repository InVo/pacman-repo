#include "LevelObject.h"

LevelObject::LevelObject()
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