#include<Windows.h>

void setTopWindow(const char *windowName) {
	HWND hWnd = FindWindowA(NULL, windowName);
	SetForegroundWindow(hWnd);
	SwitchToThisWindow(hWnd, TRUE);
	SetFocus(hWnd);
}