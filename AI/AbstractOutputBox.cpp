#include "AbstractOutputBox.h"
#include <qapplication.h>
#include <qdesktopwidget.h>

AbstractOutputBox::AbstractOutputBox(QWidget *parent)
	: QWidget(parent)
	, m_mainWindowRect(0, 0, 0, 0)
	, m_mainWindowPoint(0, 0) {

	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

}

void AbstractOutputBox::OutputBoxMove(const QPoint &point, const QRect &rect) {
	if (rect.width() <= 0 || rect.height() <= 0)
		return;
	m_mainWindowRect = rect;
	m_mainWindowPoint = point;

	QRect screenRect = QApplication::desktop()->screenGeometry();
	int x, y;

	if (m_mainWindowPoint.x() > (screenRect.width() / 2)) {
		x = m_mainWindowPoint.x() - this->width() + (int)ceil((1.0 / 3) * m_mainWindowRect.width());
	}
	else {
		x = m_mainWindowPoint.x() + m_mainWindowRect.width() - (int)ceil((1.0 / 5) * m_mainWindowRect.width());
	}
	y = m_mainWindowPoint.y() - this->height() + (int)ceil((1.0 / 7) * m_mainWindowRect.height());
	y = y > 0 ? y : 0;
	move(x, y);
}

AbstractOutputBox::~AbstractOutputBox(){

}