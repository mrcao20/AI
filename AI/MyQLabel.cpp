#include "MyQLabel.h"
#include "AI.h"
#include <qevent.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qtimer.h>
#include <qmessagebox.h>
#include <qvector.h>
#include <qdir.h>
#include "Global.h"

struct MyQLabelData {
	QWidget * m_parent;
	bool m_pressFlag;
	bool m_isLockScreen;
	QPoint m_position;
	QPoint m_beginMousePos;
	QTimer *m_playTimer;

	MyQLabel::CharacterState m_characterState;
	QVector<QImage> m_images;
	int m_index;
	QString m_imageDir;
};

MyQLabel::MyQLabel(const QString &imageDir, const QString &state, QWidget *parent)
	: QLabel(parent)
	, d(new MyQLabelData)
{
	d->m_playTimer = new QTimer(this);
	d->m_pressFlag = false;
	d->m_isLockScreen = false;
	d->m_parent = parent;

	AI *mainWindow = qobject_cast<AI *>(parent);
	connect(mainWindow, &AI::screenLocked, [this]() {
		d->m_isLockScreen = true;
	});
	connect(mainWindow, &AI::screenUnlocked, [this]() {
		d->m_isLockScreen = false;
	});

	d->m_imageDir = imageDir;
	readAllImage();
	changeCharacterState(state);
	connect(d->m_playTimer, SIGNAL(timeout()), this, SLOT(changePicture()));
}

void MyQLabel::reload(const QString &imageDir) {
	d->m_imageDir = imageDir;
	readAllImage();
	changeCharacterState(d->m_characterState);
}

void MyQLabel::changePicture() {
	if (d->m_index >= d->m_images.size())
		resetIndex();
	updateImage(d->m_index);
	d->m_index++;
}

void MyQLabel::resetIndex() {
	this->d->m_index = 0;
}

void MyQLabel::updateImage(int index) {
	if (index < 0) return;
	QImage image = d->m_images.at(index);
	setPixmap(QPixmap::fromImage(image));
	resize(image.size());
}

void MyQLabel::changeCharacterState(const QString &state) {
	if (state == "stand")
		d->m_characterState = MyQLabel::Stand;
	else if (state == "dance")
		d->m_characterState = MyQLabel::Dance;
	changeCharacterState(d->m_characterState);
}

void MyQLabel::changeCharacterState(MyQLabel::CharacterState state) {
	switch (state)
	{
	case MyQLabel::Stand:
		d->m_playTimer->stop();
		updateImage(d->m_images.size() - 1);
		break;
	case MyQLabel::Dance:
		resetIndex();
		d->m_playTimer->start(80);
		break;
	default:
		break;
	}
}

void MyQLabel::readAllImage() {
	QString appPath;
#ifdef _DEBUG
	appPath = qApp->applicationDirPath() + "/../Release";
#else
	appPath = qApp->applicationDirPath();
#endif // _DEBUG
	QDir dir(appPath + "/images/Mode/" + d->m_imageDir);
	QStringList fileList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::LocaleAware);
	sort(fileList, SortAlgorithm::QuickSort);
	int count = fileList.size();
	if (count <= 0) {		// 如果文件夹中没有图片，则直接退出程序
		QMessageBox message(QMessageBox::Warning, "Information", "not found image!");
		message.exec();
		exit(1);
	}
	// 动态判断当前图片容器大小和需要加载图片的数量，动态调整容器大小
	if (count == d->m_images.capacity()) d->m_images.clear();
	else if (count > d->m_images.capacity()) {
		d->m_images.clear();
		d->m_images.reserve(count);
	}
	else {
		QVector<QImage>().swap(d->m_images);
		d->m_images.reserve(count);
	}
	// fileList中存放的仅为图片名字，不包含路径
	for (const QString &fileName : fileList) {
		QString filePath = dir.absoluteFilePath(fileName);
		QImage img(filePath, QFileInfo(filePath).suffix().toStdString().c_str());
		d->m_images.append(img);
	}
}

void MyQLabel::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		d->m_pressFlag = true;
		d->m_position = event->globalPos() - d->m_parent->pos();
		d->m_beginMousePos = event->globalPos();
	}
}

void MyQLabel::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		QPoint pos = event->globalPos() - d->m_beginMousePos;
		if (abs(pos.x()) <= 5 && abs(pos.y()) <= 5) {
		}
		d->m_pressFlag = false;
	}
	else if (event->button() == Qt::RightButton) {
		if (!d->m_isLockScreen)
			emit showMenu();
	}
}

void MyQLabel::mouseMoveEvent(QMouseEvent *event) {
	if (d->m_pressFlag && (event->buttons() && Qt::LeftButton)) {
		QPoint point = event->globalPos() - d->m_position;
		d->m_parent->move(point);
	}
}