#pragma once
#pragma execution_character_set("utf-8")

#include <QtWidgets/QWidget>
#include "ui_AI.h"
#include <qscopedpointer.h>

using namespace std;

struct AIPrivateData;

class AI : public QWidget {
	Q_OBJECT

public:
	AI(QWidget *parent = Q_NULLPTR);
	~AI();

protected:
	void moveEvent(QMoveEvent * event);
	void keyPressEvent(QKeyEvent *);
	void keyReleaseEvent(QKeyEvent * event);
	void resizeEvent(QResizeEvent *event);

signals:
	void screenLocked();
	void screenUnlocked();
	void unlockScreen(const QString &);

private slots:
	void resetPosition();
	void openInputEdit();
	void lockScreen();
	void quit();

private:
	QScopedPointer<AIPrivateData> d;

	void init();
	void initTray();
	void getFocus();
	void setScreenSaverTimer();
	void closeTimer(QTimer *);
	void mousePress(int x, int y);

};