#include "stdafx.h"
#include "Pacman.h"
#include <windows.h>
#include <cassert>

const unsigned int Pacman::NUMBER_OF_KEYS = 256;
const GLfloat Pacman::PERSPECTIVE_CLOSE = 0.1f;
const GLfloat Pacman::PERSPECTIVE_FAR = 100.0f;
const GLfloat Pacman::PERSPECTIVE_FOV = 60.f;

bool Pacman::_appActive = false;
bool Pacman::_fullScreen = false;
GLfloat Pacman::_rtri = 0.f;
GLfloat Pacman::_rquad = 0.f;
GLfloat Pacman::_zoom = -15.f;
GLfloat Pacman:: _tilt = 0.f;
GLfloat Pacman:: _spin = 1.f;

std::vector<bool> Pacman::_keysPressed;
GLuint Pacman::_texture[NUMBER_OF_TEXTURES];
GLuint Pacman::_filter = 0;

unsigned char* Pacman::_image = 0; // �������� ��� �������� �������
int Pacman::_imageWidth = 0;
int Pacman::_imageHeight = 0;
int* Pacman::_imageChannels = 0;

Pacman::Star Pacman::stars[STARS_NUMBER];

GLfloat Pacman::_xrot = 0.f;
GLfloat Pacman::_yrot = 0.f;
GLfloat Pacman::_zrot = 0.f;

GLfloat Pacman::_xspeed = 0.f;
GLfloat Pacman::_yspeed = 0.f;

GLfloat Pacman::_lightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };

GLfloat Pacman::_lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat Pacman::_lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f }; 

GLfloat Pacman::_z = -6.f;

BOOL Pacman::_light = false;      // ���� ��� / ����
BOOL Pacman::_blend = false;	  // ���������� ��� / ����
BOOL Pacman::_twinkle = false;	  // Twinkling stars
BOOL Pacman::_lp = false;         // L ������?
BOOL Pacman::_fp = false;         // F ������?
BOOL Pacman::_bp = false;         // B ������?
BOOL Pacman::_tp = false;		  // T ������?
BOOL Pacman::_done = false;

HGLRC Pacman::_hRC;
HDC Pacman::_hDC;
HWND Pacman::_hWnd;
HINSTANCE Pacman::_hInstance;

Game* Pacman::_game = 0;

Pacman::Pacman()
{

}

Pacman::~Pacman()
{
	if (_game) {
		delete _game;
	}
}

bool Pacman::init()
{
     _hRC = nullptr;              // ���������� �������� ����������
     _hDC = nullptr;				// ��������� �������� ���������� GDI
     _hWnd = nullptr;              // ����� ����� �������� ���������� ����

	 _keysPressed = std::vector<bool>(NUMBER_OF_KEYS);
	 _appActive = true;
	 _fullScreen = true;

	 _rtri = 0.f;
	 _rquad = 0.f;
	 return true;
}

int Pacman::start()
{
	init();
	_game = new Game();
	// ���������� ������������, ����� ����� ������ �� ������������
	if( MessageBox( nullptr, _T("������ �� �� ��������� ���������� � ������������� ������?"),  
						_T("��������� � ������������� ������?"), MB_YESNO | MB_ICONQUESTION) == IDNO )
	{
		_fullScreen = false;          // ������� �����
	} 

	if( !createGLWindow( _T("NeHe OpenGL ����"), 1024, 768, 32, _fullScreen ) )
	{
		return 0;           // �����, ���� ���� �� ����� ���� �������
	}

	MSG  msg;						// ��������� ��� �������� ��������� Windows

	while(!_done) {
		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) ){    // ���� �� � ������� �����-������ ���������?
			
			if (msg.message == WM_QUIT) {
				_done = true;
			} else {
				TranslateMessage( &msg );        // ��������� ���������
				DispatchMessage( &msg );        // �������� ���������
			}

		} else {
			if (_appActive) {

				if (!_done) {
					drawGLScene();
					SwapBuffers(_hDC);
				}
			}
			if (!handleKeyboardPress()){
				return 0;
			}
		}
	}

	killGLWindow();
	return msg.wParam;
}

