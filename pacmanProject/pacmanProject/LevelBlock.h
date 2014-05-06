#include "LevelObject.h"

#include <windows.h>
#include <gl/GL.h>
/**
	Class of walls on the game level
*/
class LevelBlock: public LevelObject
{
public:

	static const std::string FILE_LEVEL_BLOCK_SQUARE;
	static const std::string FILE_LEVEL_BLOCK_DEAD_END;
	static const std::string FILE_LEVEL_BLOCK_LINEAR;
	static const std::string FILE_LEVEL_BLOCK_T_TYPE;
	static const std::string FILE_LEVEL_BLOCK_CORNER;
	static const std::string FILE_LEVEL_BLOCK_CROSS;

	enum LevelBlockType {
		LEVEL_BLOCK_TYPE_SQUARE,
		LEVEL_BLOCK_TYPE_DEAD_END,
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
	static GLuint _textureSquare;
	static GLuint _textureDeadEnd;
	static GLuint _textureLinear;
	static GLuint _textureTType;
	static GLuint _textureCorner;
	static GLuint _textureCross;
};