#include "MyQMenu.h"
#include "AppShortcutMenu.h"
#include "OutputMenu.h"

MyQMenu::MyQMenu(QWidget *parent) :
	QMenu(parent),
	input(new QAction("Input", this)){
	//browserMenu = new BrowserMenu(this);
	m_appShortcutMenu = new AppShortcutMenu(this);
	outputMenu = new OutputMenu(this);
	
	this->addAction(input);
	this->addSeparator();
	//this->addMenu(browserMenu);
	this->addMenu(m_appShortcutMenu);
	this->addMenu(outputMenu);
	
	connect(input, &QAction::triggered, [this]() {
		emit openInputEdit();
	});
}

QMenu *MyQMenu::appShortcutMenu() {
	return m_appShortcutMenu;
}