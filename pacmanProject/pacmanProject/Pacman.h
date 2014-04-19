#ifndef _PACMAN_H_
#define _PACMAN_H_

#include <vector>

#include <gl\GL.h>
#include <gl\GLU.h>
#include <stdio.h>
#include "SOIL.h"

#include "Game.h"

class Pacman
{
public:
	Pacman();
	~Pacman();
	static bool init();
	static int start();

	static const std::vector<bool>& getKeysPressed();
	static bool getKeyPressed(unsigned int index);
	static void setKeyPressed(unsigned int index, bool value);

	static bool getAppActive();
	static void setAppActive(bool value);

	static bool getFullScreen();
	static void setFullScreen(bool value);

	static LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );        // Прототип функции WndProc
protected:
	static GLvoid resizeGLScene( GLsizei width, GLsizei height);
	static GLvoid killGLWindow();
	static int loadGLTextures(GLuint& texture, const char* fileName);
	static unsigned char* loadImage(const char* fileName);
	static bool createGLTexturesFromImage(unsigned char* image);
	static bool drawGLScene();
	static bool initGL();
	static bool createGLWindow(LPCWSTR title, int width, int height, int bits, bool fullscreenFlag);
private:

	struct Star {
		int r, g, b; // Цвета
		GLfloat dist; // Расстояние от центра
		GLfloat angle; // Текущий угол звезды
	};

	static const unsigned int NUMBER_OF_KEYS; // число клавиш на клавиатуре (размер вектора keysPressed)
	static const unsigned int NUMBER_OF_TEXTURES = 3; // число текстур
	static const GLfloat PERSPECTIVE_CLOSE;
	static const GLfloat PERSPECTIVE_FAR;
	static const GLfloat PERSPECTIVE_FOV;

	static std::vector<bool> _keysPressed; // Массив, используемый для операций с клавиатурой
	static bool _appActive;			// Флаг активности окна, установленный в true по умолчанию
	static bool _fullScreen;		// Флаг режима окна, установленный в полноэкранный по умолчанию

	static HGLRC _hRC;				// Постоянный контекст рендеринга
	static HDC _hDC;				// Приватный контекст устройства GDI
	static HWND _hWnd;				// Здесь будет храниться дескриптор окна
	static HINSTANCE _hInstance;	// Здесь будет храниться дескриптор приложения

	static GLfloat _rtri;           // Угол для треугольник
	static GLfloat _rquad;          // Угол для четырехугольника

	static GLfloat	_xrot;			// Вращение X
	static GLfloat	_yrot;			// Y
	static GLfloat	_zrot;			// Z

	static GLfloat	_xspeed;		// Y
	static GLfloat	_yspeed;		// Z

	static BOOL _light;				// Свет ВКЛ / ВЫКЛ
	static BOOL _blend;				// Смешивание ВКЛ / ВЫКЛ
	static BOOL _twinkle;			// Twinkling Stars (Вращающиеся звезды)
	static BOOL _tp;                // 'T' клавиша нажата?
	static BOOL _lp;				// L нажата?
	static BOOL _fp;				// F нажата?
	static BOOL _bp;				// B нажата?
	static BOOL _done;				// Закончена ли работа приложения

	static GLfloat _zoom;                    // Расстояние от наблюдателя до звезд
	static GLfloat _tilt;                    // Начальный угол
	static GLfloat _spin;                    // Для вращения звезд

	// Звезды
	static const int STARS_NUMBER = 50;
	static Star stars[];

	static GLfloat _z;				// Сдвиг вглубь экрана

	static GLfloat _lightAmbient[]; // Значения фонового света ( НОВОЕ )

	static GLfloat _lightDiffuse[]; // Значения диффузного света ( НОВОЕ )

	static GLfloat _lightPosition[];     // Позиция света ( НОВОЕ )

	static GLuint _filter;         // Какой фильтр использовать

	static GLuint _texture[NUMBER_OF_TEXTURES];

	static unsigned char* _image; // Картинка для создания текстур
	static int _imageWidth;
	static int _imageHeight;
	static int* _imageChannels;
	static Game* _game;

	static bool handleKeyboardPress();
	static void initStars();
	static void checkLight();
	static void checkBlend();
	static void checkStars();
	static void drawStars();
	static void drawCube();
};

#endif