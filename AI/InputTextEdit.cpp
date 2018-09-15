#include "InputTextEdit.h"
#include <qevent.h>

struct MyTextEditDataPrivate{
	bool m_pressFlag;
	QPoint m_position;
	QPoint m_beginMousePos;
	QWidget *m_parent;
};

InputTextEdit::InputTextEdit(QWidget *parent)
	: QPlainTextEdit(parent),
	d(new MyTextEditDataPrivate)
{
	d->m_parent = parent ? parent : this;
	d->m_pressFlag = false;
}

void InputTextEdit::mouseDoubleClickEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		d->m_pressFlag = true;
		d->m_position = event->globalPos() - d->m_parent->pos();
		d->m_beginMousePos = event->globalPos();
	}
	QPlainTextEdit::mouseDoubleClickEvent(event);
}

void InputTextEdit::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton && d->m_pressFlag) {
		QPoint pos = event->globalPos() - d->m_beginMousePos;
		if (abs(pos.x()) <= 5 && abs(pos.y()) <= 5) {
		}
		d->m_pressFlag = false;
	}
	QPlainTextEdit::mouseReleaseEvent(event);
}

void InputTextEdit::mouseMoveEvent(QMouseEvent *event) {
	if (d->m_pressFlag && (event->buttons() == Qt::LeftButton)) {
		d->m_parent->move(event->globalPos() - d->m_position);
	}
	QPlainTextEdit::mouseMoveEvent(event);
}

InputTextEdit::~InputTextEdit()
{
}
