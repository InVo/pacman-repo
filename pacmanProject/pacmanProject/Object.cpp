#include "Object.h"
#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>

Object::Object():
	_x(0.f),
	_y(0.f),
	_z(0.f),
	_rotationX(0.f),
	_rotationY(0.f),
	_rotationZ(0.f)
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
	//Rotating
	glRotatef(_rotationX, 1.f, 0.f, 0.f);
	glRotatef(_rotationY, 0.f, 1.f, 0.f);
	glRotatef(_rotationZ, 0.f, 0.f, 1.f);

	draw();
	for (auto child = _children.begin(); child != _children.end(); ++child) {
		(*child)->render();
	}

	// Cancelling rotation
	glRotatef(_rotationX, 1.f, 0.f, 0.f);
	glRotatef(_rotationY, 0.f, 1.f, 0.f);
	glRotatef(_rotationZ, 0.f, 0.f, 1.f);
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


//Position

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


//Rotation

float Object::getRotationX()
{
	return _rotationX;
}

float Object::getRotationY()
{
	return _rotationY;
}

float Object::getRotationZ()
{
	return _rotationZ;
}

void Object::setRotationX(float value)
{
	_rotationX = value;
}

void Object::setRotationY(float value)
{
	_rotationY = value;
}

void Object::setRotationZ(float value)
{
	_rotationZ = value;
}