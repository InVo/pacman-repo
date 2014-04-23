#include "Utils.h"
#include <windows.h>

int Utils::showMessage(LPCWSTR text)
{
	MessageBox( nullptr, text, L"Message", MB_OK | MB_ICONINFORMATION );
	return 0;
}