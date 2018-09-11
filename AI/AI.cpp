#include "AI.h"
#include <qapplication.h>
#include <qdesktopwidget.h>
#include <Windows.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qevent.h>
#include <qsystemtrayicon.h>
#include "Input.h"
#include "OutputTransform.h"
#include "ScreenSaverCtrl.h"
#include "TrayMenu.h"
#include "ScreenLock.h"
#include "MyQLabel.h"
#include "Output.h"
#include "MyQMenu.h"
#include "WinAPI.h"
#include "HookDll.h"
#include "OutputBox.h"

struct AIPrivateData : public Ui::AIClass {
	QLabel * label;
	QMenu *menu;
	QSystemTrayIcon *system_tray;
	TrayMenu *tray_menu;
	QScopedPointer<QTimer> screenSaverTimer;
	QScopedPointer<ScreenLock> screenLock;
	QScopedPointer<Input> input;
	OutputTransform *outputTransform;
	ScreenSaverCtrl screenSaverCtrl;
	QString passwd;
	bool isPressCtrl;
	bool isScreenLock;
};

AI::AI(QWidget *parent)
	: QWidget(parent)
	, d(new AIPrivateData) {
	
	d->isScreenLock = false;
	d->passwd = "839566521";
	d->label = new MyQLabel(this);
	d->menu = new MyQMenu(this);
	d->input.reset(nullptr);
	d->outputTransform = new OutputTransform(this);

	d->setupUi(this);
	this->setWindowTitle("AI");
	QGuiApplication::setQuitOnLastWindowClosed(false);  // 设置子窗口关闭时程序不退出
	setAttribute(Qt::WA_TranslucentBackground, true); //背景透明
	setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::SubWindow); //去除边框 //SubWindow

	init();

	connect(d->menu, SIGNAL(openInputEdit()), SLOT(openInputEdit()));
	connect(qobject_cast<MyQLabel *>(d->label), &MyQLabel::showMenu, [this]() {d->menu->exec(QCursor::pos()); });

	show();
	resetPosition();
}

void AI::init() {
	vector<int> vk_key;
	vk_key.push_back(VK_CONTROL);
	vk_key.push_back(VK_F12);
	SetFunction(vk_key, std::bind(&AI::getFocus, this));
	this->initTray();
	this->setScreenSaverTimer();
	this->resize(d->label->size());
}

void AI::setScreenSaverTimer() {
	d->screenSaverTimer.reset(new QTimer(this));
	d->screenSaverTimer->setInterval(5000);
	d->isScreenLock = false;
	SetHook(d->isScreenLock);
	connect(d->screenSaverTimer.data(), &QTimer::timeout, [this]() {
		if (!d->screenSaverCtrl.GetScreenSaverRunning()) {
			d->screenSaverTimer->start();
			return;
		}
		UnsetHook();
		d->screenSaverTimer.reset();
		lockScreen();
	});
	d->screenSaverTimer->start();
}

void AI::lockScreen() {
	d->screenLock.reset(new ScreenLock());
	d->isScreenLock = true;
	SetHook(d->isScreenLock);
	connect(this, SIGNAL(unlockScreen(QString)), d->screenLock.data(), SLOT(unlockScreen(QString)));
	connect(d->screenLock.data(), &ScreenLock::loginSuccessful, [this]() {
		UnsetHook();
		QTimer *deleteScreenLockTimer = new QTimer(this);
		connect(deleteScreenLockTimer, &QTimer::timeout, [this, deleteScreenLockTimer]() {
			d->screenLock->close();
			d->screenLock.reset();
			closeTimer(deleteScreenLockTimer);
		});
		deleteScreenLockTimer->start(100);
		setScreenSaverTimer();
		emit screenUnlocked();
	});
	QTimer *screenLockTimer = new QTimer(this);
	connect(screenLockTimer, &QTimer::timeout, [this, screenLockTimer]() {
		if (d->screenSaverCtrl.GetScreenSaverRunning())
			return;
		closeTimer(screenLockTimer);
		setTopWindow(d->screenLock->windowTitle().toStdString().c_str());
		//getFocus();
	});
	screenLockTimer->start(10);
	emit screenLocked();
}

