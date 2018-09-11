#pragma once

#include <QtWidgets/QWidget>
#include "ui_ScreenSaver.h"
#include <vector>
#include <qimage.h>
#include <qfileinfo.h>

class ScreenSaver : public QWidget{
	Q_OBJECT

public:
	ScreenSaver(QWidget *parent = Q_NULLPTR);

signals:
	void quitScreenSaver();

private slots:
	void setImage();

protected:
	bool eventFilter(QObject *obj, QEvent *event);

private:
	void loadAllImage(const QString &);
	int generateRandomNum();

private:
	Ui::ScreenSaver ui;

	QFileInfoList imageList;
	QPoint mousePoint;
};