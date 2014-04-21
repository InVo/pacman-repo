#include "Object.h"
#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>

Object::Object():
	_x(0),
	_y(0),
	_z(0)
{

}

Object::~Object()
{

}

void Object::draw()
{
}

void Object::render()
{
	// Move to the drawing place
	glTranslatef(_x,_y, _z);
	draw();
	for (auto child = _children.begin(); child != _children.end(); ++child) {
		(*child)->render();
	}
	//Move back to the start place
	glTranslatef(-_x,-_y,-_z);
}

void Object::addChild(Object* child)
{
	for (auto ch = _children.begin(); ch != _children.end(); ++ch) {
		if (child == *ch) {
			return;
		}
	}

	_children.push_back(child);
}

void Object::removeChild(Object* child)
{
	int index = 0;
	for (auto ch = _children.begin(); ch != _children.end(); ++ch) {
		if (child == *ch) {
			_children.erase(_children.begin() + index);
			return;
		}
		++index;
	}
}

float Object::getPositionX()
{
	return _x;
}

float Object::getPositionY()
{
	return _y;
}

float Object::getPositionZ()
{
	return _z;
}

void Object::setPositionX(float value)
{
	_x = value;
}

void Object::setPositionY(float value)
{
	_y = value;
}

void Object::setPositionZ(float value)
{
	_z = value;
}