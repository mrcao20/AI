#pragma once
#pragma execution_character_set("utf-8")
#include "ui_AppList.h"
#include <qmap.h>
#include <qfileinfo.h>
#include <qmenu.h>
#include <qscopedpointer.h>

struct AppListData;

class AppList : public QWidget{
	Q_OBJECT

public:
	AppList(QWidget *parent = nullptr);
	~AppList();

	void allowCreateShortcut(const QString &pos);

protected:
	void resizeEvent(QResizeEvent *event);
	void closeEvent(QCloseEvent *event);
	//���û��϶��ļ������ڲ�����ʱ�򣬾ͻᴥ��dragEnterEvent�¼�
	void dragEnterEvent(QDragEnterEvent *event) override;
	//���û���������ļ��󣬾ͻᴥ��dropEvent�¼�
	void dropEvent(QDropEvent *event) override;

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
	void createShortcut(const QString &filePath);

private:
	Ui::AppListWidget ui;
	QScopedPointer<AppListData> d;

	QMap<QString, QTableWidgetItem *> items;
	QMenu *menu;
};