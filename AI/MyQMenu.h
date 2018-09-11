#pragma once
#pragma execution_character_set("utf-8")

#include <qmenu.h>
#include <qaction.h>

class MyQMenu : public QMenu {
	Q_OBJECT

public:
	MyQMenu(QWidget * = 0);

signals:
	void openInputEdit();

private slots:

private:
	QAction *input;

	//QMenu *browserMenu;
	QMenu *appShortcutMenu;
	QMenu *outputMenu;

private:

};