#pragma once

#include <vector>
#include <windows.h>
#include <gl/GL.h>

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
	virtual float getPositionX() const;
	virtual void setPositionX(float value);
	
	virtual float getPositionY() const;
	virtual void setPositionY(float value);

	virtual float getPositionZ() const;
	virtual void setPositionZ(float value);

	//Rotation
	virtual float getRotationX() const;
	virtual void setRotationX(float value);
	
	virtual float getRotationY() const;
	virtual void setRotationY(float value);

	virtual float getRotationZ() const;
	virtual void setRotationZ(float value);

	//Size
	virtual float getWidth() const;
	virtual void setWidth(GLfloat value);

	virtual float getHeight() const;
	virtual void setHeight(GLfloat value);

protected:
	std::vector<Object*> _children;
	float _x;
	float _y;
	float _z;

	float _rotationX;
	float _rotationY;
	float _rotationZ;

	GLfloat _width;
	GLfloat _height;
};