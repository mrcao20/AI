#include "ScreenSaver.h"
#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qdir.h>
#include <Output.h>
#include <qdatetime.h>
#include <qtimer.h>
#include <qevent.h>
#include <qdebug.h>

ScreenSaver::ScreenSaver(QWidget *parent)
	: QWidget(parent)
	, mousePoint(QCursor::pos()){

	ui.setupUi(this);

	setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool); 

	QRect screenRect = QApplication::desktop()->screenGeometry();
	this->resize(screenRect.size());
	ui.PictureLabel->resize(screenRect.size());

	loadAllImage("D:/picture");
	setImage();

	QTimer *playTimer = new QTimer(this);
	connect(playTimer, &QTimer::timeout, this, &ScreenSaver::setImage);
	playTimer->start(2000);

	ui.PictureLabel->installEventFilter(this);
	this->installEventFilter(this);
	ui.PictureLabel->setMouseTracking(true);

	this->showFullScreen();
}

bool ScreenSaver::eventFilter(QObject *obj, QEvent *event) {
	bool isInput = false;
	QPoint point = mousePoint - QCursor::pos();
	isInput = ((event->type() == QEvent::MouseMove) && (point.x() >= 5 || point.y() >= 5)) ||
		(event->type() == QEvent::KeyPress) ||
		(event->type() == QEvent::MouseButtonPress);
	if (isInput) {
		emit quitScreenSaver();
		return true;
	}
	mousePoint = QCursor::pos();
	return QWidget::eventFilter(obj, event);
}

void ScreenSaver::loadAllImage(const QString &imagePath) {
	QDir imageDir(imagePath);
	if (!imageDir.exists()) {
		output << QString("image path is not exists");
		emit quitScreenSaver();
		return;
	}

	imageList = imageDir.entryInfoList(QDir::Files);
	
	if (imageList.isEmpty()) {
		output << QString("this dir has not image");
		emit quitScreenSaver();
		return;
	}
}

int ScreenSaver::generateRandomNum() {
	if (imageList.size() <= 0)
		return -1;
	int num = 0;

	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
	num = qrand() % imageList.size();

	return num;
}

void ScreenSaver::setImage() {
	if (imageList.isEmpty())
		return;
	QRect screenRect = QApplication::desktop()->screenGeometry();
	int index = generateRandomNum();
	if (index < 0)
		return;
	QImage image(imageList.at(index).absoluteFilePath());
	ui.PictureLabel->setPixmap(QPixmap::fromImage(image.scaled(screenRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
}