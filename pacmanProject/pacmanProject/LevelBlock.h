#include "Object.h"

#include <windows.h>
#include <gl/GL.h>

class LevelBlock: public Object
{
public:

	static const std::string FILE_LEVEL_BLOCK_LINEAR;
	static const std::string FILE_LEVEL_BLOCK_T_TYPE;
	static const std::string FILE_LEVEL_BLOCK_CORNER;
	static const std::string FILE_LEVEL_BLOCK_CROSS;

	enum LevelBlockType {
		LEVEL_BLOCK_TYPE_LINEAR,
		LEVEL_BLOCK_TYPE_T_TYPE,
		LEVEL_BLOCK_TYPE_CORNER,
		LEVEL_BLOCK_TYPE_CROSS
	};

	enum Orientation {
		ORIENTATION_TOP,
		ORIENTATION_BOTTOM,
		ORIENTATION_LEFT,
		ORIENTATION_RIGHT
	};

	LevelBlock();
	// Initializes object with texture and sets its orientation
	void create(LevelBlockType blocktype, Orientation orientation);

	LevelBlockType getBlockType() const;
	Orientation getOrientation() const;
	

protected:
	virtual void draw();
	void setBlockType(LevelBlockType value);
	void setOrientation(Orientation value);

private:
	//Make this method private to prevent manual rotation
	virtual void setRotationZ(float value);
	LevelBlockType _blockType;
	Orientation _orientation;
	GLuint _texture;
};