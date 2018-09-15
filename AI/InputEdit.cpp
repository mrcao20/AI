#include "InputEdit.h"
#include <qdebug.h>

InputEdit::InputEdit(QWidget *parent)
	: InputTextEdit(parent)
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
	QPlainTextEdit::keyPressEvent(event);
}

void InputEdit::keyReleaseEvent(QKeyEvent * event) {
	if (event->key() == Qt::Key_Control) {
		isPressCtrl = false;
		return;
	}
	QPlainTextEdit::keyPressEvent(event);
}