bool Pacman::handleKeyboardPress()
{
	if (_keysPressed[VK_ESCAPE]) {
		_done = true;
	}
	if (_keysPressed['L'] && !_lp) {
		_lp = TRUE;
		_light = !_light;
		checkLight();
	}

	if (!_keysPressed['L']) {
		_lp = FALSE;
	}

	if (_keysPressed['F'] && !_fp) {
		_fp = TRUE;
		if (_filter >= NUMBER_OF_TEXTURES - 1) {
			_filter = 0;
		} else {
			++_filter;
		}
	}

	if (!_keysPressed['F']){
		_fp = FALSE;
	}

	if (_keysPressed['B'] && !_bp) {
		_bp = TRUE;
		_blend = !_blend;
		checkBlend();
	}

	if (!_keysPressed['B']){
		_bp = FALSE;
	}

	if (_keysPressed['T'] && !_tp) {
		_tp = TRUE;
		_twinkle = !_twinkle;
	}

	if (!_keysPressed['T']){
		_tp = FALSE;
	}

    if (_keysPressed[VK_PRIOR])   // ������� 'Page Up' ������?
    {
		_z-=0.02f;            // ���� ���, �� ������� ������ ������
    }

    if (_keysPressed[VK_NEXT])    // ������� 'Page Down' ������?
    {
		_z+=0.02f;            // ���� ���, �� ��������� � �����������
    }

    if (_keysPressed[VK_UP])     // ������� ������� ����� ������?
    {
		_xspeed-=0.01f;      // ���� ���, �� �������� xspeed
    }
    if (_keysPressed[VK_DOWN])   // ������� ������� ���� ������?
    {
		_xspeed+=0.01f;      // ���� ���, �� �������� xspeed
    }
    if (_keysPressed[VK_RIGHT])  // ������� ������� ������ ������?
    {
		_yspeed+=0.01f;      // ���� ���, �� �������� yspeed
    }
    if (_keysPressed[VK_LEFT])   // ������� ������� ����� ������?
    {
		_yspeed-=0.01f;      // ���� ���, �� �������� yspeed
    }



	if (_keysPressed[VK_F1]) {
		_keysPressed[VK_F1] = false;
		killGLWindow();
		_fullScreen = !_fullScreen;

		// ����������� ����
		if( !createGLWindow( _T("NeHe OpenGL ����"), 1024, 768, 32, _fullScreen ) )
		{
			return false;           // �����, ���� ���� �� ����� ���� �������
		}				
	}

	if (_keysPressed[VK_F2]) {
		_keysPressed[VK_F2] = false;
		_done = true;			
	}

	return true;
}

void Pacman::checkLight()
{
	_light ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
}

void Pacman::checkBlend()
{
    if(_blend)                        // blend TRUE?
    {
        glEnable(GL_BLEND);        // �������� ����������
        glDisable(GL_DEPTH_TEST);  // ��������� ���� �������
    }
    else                            
    {
        glDisable(GL_BLEND);        // ��������� ����������
        glEnable(GL_DEPTH_TEST);    // �������� ���� �������
    }
}

bool Pacman::initGL()
{
/*	_image = loadImage("Data/Glass.bmp");
	if (!_image) {
		return false;
	}

	if (!createGLTexturesFromImage(_image)) {
		return false;
	}*/

	glEnable(GL_TEXTURE_2D);                        // Enable Texture Mapping
	glShadeModel( GL_SMOOTH );						// ��������� ������� �������� �����������
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);           // ������� ������ � ������ ����
    glClearDepth( 1.0f );							// ��������� ������� ������ �������
//  glEnable( GL_DEPTH_TEST );					// ��������� ���� �������
//  glDepthFunc( GL_LEQUAL );						// ��� ����� �������

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );      // ��������� � ���������� �����������

	glLightfv(GL_LIGHT1, GL_AMBIENT, _lightAmbient);		// ���������� ������� ����
	glLightfv(GL_LIGHT1, GL_DIFFUSE, _lightDiffuse);		// ���������� ��������� ����
	glLightfv(GL_LIGHT1, GL_POSITION, _lightPosition);		// ��������� ��������� �����

	glEnable(GL_LIGHT1);

	glColor4f(1.0f,1.0f,1.0f,0.5f);				// ������ �������, 50% �����
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);	// ������� ���������� ��� ��������������,
												// ������������ �� �������� �����
	glEnable(GL_BLEND);
//	initStars();
	return true;                // ������������� ������ �������
}

