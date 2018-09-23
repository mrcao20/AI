#pragma once
#pragma execution_character_set("utf-8")
#include <qtextedit.h>
#include <qmainwindow.h>
#include <qevent.h>
#include <qscopedpointer.h>
#include "InputTextEdit.h"

struct InputEditData;

class InputEdit : public InputTextEdit {
	Q_OBJECT

public:
	InputEdit(QWidget * = 0);

protected:
	void keyPressEvent(QKeyEvent *) override;
	void keyReleaseEvent(QKeyEvent * event) override;

signals:
	void hasInput(QString);
	void completeCommand(QString);
	void showPreviousRecord();
	void showNextRecord();

private:
	QScopedPointer<InputEditData> d;

};
