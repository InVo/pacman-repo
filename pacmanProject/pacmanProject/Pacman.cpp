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

unsigned char* Pacman::_image = 0; // Картинка для создания текстур
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

BOOL Pacman::_light = false;      // Свет ВКЛ / ВЫКЛ
BOOL Pacman::_blend = false;	  // Смешивание ВКЛ / ВЫКЛ
BOOL Pacman::_twinkle = false;	  // Twinkling stars
BOOL Pacman::_lp = false;         // L нажата?
BOOL Pacman::_fp = false;         // F нажата?
BOOL Pacman::_bp = false;         // B нажата?
BOOL Pacman::_tp = false;		  // T нажата?
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
     _hRC = nullptr;              // Постоянный контекст рендеринга
     _hDC = nullptr;				// Приватный контекст устройства GDI
     _hWnd = nullptr;              // Здесь будет хранится дескриптор окна

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
	// Спрашивает пользователя, какой режим экрана он предпочитает
	if( MessageBox( nullptr, _T("Хотите ли Вы запустить приложение в полноэкранном режиме?"),  
						_T("Запустить в полноэкранном режиме?"), MB_YESNO | MB_ICONQUESTION) == IDNO )
	{
		_fullScreen = false;          // Оконный режим
	} 

	if( !createGLWindow( _T("NeHe OpenGL окно"), 1024, 768, 32, _fullScreen ) )
	{
		return 0;           // Выйти, если окно не может быть создано
	}

	MSG  msg;						// Структура для хранения сообщения Windows

	while(!_done) {
		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) ){    // Есть ли в очереди какое-нибудь сообщение?
			
			if (msg.message == WM_QUIT) {
				_done = true;
			} else {
				TranslateMessage( &msg );        // Переводим сообщение
				DispatchMessage( &msg );        // Отсылаем сообщение
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

    if (_keysPressed[VK_PRIOR])   // Клавиша 'Page Up' нажата?
    {
		_z-=0.02f;            // Если так, то сдвинем вглубь экрана
    }

    if (_keysPressed[VK_NEXT])    // Клавиша 'Page Down' нажата?
    {
		_z+=0.02f;            // Если так, то придвинем к наблюдателю
    }

    if (_keysPressed[VK_UP])     // Клавиша стрелка вверх нажата?
    {
		_xspeed-=0.01f;      // Если так, то уменьшим xspeed
    }
    if (_keysPressed[VK_DOWN])   // Клавиша стрелка вниз нажата?
    {
		_xspeed+=0.01f;      // Если так, то увеличим xspeed
    }
    if (_keysPressed[VK_RIGHT])  // Клавиша стрелка вправо нажата?
    {
		_yspeed+=0.01f;      // Если так, то увеличим yspeed
    }
    if (_keysPressed[VK_LEFT])   // Клавиша стрелка влево нажата?
    {
		_yspeed-=0.01f;      // Если так, то уменьшим yspeed
    }



	if (_keysPressed[VK_F1]) {
		_keysPressed[VK_F1] = false;
		killGLWindow();
		_fullScreen = !_fullScreen;

		// Пересоздаем окно
		if( !createGLWindow( _T("NeHe OpenGL окно"), 1024, 768, 32, _fullScreen ) )
		{
			return false;           // Выйти, если окно не может быть создано
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
        glEnable(GL_BLEND);        // Включаем смешивание
        glDisable(GL_DEPTH_TEST);  // Выключаем тест глубины
    }
    else                            
    {
        glDisable(GL_BLEND);        // Выключаем смешивание
        glEnable(GL_DEPTH_TEST);    // Включаем тест глубины
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
	glShadeModel( GL_SMOOTH );						// Разрешить плавное цветовое сглаживание
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);           // Очистка экрана в черный цвет
    glClearDepth( 1.0f );							// Разрешить очистку буфера глубины
//  glEnable( GL_DEPTH_TEST );					// Разрешить тест глубины
//  glDepthFunc( GL_LEQUAL );						// Тип теста глубины

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );      // Улучшение в вычислении перспективы

	glLightfv(GL_LIGHT1, GL_AMBIENT, _lightAmbient);		// Установить фоновый свет
	glLightfv(GL_LIGHT1, GL_DIFFUSE, _lightDiffuse);		// Установить диффузный свет
	glLightfv(GL_LIGHT1, GL_POSITION, _lightPosition);		// Положение источника света

	glEnable(GL_LIGHT1);

	glColor4f(1.0f,1.0f,1.0f,0.5f);				// Полная яркость, 50% альфа
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);	// Функция смешивания для непрозрачности,
												// базирующаяся на значении альфы
	glEnable(GL_BLEND);
//	initStars();
	return true;                // Инициализация прошла успешно
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
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );      // Очистить экран и буфер глубины
    glLoadIdentity();              // Сбросить текущую матрицу

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

    return true;                // Прорисовка прошла успешно
}

