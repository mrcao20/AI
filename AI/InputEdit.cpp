#include "InputEdit.h"

struct InputEditData {
	bool m_isPressCtrl;
};

InputEdit::InputEdit(QWidget *parent)
	: InputTextEdit(parent)
	, d(new InputEditData) {

	d->m_isPressCtrl = false;
	resize(300, 150);
	
	setFocus();
}

void InputEdit::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Control) {
		d->m_isPressCtrl = true;
		return;
	}
	else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		if (d->m_isPressCtrl)
			this->insertPlainText("\n");
		else {
			QString str = toPlainText();
			this->clear();
			emit hasInput(str);
		}
		return;
	}
	else if (event->key() == Qt::Key_E) {
		if (d->m_isPressCtrl) {
			d->m_isPressCtrl = false;
			this->clear();
			emit hasInput(":quit");
			return;
		}
	}
	else if (event->key() == Qt::Key_Tab) {
		emit completeCommand(toPlainText());
		return;
	}
	else if (event->key() == Qt::Key_Up) {
		emit showPreviousRecord();
		return;
	}
	else if (event->key() == Qt::Key_Down) {
		emit showNextRecord();
		return;
	}
	QPlainTextEdit::keyPressEvent(event);
}

void InputEdit::keyReleaseEvent(QKeyEvent * event) {
	if (event->key() == Qt::Key_Control) {
		d->m_isPressCtrl = false;
		return;
	}
	QPlainTextEdit::keyPressEvent(event);
}
