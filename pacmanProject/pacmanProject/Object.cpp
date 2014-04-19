#include "Object.h"

void Object::draw()
{
}

void Object::render()
{
	draw();
	for (auto child = _children.begin(); child != _children.end(); ++child) {
		(*child)->draw();
	}
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