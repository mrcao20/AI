#pragma once
#pragma execution_character_set("utf-8")

#include <qaction.h>
#include <qmenu.h>
#include <qapplication.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include <qscopedpointer.h>

class TrayMenu : public QMenu {
	Q_OBJECT

public:
	TrayMenu(QWidget * = 0);

	void changeDisplay();

signals:
	void reset();
	void showCharacter();
	void hideCharacter();
	void quit();

private:
	QAction * action_quit;
	QAction *reset_action;
	QScopedPointer<QAction> display_action;

private:
	void createActions();
	void setText();
	void addActions();
	void connectActions();
	void addDisplayAction(QString oldTitle, QString newTitle);
};