#pragma once

#include <QAbstractNativeEventFilter>
#include <qbytearray.h>

#ifdef Q_CC_MSVC
#include <windows.h>
#pragma comment(lib, "user32.lib")
#endif

#define WM_WinCreate WM_USER+1000

class AI;

class MyNativeEventFilter : public QAbstractNativeEventFilter{

public:
	void setMainWidget(AI *w);
	bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE;

private:
	AI *m_w;

};
