#pragma once

#include <vector>

/* Base class for all drawable objects
*/
class Object
{
public:
	virtual void draw();
	virtual void render();
	virtual void addChild(Object* child);
	virtual void removeChild(Object* child);
private:
	std::vector<Object*> _children;
};