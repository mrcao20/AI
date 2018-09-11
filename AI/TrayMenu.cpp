#include "TrayMenu.h"

TrayMenu::TrayMenu(QWidget *parent) : QMenu(parent) {
	createActions();
	addActions();
	connectActions();
}

void TrayMenu::createActions() {
	//����������
	action_quit = new QAction(this);
	reset_action = new QAction(this);
	setText();
}

//�����������ı�
void TrayMenu::setText() {
	action_quit->setText("Quit");
	reset_action->setText("Reset");
}

//��Ӳ˵���
void TrayMenu::addActions() {
	this->addAction(reset_action);
	changeDisplay();
	this->addSeparator(); // ��ӷָ���
	this->addAction(action_quit);
}

//�����ź����ӣ�������о�������ʾ���ڵ��źţ�
void TrayMenu::connectActions() {
	QObject::connect(action_quit, SIGNAL(triggered(bool)), this, SIGNAL(quit()));
	connect(reset_action, SIGNAL(triggered(bool)), this, SIGNAL(reset()));
}

void TrayMenu::changeDisplay() {
	QString oldTitle, newTitle;
	if (!display_action) {
		oldTitle = "Show";
		newTitle = "Hide";
	}
	else {
		oldTitle = display_action->text();
		if (oldTitle == "Show")
			newTitle = "Hide";
		else
			newTitle = "Show";
	}
	addDisplayAction(oldTitle, newTitle);
}

void TrayMenu::addDisplayAction(QString oldTitle, QString newTitle) {
	display_action.reset(new QAction(this));
	display_action->setText(newTitle);
	this->insertAction(reset_action, display_action.data());
	connect(display_action.data(), &QAction::triggered, [this, oldTitle, newTitle]() {
		if (display_action->text() == "Hide")
			emit hideCharacter();
		else if (display_action->text() == "Show")
			emit showCharacter();
		this->removeAction(display_action.data());
		addDisplayAction(newTitle, oldTitle);
	});
}