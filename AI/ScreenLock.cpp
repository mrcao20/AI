#include "ScreenLock.h"
#include <qlabel.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qevent.h>
#include <qpainter.h>
#include <qdatetime.h>
#include <Windows.h>
#include "AI.h"

#ifdef _DEBUG
#else
void faceDetection();
#endif // _DEBUG

ScreenLock::ScreenLock(QWidget *parent)
	: QWidget(parent)
	, dateTimeTimer(new QTimer(this))
	, isLogin(false){

	ui.setupUi(this);
	this->setWindowTitle("ScreenLock");
	setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::SubWindow);
	QRect screenRect = QApplication::desktop()->screenGeometry();
	this->resize(screenRect.size());
	ui.ScreenLockPictureLabel->resize(screenRect.size());
	image = QImage(":/AI/images/ScreenLock/128.jpg");
	image = image.scaled(screenRect.size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
	pictureDarkened(-70);
	ui.ScreenLockPictureLabel->installEventFilter(this);
	
	hideLogin();
	hideError();
	
	setDateTime();
	connect(dateTimeTimer, &QTimer::timeout, this, &ScreenLock::setDateTime);
	dateTimeTimer->start(60000);

	connect(ui.loginButton, &QCommandLinkButton::clicked, this, &ScreenLock::login);
	connect(ui.passwordEdit, &QLineEdit::returnPressed, this, &ScreenLock::login);
	connect(ui.verifyButton, &QPushButton::clicked, [this]() {
		showLogin();
		hideError();
	});

	this->showFullScreen();
} 

void ScreenLock::setDateTime() {
	QDateTime local(QDateTime::currentDateTime());
	ui.TimeLabel->setText(local.toString("hh:mm"));
	ui.DateLabel->setText(local.toString("MÔÂdÈÕ, dddd"));
}

void ScreenLock::keyPressEvent(QKeyEvent *event) {
	userLogin();
}

bool ScreenLock::eventFilter(QObject *obj, QEvent *event) {
	if (obj == ui.ScreenLockPictureLabel){
		if (event->type() == QEvent::MouseButtonPress){
			QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
			if (mouseEvent->button() == Qt::LeftButton){
				userLogin();
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	if (event->type() == QEvent::KeyPress) {
		userLogin();
		return true;
	}
	return QWidget::eventFilter(obj, event);
}

void ScreenLock::login() {
	QString password = ui.passwordEdit->text();
	if (password.isEmpty()) {
		#ifdef _DEBUG
		#else
		//faceDetection();
		//unlockScreen("839566521");
		#endif // _DEBUG
	}
	else if (password != "839566521") {
		hideLogin();
		showError();
		return;
	}
	unlockScreen(password);
}

void ScreenLock::unlockScreen(const QString &password) {
	if (password.isEmpty()) {
		userLogin();
		return;
	}
	else if (password == "839566521") {
		hide();
		emit loginSuccessful();
	}
}

void ScreenLock::mousePress(int x, int y) {
	SetCursorPos(x, y);
	Sleep(50);
	mouse_event(0x02, 0, 0, 0, GetMessageExtraInfo());
	Sleep(50);
	mouse_event(0x04, 0, 0, 0, GetMessageExtraInfo());
}

void ScreenLock::showError() {
	ui.errorLabel->setVisible(true);
	ui.verifyButton->setVisible(true);
	ui.verifyButton->setFocus();
}

void ScreenLock::hideError() {
	ui.errorLabel->setVisible(false);
	ui.verifyButton->setVisible(false);
}

void ScreenLock::hideLogin() {
	ui.loginButton->setVisible(false);
	ui.passwordEdit->setVisible(false);
}

void ScreenLock::showLogin() {
	ui.loginButton->setVisible(true);
	ui.passwordEdit->setVisible(true);
	ui.passwordEdit->clear();
	ui.passwordEdit->setFocus();

	/*QPoint pos(ui.passwordEdit->pos().x() + 10, ui.passwordEdit->pos().y() + 10);
	QMouseEvent event0(QEvent::MouseButtonPress, pos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	QApplication::sendEvent(ui.ScreenLockPictureLabel, &event0);
	QMouseEvent event1(QEvent::MouseButtonRelease, pos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	QApplication::sendEvent(ui.ScreenLockPictureLabel, &event1);*/
	mousePress(ui.passwordEdit->pos().x() + 10, ui.passwordEdit->pos().y() + 10);

	//ui.ScreenLockPictureLabel->setFocus();
}

void ScreenLock::userLogin() {
	if (isLogin) return;
	isLogin = true;

	pictureDarkened(-150);

	QTimer *slideTimer = new QTimer(this);
	connect(slideTimer, &QTimer::timeout, [this, slideTimer]() {
		if (ui.DateLabel->pos().y() + ui.DateLabel->size().height() <= -1) {
			showLogin();
			slideTimer->stop();
			delete slideTimer;
			return;
		}
		ui.TimeLabel->move(ui.TimeLabel->pos() - QPoint(0, 25));
		ui.DateLabel->move(ui.DateLabel->pos() - QPoint(0, 25));
	});
	slideTimer->start(5);
}

void ScreenLock::pictureDarkened(int delta) {
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);

	QColor oldColor;
	int r, g, b;
	 
	for (int x = 0; x<newImage.width(); x++) {
		for (int y = 0; y<newImage.height(); y++) {
			oldColor = QColor(image.pixel(x, y));

			r = oldColor.red() + delta;
			g = oldColor.green() + delta;
			b = oldColor.blue() + delta;

			//we check if the new values are between 0 and 255  
			r = qBound(0, r, 255);
			g = qBound(0, g, 255);
			b = qBound(0, b, 255);

			newImage.setPixel(x, y, qRgb(r, g, b));
		}
	}
	QPixmap p = QPixmap::fromImage(newImage);
	ui.ScreenLockPictureLabel->setPixmap(p);
}