#pragma once
#include <qmenu.h>
#include <qaction.h>

class OutputMenu : public QMenu {
	Q_OBJECT

public:
	OutputMenu(QMenu *parent = Q_NULLPTR);

private:
	QAction *speechText;
	QAction *text;
	QAction *speech;
};