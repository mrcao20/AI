#pragma once
#pragma execution_character_set("utf-8")

#include <qmenu.h>
#include <qaction.h>
#include <qprocess.h>
#include "DeleteFile.h"
#include <qthread.h>

class BrowserMenu : public QMenu {
	Q_OBJECT

public:
	BrowserMenu(QMenu *parent = Q_NULLPTR);
	~BrowserMenu();

private slots:
	void startBrowserProcess();

private:
	bool isDeleteFile();
	void createDelete(QString filePath, QString filterString);
	void createThread();

private:
	QAction *browser_action;
	QAction *clearCache_action;
	QAction *clearAll_action;
	QProcess *browser_process;
	DeleteFile *deleteFile;
	QThread *thread;

};