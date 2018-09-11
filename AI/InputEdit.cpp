#include "InputEdit.h"
#include <qdebug.h>

InputEdit::InputEdit(QWidget *parent)
	: QTextEdit(parent)
	, parent(parent)
	, isPressCtrl(false) {

	this->resize(300, 150);
	
	setFocus();
}

void InputEdit::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Control) {
		isPressCtrl = true;
		return;
	}
	else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		if (isPressCtrl)
			this->insertPlainText("\n");
		else {
			QString str = toPlainText();
			this->clear();
			emit hasInput(str);
		}
		return;
	}
	else if (event->key() == Qt::Key_E) {
		if (isPressCtrl) {
			isPressCtrl = false;
			this->clear();
			emit hasInput(":quit");
			return;
		}
	}
	else if (event->key() == Qt::Key_Tab) {
		emit completeCommand(toPlainText());
		return;
	}
	QTextEdit::keyPressEvent(event);
}

void InputEdit::keyReleaseEvent(QKeyEvent * event) {
	if (event->key() == Qt::Key_Control) {
		isPressCtrl = false;
		return;
	}
	QTextEdit::keyPressEvent(event);
}

void InputEdit::mouseDoubleClickEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		this->pressFlag = true;
		this->position = event->globalPos() - parent->pos();
		this->beginMousePos = event->globalPos();
	}
	QTextEdit::mouseDoubleClickEvent(event);
}

void InputEdit::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton && pressFlag) {
		QPoint pos = event->globalPos() - beginMousePos;
		if (abs(pos.x()) <= 5 && abs(pos.y()) <= 5) {
		}
		this->pressFlag = false;
	}
	QTextEdit::mouseReleaseEvent(event);
}

void InputEdit::mouseMoveEvent(QMouseEvent *event) {
	if (pressFlag && (event->buttons() && Qt::LeftButton)) {
		parent->move(event->globalPos() - position);
	}
	QTextEdit::mouseMoveEvent(event);
}