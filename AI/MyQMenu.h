#pragma once
#pragma execution_character_set("utf-8")

#include <qmenu.h>
#include <qaction.h>

class MyQMenu : public QMenu {
	Q_OBJECT

public:
	MyQMenu(QWidget * = 0);

	QMenu *appShortcutMenu();

signals:
	void openInputEdit();

private:
	QAction *input;

	QMenu *m_appShortcutMenu;
	QMenu *outputMenu;

};