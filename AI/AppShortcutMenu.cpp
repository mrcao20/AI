#include <AppShortcutMenu.h>
#include <qdir.h>
#include <qdebug.h>
#include <qprocess.h>
#include <qregularexpression.h>
#include "Output.h"

AppShortcutMenu::AppShortcutMenu(QMenu *parent)
	: QMenu(parent)
	, m_appList(nullptr){

	this->setTitle("App");

	shortcutDir = qApp->applicationDirPath();		// ����ļ��н�β������/��\\�����ֶ�����
#ifdef _DEBUG
	shortcutDir += "/../Release/AppShortcut";
#else
	shortcutDir += "/AppShortcut";
#endif // _DEBUG

	QAction *refresh_action = this->addAction("refresh");
	QAction *more_action = this->addAction("more...");
	this->addSeparator();
	load();
	
	connect(refresh_action, &QAction::triggered, this, &AppShortcutMenu::reload);
	connect(more_action, &QAction::triggered, this, &AppShortcutMenu::createAppList);
}

void AppShortcutMenu::createAppList() {
	if (m_appList)
		return;
	m_appList.reset(new AppList());
	connect(m_appList.data(), &AppList::quit, [this]() {
		m_appList.reset();
	});
}

// ����ָ����λ������ָ����ݷ�ʽ�Ĵ���λ�ã�����AppList�Ƿ��Ѿ�������һ������ָ�������ı��ݷ�ʽ���λ��
void AppShortcutMenu::createShortcut(const QString &pos) {
	createAppList();
	m_appList->allowCreateShortcut(pos);
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
		if (QProcess::startDetached(filePath, QStringList())) output << baseName + QString(": �����ɹ�");
		else output << baseName + QString(": ����ʧ��");
	});
	return action;
}