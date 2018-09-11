#pragma once
#pragma execution_character_set("utf-8")
#include <qtextedit.h>
#include <qmainwindow.h>
#include <qevent.h>

class InputEdit : public QTextEdit {
	Q_OBJECT

public:
	InputEdit(QWidget * = 0);

protected:
	void mouseDoubleClickEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void keyPressEvent(QKeyEvent *);
	void keyReleaseEvent(QKeyEvent * event);

signals:
	void hasInput(QString);
	void completeCommand(QString);

private:
	QWidget * parent;

	bool pressFlag;
	QPoint position;
	QPoint beginMousePos;
	bool isPressCtrl;

};
