#pragma once
#pragma execution_character_set("utf-8")

#include <QtWidgets/QWidget>
#include "ui_ScreenLock.h"
#include <qtimer.h>

class ScreenLock : public QWidget
{
	Q_OBJECT

public:
	ScreenLock(QWidget *parent = Q_NULLPTR);

	

protected:
	bool eventFilter(QObject *obj, QEvent *event);

signals:
	void loginSuccessful();

public slots:
	void unlockScreen(const QString &);

private slots:
	void setDateTime();
	void login();

private:
	void userLogin();
	void pictureDarkened(int delta);
	inline void showLogin();
	inline void showError();
	inline void hideLogin();
	inline void hideError();
	void mousePress(int x, int y);

private:
	Ui::ScreenLock ui;

	bool isLogin;
	QImage image;
	QTimer *dateTimeTimer;
};