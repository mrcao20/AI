#pragma once
#pragma execution_character_set("utf-8")
#include "ui_AppList.h"
#include <qmap.h>
#include <qfileinfo.h>
#include <qmenu.h>

class AppList : public QWidget{
	Q_OBJECT

public:
	AppList(QWidget *parent = nullptr);
	~AppList();

protected:
	void resizeEvent(QResizeEvent *event);
	void closeEvent(QCloseEvent *event);

signals:
	void quit();

private slots:
	void handleContextMenuRequested(const QPoint &pos);
	void openApp(QTableWidgetItem *item);

private:
	void loadApp(const QString &);
	void reloadApp();
	void addApp();
	void resizeTableWidget();
	QString getTargetFile(const QString &);
	void deleteFile(QTableWidgetItem *item);
	void deleteItem(QTableWidgetItem *item);
	void deleteItems(QMap<QString, QTableWidgetItem *> &);

private:
	Ui::AppListWidget ui;

	QMap<QString, QTableWidgetItem *> items;
	QMenu *menu;
};