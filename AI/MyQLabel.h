#pragma once
#pragma execution_character_set("utf-8")

#include <qlabel.h>
#include <qscopedpointer.h>
#include "Global.h"

struct MyQLabelData;

class MyQLabel : public QLabel {
	Q_OBJECT

public:
	MyQLabel(const QString &imageDir, const QString &state, QWidget * = 0);

public:
	enum CharacterState { Stand, Dance };

public slots:
	void reload(const QString &imageDir);

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);

signals:
	void showMenu();

private slots :
	void changePicture();  //超时处理函数  
	void changeCharacterState(const QString &state);
	void changeCharacterState(MyQLabel::CharacterState state);

private:
	void readAllImage();
	void resetIndex();
	void updateImage(int index);

private:
	QScopedPointer<MyQLabelData> d;

};