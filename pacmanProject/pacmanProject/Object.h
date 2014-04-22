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
	
	//Position
	virtual float getPositionX();
	virtual void setPositionX(float value);
	
	virtual float getPositionY();
	virtual void setPositionY(float value);

	virtual float getPositionZ();
	virtual void setPositionZ(float value);

	//Rotation
	virtual float getRotationX();
	virtual void setRotationX(float value);
	
	virtual float getRotationY();
	virtual void setRotationY(float value);

	virtual float getRotationZ();
	virtual void setRotationZ(float value);
private:
	std::vector<Object*> _children;
	float _x;
	float _y;
	float _z;

	float _rotationX;
	float _rotationY;
	float _rotationZ;
};