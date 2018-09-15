#include "MyNativeEventFilter.h"
#include <qdebug.h>
#include <qstring.h>
#include "AI.h"

void MyNativeEventFilter::setMainWidget(AI *w) {
	m_w = w;
}

bool MyNativeEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result) {
	if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG") {
		MSG * pMsg = reinterpret_cast<MSG *>(message);
		if (pMsg->message == WM_POWERBROADCAST) {
			if (pMsg->wParam == PBT_APMSUSPEND) {
				if (m_w) {
					m_w->lockScreen();
					return true;
				}
			}
		}
	}
	return false;	// �����Ҫ�Ը���Ϣ����һ�������򷵻�true�����򷵻�false
}