void Pacman::initStars()
{
	for(int i = 0; i < STARS_NUMBER; ++i) {
		stars[i].dist = (float(i)/STARS_NUMBER) *5.f;
		stars[i].angle = (float(i)/STARS_NUMBER) * 360.f;
		stars[i].r = rand() % 256;
		stars[i].g = rand() % 256;
		stars[i].b = rand() % 256;
	}
}

bool Pacman::drawGLScene()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );      // �������� ����� � ����� �������
    glLoadIdentity();              // �������� ������� �������

	// PLACE DRAW CODE HERE

/*	glTranslatef(0.f,0.f, _z);
	glRotatef(_xrot,1.0f,0.0f,0.0f);                     // Rotate On The X Axis
	glRotatef(_yrot,0.0f,1.0f,0.0f);                     // Rotate On The Y Axis
	glRotatef(_zrot,0.0f,0.0f,1.0f);                     // Rotate On The Z Axis
*/
	
//	glBindTexture(GL_TEXTURE_2D, _texture[_filter]);               // Select Our Texture

//	drawStars();

//    _xrot+=_xspeed;                             // X Axis Rotation
//    _yrot+=_yspeed;                             // Y Axis Rotation
//	drawCube();
	_game->render();

    return true;                // ���������� ������ �������
}

void Pacman::drawStars()
{
	for (int i = 0; i < STARS_NUMBER; ++i) {
		// �������� ������� ������� (Model Matrix) ����� ������ �������
		glLoadIdentity();
		// ��������� �� ��� z �� 'zoom'
		glTranslatef(0.0f, 0.0f, _zoom);
		// ������� ������ ��� x �� ���� 'tilt'
		glRotatef(_tilt,1.0f,0.0f,0.0f);

        // ������������ �� ���� ������ ������ ��� y
        glRotatef(stars[i].angle, 0.0f, 0.0f, 1.0f);
        // ��������� ������ �� ��� x
        glTranslatef(stars[i].dist, 0.0f, 0.0f);

		// �������� ������� ����� ����, ��� ������������ �� ��� X
        glRotatef(-stars[i].angle,0.0f,0.0f,1.0f);
        // �������� ������� ������� ������
        glRotatef(-_tilt,1.0f,0.0f,0.0f);        // �������� ������� ������

		if (_twinkle) {
			glColor4ub(stars[(STARS_NUMBER-i)-1].r,stars[(STARS_NUMBER-i)-1].g,stars[(STARS_NUMBER-i)-1].b,255);
			glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);				
			glEnd();
		}

		glRotatef(_spin,0.0f,0.0f,1.0f);// ������������ ������ �� ��� z
		glColor4ub(stars[i].r, stars[i].g, stars[i].b, 255);

        glBegin(GL_QUADS);              // �������� �������� ���������� �������
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
        glEnd();                                        // ��������� ��������

        _spin += 0.01f;								// ������������ ��� �������� ������
        stars[i].angle += 0.5f;//float(i)/STARS_NUMBER;	// ������ ���� ������
        stars[i].dist -= 0.01f;						// ������ ��������� �� ������

        if (stars[i].dist < 0.0f)    // ������ � ������ ������?
        {
                stars[i].dist += 5.0f;  // ���������� �� 5 ������ �� ������
                // ����� �������� ������� ���������� �����
                stars[i].r = rand()%256;
                // ����� �������� �������� ���������� �����
                stars[i].g = rand()%256;
                // ����� �������� ����� ���������� �����
                stars[i].b = rand()%256;
        }
	}
}

void Pacman::drawCube()
{
	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);     // ������� ��������� �� �����������
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
		// Back Face
		glNormal3f( 0.0f, 0.0f, -1.0f);     // ������� ��������� �� �����������
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
		// Top Face
		glNormal3f( 0.0f, 1.0f, 0.0f);     // ������� ��������� �� �����������
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
		// Bottom Face
		glNormal3f( 0.0f, -1.0f, 0.0f);     // ������� ��������� �� �����������
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
		// Right face
		glNormal3f( 1.0f, 0.0f, 0.0f);     // ������� ��������� �� �����������
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
		// Left Face
		glNormal3f( -1.0f, 0.0f, 0.0f);     // ������� ��������� �� �����������
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
	glEnd();
}

