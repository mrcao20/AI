#pragma once
#pragma execution_character_set("utf-8")
#include <qmenu.h>
#include <qmap.h>
#include <qfileinfo.h>
#include <qscopedpointer.h>
#include "AppList.h"

class AppShortcutMenu : public QMenu{
	Q_OBJECT

public:
	AppShortcutMenu(QMenu *parent = Q_NULLPTR);

public slots:
	void createAppList();
	void createShortcut(const QString &pos);

private slots:
	void reload();

private:
	void load();
	QAction *loadApp(QFileInfo &);

private:
	QString shortcutDir;
	QStringList fileNames;
	QMap<QString, QAction *> appAction_map;
	QScopedPointer<AppList> m_appList;
};