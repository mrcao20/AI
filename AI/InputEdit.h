#pragma once
#pragma execution_character_set("utf-8")
#include <qtextedit.h>
#include <qmainwindow.h>
#include <qevent.h>
#include "InputTextEdit.h"

class InputEdit : public InputTextEdit {
	Q_OBJECT

public:
	InputEdit(QWidget * = 0);

protected:
	void keyPressEvent(QKeyEvent *);
	void keyReleaseEvent(QKeyEvent * event);

signals:
	void hasInput(QString);
	void completeCommand(QString);

private:
	bool isPressCtrl;

};