GLvoid Pacman::killGLWindow()
{
	if (_fullScreen) {
        ChangeDisplaySettings( nullptr, 0 );        // ���� ��, �� ������������� ������� � ������� �����
        ShowCursor( true );            // �������� ������ �����
	}

	// ����������� Render Context
	if (_hRC) {
		if( !wglMakeCurrent( nullptr, nullptr ) ) {       // �������� �� ���������� RC � DC?
			MessageBox( nullptr, L"Release Of DC And RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		}

		if (!wglDeleteContext(_hRC)) {
			MessageBox( nullptr, L"Deleting RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		}
		_hRC = nullptr;              // ���������� RC � NULL
	}

	// ����������� Device Context
    if( _hDC && !ReleaseDC( _hWnd, _hDC ) )          // �������� �� ���������� DC?
     {
        MessageBox( nullptr, L"Release Device Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        _hDC = nullptr;                // ���������� DC � NULL
     }

	// ����������� ���������� ����
	if(_hWnd && !DestroyWindow(_hWnd))            // �������� �� ���������� ����?
	{
	MessageBox( nullptr, L"Could Not Release hWnd.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
	_hWnd = nullptr;                // ���������� hWnd � NULL
	}

	// ��������������� �����
	if( !UnregisterClass( L"OpenGL", _hInstance ) ) {        // �������� �� ����������������� �����
		MessageBox( nullptr, L"Could Not Unregister Class.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		_hInstance = nullptr;                // ���������� hInstance � NULL
	}
}

int Pacman::loadGLTextures(GLuint& texture, const char* fileName)
{
	// �������� ��������
    /* load an image file directly as a new OpenGL texture */
    texture = SOIL_load_OGL_texture
        (
        fileName,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );

    if(texture == 0)
        return false;
 
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 
    return true;                                        // Return Success	
}

unsigned char* Pacman::loadImage(const char* fileName)
{
	return SOIL_load_image(fileName, &_imageWidth, &_imageHeight, 0, SOIL_LOAD_RGB);
}

bool Pacman::createGLTexturesFromImage(unsigned char* image)
{
	// �������� �������� � �������� �� �������� ��������
	_texture[0] = SOIL_create_OGL_texture(_image, _imageWidth, _imageHeight, 0, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (_texture[0] == 0) {
		return false;
	}
	glBindTexture(GL_TEXTURE_2D, _texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // ( ����� )
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // ( ����� )
	glTexImage2D(GL_TEXTURE_2D, 0, 3, _imageWidth, _imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, _image);

	_texture[1] = SOIL_create_OGL_texture(_image, _imageWidth, _imageHeight, 0, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (_texture[1] == 0) {
		return false;
	}
	glBindTexture(GL_TEXTURE_2D, _texture[1]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, _imageWidth, _imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, _image);


	_texture[2] = SOIL_create_OGL_texture(_image, _imageWidth, _imageHeight, 0, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (_texture[2] == 0) {
		return false;
	}
	glBindTexture(GL_TEXTURE_2D, _texture[2]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, _imageWidth, _imageHeight,
						GL_RGB, GL_UNSIGNED_BYTE, _image); // ( ����� )

	SOIL_free_image_data(_image);
	return true;
}

bool Pacman::createGLWindow(LPCWSTR title, int width, int height, int bits, bool fullscreenFlag)
{
	GLuint pixelFormat;				// ������ ��������� ����� ������ ������� ��������
	WNDCLASS wc;					// ��������� ������ ����
	DWORD    dwExStyle;				// ����������� ����� ����
    DWORD    dwStyle;				// ������� ����� ����

	RECT windowRect;						// �������� �������� ������ �������� � ������� ������� ����� ����
	windowRect.left=(long)0;				// ���������� ����� ������������ � 0
	windowRect.right=(long)width;           // ���������� ������ ������������ � Width
	windowRect.top=(long)0;					// ���������� ������� ������������ � 0
	windowRect.bottom=(long)height;			// ���������� ������ ������������ � Height
	
	_fullScreen = fullscreenFlag;

	_hInstance    = GetModuleHandle(NULL);					// ������� ���������� ������ ����������
	wc.style    = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// ���������� ��� ����������� � ������ ������� DC
	wc.lpfnWndProc    = (WNDPROC) Pacman::WndProc;					// ��������� ��������� ���������
	wc.cbClsExtra    = 0;									// ��� �������������� ���������� ��� ����
	wc.cbWndExtra    = 0;									// ��� �������������� ���������� ��� ����
	wc.hInstance    = _hInstance;							// ������������� ����������
	wc.hIcon    = LoadIcon(nullptr, IDI_WINLOGO);				// ��������� ������ �� ���������
	wc.hCursor    = LoadCursor(nullptr, IDC_ARROW);			// ��������� ��������� �����
	wc.hbrBackground  = nullptr;								// ��� �� ��������� ��� GL
	wc.lpszMenuName    = nullptr;								// ���� � ���� �� �����
	wc.lpszClassName  = L"OpenGL";							// ������������� ��� ������

	if (!RegisterClass(&wc)) {
		MessageBox( nullptr, L"Failed to register the window class.", L"ERROR", MB_OK | MB_ICONINFORMATION);
		return false;
	}

	if (_fullScreen) {
		DEVMODE dmScreenSettings;													// ����� ����������
		memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );					// ������� ��� �������� ���������
		dmScreenSettings.dmSize=sizeof( dmScreenSettings );							// ������ ��������� Devmode
		dmScreenSettings.dmPelsWidth  =   width;									// ������ ������
		dmScreenSettings.dmPelsHeight  =   height;									// ������ ������
		dmScreenSettings.dmBitsPerPel  =   bits;									// ������� �����
		dmScreenSettings.dmFields= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;	// ����� �������

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			if( MessageBox( nullptr, L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",
				L"NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES ){
				_fullScreen = false;
			} else {
				// ������������� ����, ���������� ������������ � �������� ����.
				MessageBox( nullptr, L"Program Will Now Close.", L"ERROR", MB_OK | MB_ICONSTOP );
				return false;            // ����� � ����������� �������� false
			}
		}
	}

	if (_fullScreen) {
		dwExStyle  =   WS_EX_APPWINDOW;			// ����������� ����� ����
		dwStyle    =   WS_POPUP;				// ������� ����� ����
		ShowCursor( false );					// ������ ��������� �����
	} else {
		dwExStyle  =   WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;      // ����������� ����� ����
		dwStyle    =   WS_OVERLAPPEDWINDOW;						// ������� ����� ����
	}

	AdjustWindowRectEx(&windowRect, dwStyle, false, dwExStyle );

	if( !( _hWnd = CreateWindowEx(  dwExStyle,          // ����������� ����� ��� ����
		_T("OpenGL"),									// ��� ������
		title,											// ��������� ����
		WS_CLIPSIBLINGS |								// ��������� ����� ��� ����
		WS_CLIPCHILDREN |								// ��������� ����� ��� ����
		dwStyle,										// ���������� ����� ��� ����
		0, 0,											// ������� ����
		windowRect.right - windowRect.left,				// ���������� ���������� ������
		windowRect.bottom - windowRect.top,				// ���������� ���������� ������
		nullptr,										// ��� �������������
		nullptr,										// ��� ����
		_hInstance,										// ���������� ����������
		nullptr ) ) ){									// �� ������� ������ �� WM_CREATE (???)  

		killGLWindow();                // ������������ �����
		MessageBox( NULL, _T("Window Creation Error."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
		return false;                // ������� false
	  }

	static  PIXELFORMATDESCRIPTOR pfd=				// pfd �������� Windows ����� ����� ����� �� ����� ������� �������
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// ������ ����������� ������� ������� ��������
		1,											// ����� ������
		PFD_DRAW_TO_WINDOW |						// ������ ��� ����
		PFD_SUPPORT_OPENGL |						// ������ ��� OpenGL
		PFD_DOUBLEBUFFER,							// ������ ��� �������� ������
		PFD_TYPE_RGBA,								// ��������� RGBA ������
		bits,										// ���������� ��� ������� �����
		0, 0, 0, 0, 0, 0,							// ������������� �������� �����
		0,											// ��� ������ ������������
		0,											// ��������� ��� ������������
		0,											// ��� ������ ����������
		0, 0, 0, 0,									// ���� ���������� ������������
		32,											// 32 ������ Z-����� (����� �������)
		0,											// ��� ������ ���������
		0,											// ��� ��������������� �������
		PFD_MAIN_PLANE,								// ������� ���� ���������
		0,											// ���������������
		0, 0, 0										// ����� ���� ������������
	};

	if( !( _hDC = GetDC( _hWnd ) ) )				// ����� �� �� �������� �������� ����������?
	{
		killGLWindow();								// ������������ �����
		MessageBox( NULL, _T("Can't Create A GL Device Context."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
		return false;								// ������� false
	}

	if( !( pixelFormat = ChoosePixelFormat( _hDC, &pfd ) ) )        // ������ �� ���������� ������ �������?
	{
		killGLWindow();								// ������������ �����
		MessageBox( nullptr, _T("Can't Find A Suitable PixelFormat."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
		return false;								// ������� false
	}

	if( !SetPixelFormat( _hDC, pixelFormat, &pfd ) )          // �������� �� ���������� ������ �������?
	{
		killGLWindow();                // ������������ �����
		MessageBox( nullptr, _T("Can't Set The PixelFormat."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
		return false;                // ������� false
	}

	if( !( _hRC = wglCreateContext( _hDC ) ) )          // �������� �� ���������� �������� ����������?
	{
		killGLWindow();                // ������������ �����
		MessageBox( nullptr, _T("Can't Create A GL Rendering Context."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return false;                // ������� false
	}

	if( !wglMakeCurrent( _hDC, _hRC ) )            // ����������� ������������ �������� ����������
	{
		killGLWindow();                // ������������ �����
		MessageBox( nullptr, _T("Can't Activate The GL Rendering Context."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
		return false;                // ������� false
	}

	ShowWindow( _hWnd, SW_SHOW );				// �������� ����
	SetForegroundWindow( _hWnd );				// ������ ������� ���������
	SetFocus( _hWnd );							// ���������� ����� ���������� �� ���� ����
	resizeGLScene( width, height );             // �������� ����������� ��� ������ OpenGL ������.

	if( !initGL() )                  // ������������� ������ ��� ���������� ����
	{
		killGLWindow();                // ������������ �����
		MessageBox( nullptr, _T("Initialization Failed."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
		return false;                // ������� false
	}



	return true;
}

GLvoid Pacman::resizeGLScene( GLsizei width, GLsizei height)
{
    if( height == 0 )              // �������������� ������� �� ����
    {
        height = 1;
    }
    
    glViewport( 0, 0, width, height );          // ����� ������� ������� ������

    glMatrixMode( GL_PROJECTION );            // ����� ������� ��������
    glLoadIdentity();              // ����� ������� ��������
 
    // ���������� ����������� �������������� �������� ��� ����
    gluPerspective( PERSPECTIVE_FOV, (GLfloat)width/(GLfloat)height, PERSPECTIVE_CLOSE, PERSPECTIVE_FAR );
 
    glMatrixMode( GL_MODELVIEW );            // ����� ������� ���� ������
    glLoadIdentity();              // ����� ������� ���� ������

}

const std::vector<bool>& Pacman::getKeysPressed() {
	return _keysPressed;
}

bool Pacman::getKeyPressed(unsigned int index) {
	assert(index < _keysPressed.size());
	return _keysPressed[index];
}

void Pacman::setKeyPressed(unsigned int index, bool value) {
	assert(index < _keysPressed.size());
	_keysPressed[index] = value;
}

LRESULT CALLBACK Pacman::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg) {
		case WM_ACTIVATE:            // �������� ��������� ���������� ����
		  if( !HIWORD( wParam ) )        // ��������� ��������� �����������
		  {
			_appActive = true;          // ��������� �������
		  }
		  else
		  {
			_appActive = false;          // ��������� ������ �� �������
		  }
 
		  return 0;            // ������������ � ���� ��������� ���������
		  break;
		case WM_SYSCOMMAND:            // ������������� ��������� �������
		  switch ( wParam )            // ������������� ��������� �����
		  {
			case SC_SCREENSAVE:        // �������� �� ����������� �����������?
			case SC_MONITORPOWER:        // �������� �� ������� ������� � ����� ���������� �������?
			return 0;          // ������������� ���
		  }
		  break;              
		case WM_CLOSE:              // �� �������� ��������� � ��������?
		  PostQuitMessage( 0 );          // ��������� ��������� � ������
		  return 0;            
		case WM_KEYDOWN:            // ���� �� ������ ������
			_keysPressed[wParam] = true;          // ���� ���, �� ����������� ���� ������ true
			return 0;            
		case WM_KEYUP:				// ���� �� �������� ������
			_keysPressed[wParam] = false; // ���� ���, ����������� ���� ������ false
			return 0;
		case WM_SIZE:
			resizeGLScene( LOWORD(lParam), HIWORD(lParam) );  // ������� �����=Width, ������� �����=Height
			return 0;            // ������������
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}