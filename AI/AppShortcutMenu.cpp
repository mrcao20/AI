#include <AppShortcutMenu.h>
#include <qdir.h>
#include <qdebug.h>
#include <qprocess.h>
#include "Output.h"

AppShortcutMenu::AppShortcutMenu(QMenu *parent)
	: QMenu(parent)
	, shortcutDir("./AppShortcut")
	, appList(nullptr){

	this->setTitle("App");

	QAction *refresh_action = this->addAction("refresh");
	QAction *more_action = this->addAction("more...");
	this->addSeparator();
	load();
	
	connect(refresh_action, &QAction::triggered, this, &AppShortcutMenu::reload);
	connect(more_action, &QAction::triggered, [this]() {
		if (appList)
			return;
		appList.reset(new AppList());
		connect(appList.data(), &AppList::quit, [this]() {
			appList.reset();
		});
	});
}

void AppShortcutMenu::load() {
	QDir dir(shortcutDir);
	QFileInfoList fileList = dir.entryInfoList(QDir::Files, QDir::LocaleAware);
	for (QFileInfo file : fileList) {
		fileNames.append(file.baseName());
		addAction(loadApp(file));
	}
}

void AppShortcutMenu::reload() {
	QDir dir(shortcutDir);
	QFileInfoList fileList = dir.entryInfoList(QDir::Files, QDir::LocaleAware);
	for (int i = 0; i < fileList.size(); i++) {
		QFileInfo file = fileList[i];
		QString baseName = file.baseName();
		if (fileNames.contains(baseName)) continue;
		fileNames.insert(i, baseName);
		if (i >= fileNames.size() - 1) {
			addAction(loadApp(file));
			continue;
		}
		insertAction(appAction_map[fileNames.at(i + 1)], loadApp(file));
	}
	if (fileNames.size() > fileList.size()) {
		for (int i = 0; i < fileNames.size(); i++) {
			QString fileName = fileNames.at(i);
			if ((i >= fileList.size()) || (fileName != fileList.at(i).baseName())) {
				QAction *action = appAction_map[fileName];
				removeAction(action);
				delete action;
				appAction_map.remove(fileName);
				fileNames.removeAt(i);
				i--;
			}
		}
	}
}

QAction *AppShortcutMenu::loadApp(QFileInfo & file) {
	QString baseName = file.baseName();
	QAction *action = new QAction(baseName, this);
	appAction_map[baseName] = action;
	connect(action, &QAction::triggered, [this, file, baseName]() {
		QString filePath;
		if (file.isSymLink()) filePath = file.symLinkTarget();
		else filePath = file.absoluteFilePath();
		if (QProcess::startDetached(filePath, QStringList())) output << baseName + QString(": 开启成功");
		else output << baseName + QString(": 开启失败");
	});
	return action;
}