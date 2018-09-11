#include "MyQLabel.h"
#include "AI.h"
#include <sstream>
#include <io.h>

MyQLabel::MyQLabel(QWidget *parent) 
	: QLabel(parent)
	, playTimer(new QTimer(this))
	, pressFlag(false)
	, isLockScreen(false)
	, parent(parent){

	AI *mainWindow = qobject_cast<AI *>(parent);
	connect(mainWindow, &AI::screenLocked, [this]() {
		this->isLockScreen = true;
	});
	connect(mainWindow, &AI::screenUnlocked, [this]() {
		this->isLockScreen = false;
	});

	imageDir = "default";
	readAllImage();
	changeCharacterState("stand");
	connect(playTimer, SIGNAL(timeout()), this, SLOT(changePicture()));
}

void MyQLabel::changePicture() {
	if (index >= images.size())
		resetIndex();
	updateImage(index);
	index++;
}

void MyQLabel::resetIndex() {
	this->index = 0;
}

void MyQLabel::updateImage(int index) {
	if (index < 0) return;
	QImage image = images.at(index);
	this->setPixmap(QPixmap::fromImage(image));
	this->resize(image.size());
}

void MyQLabel::changeCharacterState(const QString &state) {
	if (state == "stand")
		characterState = MyQLabel::Stand;
	else if (state == "dance")
		characterState = MyQLabel::Dance;
	switch (characterState)
	{
	case MyQLabel::Stand:
		playTimer->stop();
		updateImage(images.size() - 1);
		break;
	case MyQLabel::Dance:
		resetIndex();
		playTimer->start(80);
		break;
	default:
		break;
	}
}

int MyQLabel::getImageCount() {
	stringstream ss;
	ss << "images/" + imageDir + "/" << "*.png";
	intptr_t handle;                                                //用于查找的句柄
	struct _finddata_t fileinfo;                          //文件信息的结构体
	handle = _findfirst(ss.str().c_str(), &fileinfo);         //第一次查找
	if (-1 == handle)
		return 0;
	int count = 1;
	while (!_findnext(handle, &fileinfo))               //循环查找其他符合的文件，知道找不到其他的为止
	{
		count++;
	}
	_findclose(handle);
	return count;
}

void MyQLabel::readAllImage() {
	int count = getImageCount();
	if (count <= 0) {
		QMessageBox message(QMessageBox::Warning, "Information", "not found image!");
		message.exec();
		exit(1);
	}
	if (count == images.capacity()) images.clear();
	else if (count > images.capacity()) {
		images.clear();
		images.reserve(count);
	}
	else {
		vector<QImage>().swap(images);
		images.reserve(count);
	}
	for (int i = 0; i < count; i++) {
		stringstream ss;
		ss << "images/" + imageDir + "/" << i << ".png";
		images.push_back(QImage(ss.str().c_str()));
	}
}

void MyQLabel::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		this->pressFlag = true;
		this->position = event->globalPos() - parent->pos();
		this->beginMousePos = event->globalPos();
	}
}

void MyQLabel::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		QPoint pos = event->globalPos() - beginMousePos;
		if (abs(pos.x()) <= 5 && abs(pos.y()) <= 5) {
		}
		this->pressFlag = false;
	}
	else if (event->button() == Qt::RightButton) {
		if (!isLockScreen)
			emit showMenu();
	}
}

void MyQLabel::mouseMoveEvent(QMouseEvent *event) {
	if (pressFlag && (event->buttons() && Qt::LeftButton)) {
		QPoint point = event->globalPos() - position;
		parent->move(point);
	}
}