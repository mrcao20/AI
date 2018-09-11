#pragma once
#pragma execution_character_set("utf-8")

#include <qlabel.h>
#include <qevent.h>
#include <qmessagebox.h>
#include <iostream>
#include <qdebug.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qtimer.h>

using namespace std;

class MyQLabel : public QLabel {
	Q_OBJECT

public:
	MyQLabel(QWidget * = 0);

public:
	enum CharacterState { Stand, Dance };

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);

signals:
	void showMenu();

private slots :
	void changePicture();  //超时处理函数  
	void changeCharacterState(const QString &state);

private:
	QWidget * parent;
	bool pressFlag;
	bool isLockScreen;
	QPoint position;
	QPoint beginMousePos;
	QTimer *playTimer;

	CharacterState characterState;
	vector<QImage> images;
	int index;
	string imageDir;

private:
	int getImageCount();
	void readAllImage();
	void resetIndex();
	void updateImage(int index);

};