#pragma once

#include <vector>

/* Base class for all drawable objects
*/
class Object
{
public:
	Object();
	~Object();
	virtual void draw();
	virtual void render();
	virtual void addChild(Object* child);
	virtual void removeChild(Object* child);
	
	virtual float getPositionX();
	virtual void setPositionX(float value);
	
	virtual float getPositionY();
	virtual void setPositionY(float value);

	virtual float getPositionZ();
	virtual void setPositionZ(float value);

private:
	std::vector<Object*> _children;
	float _x;
	float _y;
	float _z;
};