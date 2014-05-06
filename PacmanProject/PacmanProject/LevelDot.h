#include "Eatable.h"
#include "LevelObject.h"

class LevelDot: public LevelObject, Eatable
{
public:
	LevelDot();
	virtual void onEat(void* data);
	virtual void init();
protected:
	virtual void draw();

private:
	static const std::string FILE_LEVEL_DOT_TEXTURE;
	static GLuint _textureDot;
};