void AI::openInputEdit() {
	if (d->input == nullptr) {
		d->input.reset(new Input());
		d->input->move(this->pos() - QPoint(d->input->width(), d->input->height() - this->height()) + QPoint(120, -100));
		connect(d->input.data(), &Input::quit, [this]() {
			d->input.reset();
		});
		connect(d->input.data(), &Input::hideOutputBox, d->outputTransform, &OutputTransform::hide);
		connect(d->input.data(), &Input::showOutputBox, d->outputTransform, &OutputTransform::show);
		connect(d->input.data(), SIGNAL(showHistory()), d->outputTransform, SLOT(showHistory()));
		connect(d->input.data(), SIGNAL(changeCharacterState(QString)), d->label, SLOT(changeCharacterState(QString)));
		connect(d->input.data(), SIGNAL(lockScreen()), this, SLOT(lockScreen()));
		connect(d->input.data(), SIGNAL(unlockScreen(QString)), this, SIGNAL(unlockScreen(QString)));
		connect(d->input.data(), &Input::exitApplication, [this](const QString &password) {
			if (password == d->passwd)
				quit();
		});
		connect(d->input.data(), &Input::setHook, [this]() {
			SetHook(d->isScreenLock);
		});
		connect(d->input.data(), &Input::unsetHook, [this](const QString &passwd) {
			if (passwd == d->passwd)
				UnsetHook();
		});
		connect(d->input.data(), &Input::entryCmdMode, d->outputTransform, &OutputTransform::changedToFixedOutptuBox);
		connect(d->input.data(), &Input::entryNormalMode, d->outputTransform, &OutputTransform::changedToNormalOutputBox);
	}
	else 
		d->input->displayInput();
}

void AI::initTray() {
	d->system_tray = new QSystemTrayIcon();
	//放在托盘提示信息、托盘图标
	d->system_tray->setToolTip(QString("I am tray"));
	d->system_tray->setIcon(QIcon("images/1.ico"));
	d->tray_menu = new TrayMenu();
	d->system_tray->setContextMenu(d->tray_menu);
	//点击托盘执行的事件
	connect(d->tray_menu, &TrayMenu::reset, this, &AI::resetPosition);
	connect(d->tray_menu, &TrayMenu::hideCharacter, this, &AI::hide);
	connect(d->tray_menu, &TrayMenu::showCharacter, this, &AI::show);
	connect(d->tray_menu, &TrayMenu::quit, this, &AI::quit);
	//显示托盘
	d->system_tray->show();
	//托盘显示提示信息 
	//d->system_tray->showMessage(QString("AI zero"), QString("start successfully"));
}

void AI::getFocus() {
	/*HWND hwnd = FindWindowA(NULL, "AI");
	SetForegroundWindow(hwnd);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);*/
	if (!isVisible())
		d->tray_menu->changeDisplay();
	hide();
	show();
	setTopWindow(this->windowTitle().toStdString().c_str());
	QPoint point = this->pos();
	QRect rect = this->rect();
	mousePress(point.x() + (rect.width() / 2), point.y() + (rect.height() / 2));
}

void AI::mousePress(int x, int y) {
	SetCursorPos(x, y);
	Sleep(50);
	mouse_event(0x02, 0, 0, 0, GetMessageExtraInfo());
	Sleep(50);
	mouse_event(0x04, 0, 0, 0, GetMessageExtraInfo());
}

void AI::resetPosition() {
	QRect screenRect = QApplication::desktop()->screenGeometry();
	this->move((screenRect.width() - 350), (screenRect.height() - 450));
	if (!isVisible()) {
		show();
		d->tray_menu->changeDisplay();
	}
}

void AI::moveEvent(QMoveEvent * event) {
	d->outputTransform->outputMove(this->pos(), this->rect());
	event->accept();
}

void AI::resizeEvent(QResizeEvent *event) {
	d->outputTransform->outputMove(this->pos(), this->rect());
	event->accept();
}

void AI::keyPressEvent(QKeyEvent *event) {
	switch (event->key())
	{
	case Qt::Key_Control:
		d->isPressCtrl = true;
		break;
	case Qt::Key_E:
		if (d->isPressCtrl)
			openInputEdit();
		break;
	case Qt::Key_L:
		if (d->isPressCtrl) {
			lockScreen();
		}
		break;
	case Qt::Key_U:
		if (d->isPressCtrl) {
			emit unlockScreen("");
		}
		break;
	case Qt::Key_H:
		if (d->isPressCtrl) {
			hide();
			d->tray_menu->changeDisplay();
		}
		break;
	default:
		break;
	}
	QWidget::keyPressEvent(event);
}

void AI::keyReleaseEvent(QKeyEvent * event) {
	if (event->key() == Qt::Key_Control) {
		d->isPressCtrl = false;
		return;
	}
	QWidget::keyPressEvent(event);
}

void AI::closeTimer(QTimer *timer) {
	if (timer != nullptr) {
		timer->stop();
		delete timer;
		timer = nullptr;
	}
}

inline void AI::quit() {
	QApplication::exit(0);
}

AI::~AI() {
	UnsetHook();
	delete d->label;
	delete d->system_tray;
	delete d->tray_menu;
	delete Output::instance();
	deleteLater();
}