void Pacman::drawStars()
{
	for (int i = 0; i < STARS_NUMBER; ++i) {
		// Обнуляем видовую матрицу (Model Matrix) перед каждой звездой
		glLoadIdentity();
		// Переносим по оси z на 'zoom'
		glTranslatef(0.0f, 0.0f, _zoom);
		// Вращаем вокруг оси x на угол 'tilt'
		glRotatef(_tilt,1.0f,0.0f,0.0f);

        // Поворачиваем на угол звезды вокруг оси y
        glRotatef(stars[i].angle, 0.0f, 0.0f, 1.0f);
        // Двигаемся вперед по оси x
        glTranslatef(stars[i].dist, 0.0f, 0.0f);

		// отменяем поворот после того, как продвинулись по оси X
        glRotatef(-stars[i].angle,0.0f,0.0f,1.0f);
        // Отменяет текущий поворот звезды
        glRotatef(-_tilt,1.0f,0.0f,0.0f);        // Отменяет поворот экрана

		if (_twinkle) {
			glColor4ub(stars[(STARS_NUMBER-i)-1].r,stars[(STARS_NUMBER-i)-1].g,stars[(STARS_NUMBER-i)-1].b,255);
			glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);				
			glEnd();
		}

		glRotatef(_spin,0.0f,0.0f,1.0f);// Поворачиваем звезду по оси z
		glColor4ub(stars[i].r, stars[i].g, stars[i].b, 255);

        glBegin(GL_QUADS);              // Начинаем рисовать текстурный квадрат
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
        glEnd();                                        // Закончили рисовать

        _spin += 0.01f;								// Используется для вращения звезды
        stars[i].angle += 0.5f;//float(i)/STARS_NUMBER;	// Меняем угол звезды
        stars[i].dist -= 0.01f;						// Меняем растояние до центра

        if (stars[i].dist < 0.0f)    // Звезда в центре экрана?
        {
                stars[i].dist += 5.0f;  // Перемещаем на 5 единиц от центра
                // Новое значение красной компоненты цвета
                stars[i].r = rand()%256;
                // Новое значение зеленной компоненты цвета
                stars[i].g = rand()%256;
                // Новое значение синей компоненты цвета
                stars[i].b = rand()%256;
        }
	}
}

void Pacman::drawCube()
{
	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);     // Нормаль указывает на наблюдателя
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
		// Back Face
		glNormal3f( 0.0f, 0.0f, -1.0f);     // Нормаль указывает на наблюдателя
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
		// Top Face
		glNormal3f( 0.0f, 1.0f, 0.0f);     // Нормаль указывает на наблюдателя
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
		// Bottom Face
		glNormal3f( 0.0f, -1.0f, 0.0f);     // Нормаль указывает на наблюдателя
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
		// Right face
		glNormal3f( 1.0f, 0.0f, 0.0f);     // Нормаль указывает на наблюдателя
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
		// Left Face
		glNormal3f( -1.0f, 0.0f, 0.0f);     // Нормаль указывает на наблюдателя
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
	glEnd();
}

