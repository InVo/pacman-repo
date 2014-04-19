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

	static LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );        // �������� ������� WndProc
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
		int r, g, b; // �����
		GLfloat dist; // ���������� �� ������
		GLfloat angle; // ������� ���� ������
	};

	static const unsigned int NUMBER_OF_KEYS; // ����� ������ �� ���������� (������ ������� keysPressed)
	static const unsigned int NUMBER_OF_TEXTURES = 3; // ����� �������
	static const GLfloat PERSPECTIVE_CLOSE;
	static const GLfloat PERSPECTIVE_FAR;
	static const GLfloat PERSPECTIVE_FOV;

	static std::vector<bool> _keysPressed; // ������, ������������ ��� �������� � �����������
	static bool _appActive;			// ���� ���������� ����, ������������� � true �� ���������
	static bool _fullScreen;		// ���� ������ ����, ������������� � ������������� �� ���������

	static HGLRC _hRC;				// ���������� �������� ����������
	static HDC _hDC;				// ��������� �������� ���������� GDI
	static HWND _hWnd;				// ����� ����� ��������� ���������� ����
	static HINSTANCE _hInstance;	// ����� ����� ��������� ���������� ����������

	static GLfloat _rtri;           // ���� ��� �����������
	static GLfloat _rquad;          // ���� ��� ����������������

	static GLfloat	_xrot;			// �������� X
	static GLfloat	_yrot;			// Y
	static GLfloat	_zrot;			// Z

	static GLfloat	_xspeed;		// Y
	static GLfloat	_yspeed;		// Z

	static BOOL _light;				// ���� ��� / ����
	static BOOL _blend;				// ���������� ��� / ����
	static BOOL _twinkle;			// Twinkling Stars (����������� ������)
	static BOOL _tp;                // 'T' ������� ������?
	static BOOL _lp;				// L ������?
	static BOOL _fp;				// F ������?
	static BOOL _bp;				// B ������?
	static BOOL _done;				// ��������� �� ������ ����������

	static GLfloat _zoom;                    // ���������� �� ����������� �� �����
	static GLfloat _tilt;                    // ��������� ����
	static GLfloat _spin;                    // ��� �������� �����

	// ������
	static const int STARS_NUMBER = 50;
	static Star stars[];

	static GLfloat _z;				// ����� ������ ������

	static GLfloat _lightAmbient[]; // �������� �������� ����� ( ����� )

	static GLfloat _lightDiffuse[]; // �������� ���������� ����� ( ����� )

	static GLfloat _lightPosition[];     // ������� ����� ( ����� )

	static GLuint _filter;         // ����� ������ ������������

	static GLuint _texture[NUMBER_OF_TEXTURES];

	static unsigned char* _image; // �������� ��� �������� �������
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