#include<Windows.h>

void setTopWindow(const WCHAR *windowName) {
	HWND hWnd = FindWindow(NULL, windowName);
	SetForegroundWindow(hWnd);
	SwitchToThisWindow(hWnd, TRUE);
	SetFocus(hWnd);
	::PostMessage(hWnd, WM_SETFOCUS, NULL, NULL);
}