GLvoid Pacman::killGLWindow()
{
	if (_fullScreen) {
        ChangeDisplaySettings( nullptr, 0 );        // Если да, то переключаемся обратно в оконный режим
        ShowCursor( true );            // Показать курсор мышки
	}

	// Освобождаем Render Context
	if (_hRC) {
		if( !wglMakeCurrent( nullptr, nullptr ) ) {       // Возможно ли освободить RC и DC?
			MessageBox( nullptr, L"Release Of DC And RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		}

		if (!wglDeleteContext(_hRC)) {
			MessageBox( nullptr, L"Deleting RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		}
		_hRC = nullptr;              // Установить RC в NULL
	}

	// Освобождаем Device Context
    if( _hDC && !ReleaseDC( _hWnd, _hDC ) )          // Возможно ли уничтожить DC?
     {
        MessageBox( nullptr, L"Release Device Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        _hDC = nullptr;                // Установить DC в NULL
     }

	// Освобождаем дескриптор окна
	if(_hWnd && !DestroyWindow(_hWnd))            // Возможно ли уничтожить окно?
	{
	MessageBox( nullptr, L"Could Not Release hWnd.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
	_hWnd = nullptr;                // Установить hWnd в NULL
	}

	// Разрегистрируем класс
	if( !UnregisterClass( L"OpenGL", _hInstance ) ) {        // Возможно ли разрегистрировать класс
		MessageBox( nullptr, L"Could Not Unregister Class.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		_hInstance = nullptr;                // Установить hInstance в NULL
	}
}

int Pacman::loadGLTextures(GLuint& texture, const char* fileName)
{
	// Загрузка картинки
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
	// Создание текстуры с фильтром по соседним пикселям
	_texture[0] = SOIL_create_OGL_texture(_image, _imageWidth, _imageHeight, 0, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (_texture[0] == 0) {
		return false;
	}
	glBindTexture(GL_TEXTURE_2D, _texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // ( НОВОЕ )
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // ( НОВОЕ )
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
						GL_RGB, GL_UNSIGNED_BYTE, _image); // ( НОВОЕ )

	SOIL_free_image_data(_image);
	return true;
}

bool Pacman::createGLWindow(LPCWSTR title, int width, int height, int bits, bool fullscreenFlag)
{
	GLuint pixelFormat;				// Хранит результат после поиска формата пикселей
	WNDCLASS wc;					// Структура класса окна
	DWORD    dwExStyle;				// Расширенный стиль окна
    DWORD    dwStyle;				// Обычный стиль окна

	RECT windowRect;						// Содержит значения левого верхнего и правого нижнего углов окна
	windowRect.left=(long)0;				// Установить левую составляющую в 0
	windowRect.right=(long)width;           // Установить правую составляющую в Width
	windowRect.top=(long)0;					// Установить верхнюю составляющую в 0
	windowRect.bottom=(long)height;			// Установить нижнюю составляющую в Height
	
	_fullScreen = fullscreenFlag;

	_hInstance    = GetModuleHandle(NULL);					// Считаем дескриптор нашего приложения
	wc.style    = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// Перерисуем при перемещении и создаём скрытый DC
	wc.lpfnWndProc    = (WNDPROC) Pacman::WndProc;					// Процедура обработки сообщений
	wc.cbClsExtra    = 0;									// Нет дополнительной информации для окна
	wc.cbWndExtra    = 0;									// Нет дополнительной информации для окна
	wc.hInstance    = _hInstance;							// Устанавливаем дескриптор
	wc.hIcon    = LoadIcon(nullptr, IDI_WINLOGO);				// Загружаем иконку по умолчанию
	wc.hCursor    = LoadCursor(nullptr, IDC_ARROW);			// Загружаем указатель мышки
	wc.hbrBackground  = nullptr;								// Фон не требуется для GL
	wc.lpszMenuName    = nullptr;								// Меню в окне не будет
	wc.lpszClassName  = L"OpenGL";							// Устанавливаем имя классу

	if (!RegisterClass(&wc)) {
		MessageBox( nullptr, L"Failed to register the window class.", L"ERROR", MB_OK | MB_ICONINFORMATION);
		return false;
	}

	if (_fullScreen) {
		DEVMODE dmScreenSettings;													// Режим устройства
		memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );					// Очистка для хранения установок
		dmScreenSettings.dmSize=sizeof( dmScreenSettings );							// Размер структуры Devmode
		dmScreenSettings.dmPelsWidth  =   width;									// Ширина экрана
		dmScreenSettings.dmPelsHeight  =   height;									// Высота экрана
		dmScreenSettings.dmBitsPerPel  =   bits;									// Глубина цвета
		dmScreenSettings.dmFields= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;	// Режим Пикселя

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			if( MessageBox( nullptr, L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",
				L"NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES ){
				_fullScreen = false;
			} else {
				// Выскакивающее окно, сообщающее пользователю о закрытие окна.
				MessageBox( nullptr, L"Program Will Now Close.", L"ERROR", MB_OK | MB_ICONSTOP );
				return false;            // Выход и возвращение функцией false
			}
		}
	}

	if (_fullScreen) {
		dwExStyle  =   WS_EX_APPWINDOW;			// Расширенный стиль окна
		dwStyle    =   WS_POPUP;				// Обычный стиль окна
		ShowCursor( false );					// Скрыть указатель мышки
	} else {
		dwExStyle  =   WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;      // Расширенный стиль окна
		dwStyle    =   WS_OVERLAPPEDWINDOW;						// Обычный стиль окна
	}

	AdjustWindowRectEx(&windowRect, dwStyle, false, dwExStyle );

	if( !( _hWnd = CreateWindowEx(  dwExStyle,          // Расширенный стиль для окна
		_T("OpenGL"),									// Имя класса
		title,											// Заголовок окна
		WS_CLIPSIBLINGS |								// Требуемый стиль для окна
		WS_CLIPCHILDREN |								// Требуемый стиль для окна
		dwStyle,										// Выбираемые стили для окна
		0, 0,											// Позиция окна
		windowRect.right - windowRect.left,				// Вычисление подходящей ширины
		windowRect.bottom - windowRect.top,				// Вычисление подходящей высоты
		nullptr,										// Нет родительского
		nullptr,										// Нет меню
		_hInstance,										// Дескриптор приложения
		nullptr ) ) ){									// Не передаём ничего до WM_CREATE (???)  

		killGLWindow();                // Восстановить экран
		MessageBox( NULL, _T("Window Creation Error."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
		return false;                // Вернуть false
	  }

	static  PIXELFORMATDESCRIPTOR pfd=				// pfd сообщает Windows каким будет вывод на экран каждого пикселя
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Размер дескриптора данного формата пикселей
		1,											// Номер версии
		PFD_DRAW_TO_WINDOW |						// Формат для Окна
		PFD_SUPPORT_OPENGL |						// Формат для OpenGL
		PFD_DOUBLEBUFFER,							// Формат для двойного буфера
		PFD_TYPE_RGBA,								// Требуется RGBA формат
		bits,										// Выбирается бит глубины цвета
		0, 0, 0, 0, 0, 0,							// Игнорирование цветовых битов
		0,											// Нет буфера прозрачности
		0,											// Сдвиговый бит игнорируется
		0,											// Нет буфера накопления
		0, 0, 0, 0,									// Биты накопления игнорируются
		32,											// 32 битный Z-буфер (буфер глубины)
		0,											// Нет буфера трафарета
		0,											// Нет вспомогательных буферов
		PFD_MAIN_PLANE,								// Главный слой рисования
		0,											// Зарезервировано
		0, 0, 0										// Маски слоя игнорируются
	};

	if( !( _hDC = GetDC( _hWnd ) ) )				// Можем ли мы получить Контекст Устройства?
	{
		killGLWindow();								// Восстановить экран
		MessageBox( NULL, _T("Can't Create A GL Device Context."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
		return false;								// Вернуть false
	}

	if( !( pixelFormat = ChoosePixelFormat( _hDC, &pfd ) ) )        // Найден ли подходящий формат пикселя?
	{
		killGLWindow();								// Восстановить экран
		MessageBox( nullptr, _T("Can't Find A Suitable PixelFormat."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
		return false;								// Вернуть false
	}

	if( !SetPixelFormat( _hDC, pixelFormat, &pfd ) )          // Возможно ли установить Формат Пикселя?
	{
		killGLWindow();                // Восстановить экран
		MessageBox( nullptr, _T("Can't Set The PixelFormat."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
		return false;                // Вернуть false
	}

	if( !( _hRC = wglCreateContext( _hDC ) ) )          // Возможно ли установить Контекст Рендеринга?
	{
		killGLWindow();                // Восстановить экран
		MessageBox( nullptr, _T("Can't Create A GL Rendering Context."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return false;                // Вернуть false
	}

	if( !wglMakeCurrent( _hDC, _hRC ) )            // Попробовать активировать Контекст Рендеринга
	{
		killGLWindow();                // Восстановить экран
		MessageBox( nullptr, _T("Can't Activate The GL Rendering Context."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
		return false;                // Вернуть false
	}

	ShowWindow( _hWnd, SW_SHOW );				// Показать окно
	SetForegroundWindow( _hWnd );				// Слегка повысим приоритет
	SetFocus( _hWnd );							// Установить фокус клавиатуры на наше окно
	resizeGLScene( width, height );             // Настроим перспективу для нашего OpenGL экрана.

	if( !initGL() )                  // Инициализация только что созданного окна
	{
		killGLWindow();                // Восстановить экран
		MessageBox( nullptr, _T("Initialization Failed."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
		return false;                // Вернуть false
	}



	return true;
}

GLvoid Pacman::resizeGLScene( GLsizei width, GLsizei height)
{
    if( height == 0 )              // Предотвращение деления на ноль
    {
        height = 1;
    }
    
    glViewport( 0, 0, width, height );          // Сброс текущей области вывода

    glMatrixMode( GL_PROJECTION );            // Выбор матрицы проекций
    glLoadIdentity();              // Сброс матрицы проекции
 
    // Вычисление соотношения геометрических размеров для окна
    gluPerspective( PERSPECTIVE_FOV, (GLfloat)width/(GLfloat)height, PERSPECTIVE_CLOSE, PERSPECTIVE_FAR );
 
    glMatrixMode( GL_MODELVIEW );            // Выбор матрицы вида модели
    glLoadIdentity();              // Сброс матрицы вида модели

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
		case WM_ACTIVATE:            // Проверка сообщения активности окна
		  if( !HIWORD( wParam ) )        // Проверить состояние минимизации
		  {
			_appActive = true;          // Программа активна
		  }
		  else
		  {
			_appActive = false;          // Программа теперь не активна
		  }
 
		  return 0;            // Возвращаемся в цикл обработки сообщений
		  break;
		case WM_SYSCOMMAND:            // Перехватываем системную команду
		  switch ( wParam )            // Останавливаем системный вызов
		  {
			case SC_SCREENSAVE:        // Пытается ли запуститься скринсейвер?
			case SC_MONITORPOWER:        // Пытается ли монитор перейти в режим сбережения энергии?
			return 0;          // Предотвращаем это
		  }
		  break;              
		case WM_CLOSE:              // Мы получили сообщение о закрытии?
		  PostQuitMessage( 0 );          // Отправить сообщение о выходе
		  return 0;            
		case WM_KEYDOWN:            // Была ли нажата кнопка
			_keysPressed[wParam] = true;          // Если так, мы присваиваем этой ячейке true
			return 0;            
		case WM_KEYUP:				// Была ли отпущена кнопка
			_keysPressed[wParam] = false; // Если так, присваиваем этой ячейке false
			return 0;
		case WM_SIZE:
			resizeGLScene( LOWORD(lParam), HIWORD(lParam) );  // Младшее слово=Width, старшее слово=Height
			return 0;            // Возвращаемся
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}