#include "Widget.h"
#include <qevent.h>
#include <qmimedata.h>
#include <qfileinfo.h>
#include <qfiledialog.h>
#include <qdir.h>
#include <QtConcurrent>

struct WidgetData {
	QStringList m_imageFormats;
	bool m_isCtrlPress;
	bool m_isAltPress;
};

Widget::Widget(QWidget *parent)
	: QWidget(parent)
	, d(new WidgetData)
{
	setAcceptDrops(true);
	d->m_isCtrlPress = false;
	d->m_isAltPress = false;
	setFocus();
}

void Widget::init(const QStringList&imageFormats) {
	d->m_imageFormats = imageFormats;
}

void Widget::loadImage(QList<QUrl> urls) {
	if (urls.isEmpty())
		return;

	QList<QImage> images;
	for (const QUrl &url : urls) {
		loadImage(url.toLocalFile(), images);		// 这里QUrl的fileName只包含文件的名字，不包含路径，故需使用toLocalFile
	}
	hasInputImage(images);
}

void Widget::dragEnterEvent(QDragEnterEvent *event) {
	//如果为文件，则支持拖放
	if (event->mimeData()->hasFormat("text/uri-list")) {
		event->acceptProposedAction();
	}
}

void Widget::dragLeaveEvent(QDragLeaveEvent *event) {
}

void Widget::dragMoveEvent(QDragMoveEvent *event) {
}

void Widget::dropEvent(QDropEvent *event) {
	//注意：这里如果有多文件存在，意思是用户一下子拖动了多个文件，而不是拖动一个目录
	//如果想读取整个目录，则在不同的操作平台下，自己编写函数实现读取整个目录文件名
	QList<QUrl> urls = event->mimeData()->urls();
	loadImage(urls);
}

void Widget::keyPressEvent(QKeyEvent * event) {
	switch (event->key()){
	case Qt::Key_Control:
		d->m_isCtrlPress = true;
		break;
	case Qt::Key_Alt:
		d->m_isAltPress = true;
		break;
	default:
		break;
	}
	QWidget::keyPressEvent(event);
}

void Widget::keyReleaseEvent(QKeyEvent * event) {
	switch (event->key()) {
	case Qt::Key_Control:
		d->m_isCtrlPress = false;
		break;
	case Qt::Key_Alt:
		d->m_isAltPress = false;
		break;
	case Qt::Key_S:
		if (d->m_isCtrlPress && d->m_isAltPress) {
			QString filePath = getFilePath("Save All Image");
			QtConcurrent::run(this, &Widget::saveAll, filePath);
		}
		else if (d->m_isCtrlPress) {
			save(m_currentIndex);
		}
		d->m_isCtrlPress = false;
		d->m_isAltPress = false;
		break;
	default:
		break;
	}
	QWidget::keyReleaseEvent(event);
}

void Widget::loadImage(const QString &fileName, QList<QImage> &images) {
	QFileInfo fileInfo(fileName);
	if (fileInfo.isDir()) {
		QDir dir(fileName);
		QStringList fileList = dir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::LocaleAware);
		for (const QString &file : fileList) {
			loadImage(dir.absoluteFilePath(file), images);
		}
	}
	QString suffix = fileInfo.suffix();
	if (!d->m_imageFormats.contains(suffix))
		return;
	images.append(QImage(fileName, suffix.toStdString().c_str()));
}

QString Widget::getFilePath(const QString &caption) {
	QString appPath;
#ifdef _DEBUG
	appPath = qApp->applicationDirPath() + "/../Release";
#else
	appPath = qApp->applicationDirPath();
#endif // _DEBUG
	QString path = appPath + "/images/Mode/0.png";
	QString filter;
	for (const QString &format : d->m_imageFormats) {
		filter += QString("Images (*.%1);;").arg(format);
	}
	QString filePath = QFileDialog::getSaveFileName(this, caption, path, filter);
	return filePath;
}

void Widget::saveAll(const QString &filePath) {
	if (filePath.isEmpty())
		return;
	if (!beingSaved())
		return;
	QFileInfo fileInfo(filePath);
	QString dirPath = fileInfo.absolutePath();
	QString baseName = fileInfo.baseName();
	QString suffix = fileInfo.suffix();
	int baseIndex = 0;
	for (int i = 0; i < m_images.size(); i++) {
		QString file;
		bool ok = false;
		int name_int = baseName.toInt(&ok);
		if (ok) {
			while (QFile::exists(file = QString("%1/%2.%3").arg(dirPath, QString::number(baseIndex + name_int + i), suffix))) {
				baseIndex++;
			}
		}
		else {
			while (QFile::exists(file = QString("%1/%2%3.%4").arg(dirPath, baseName, QString::number(baseIndex + i), suffix))) {
				baseIndex++;
			}
		}
		QImage image = m_images.at(i);
		save(image, file);
	}
	saveCompleted();
}

void Widget::save(int index) {
	if (index >= m_images.size() || index < 0)
		return;
	QString filePath = getFilePath("Save Image");
	if (filePath.isEmpty())
		return;
	if (!beingSaved())
		return;
	QImage image = m_images.at(index);
	save(image, filePath);
	saveCompleted(index);
}

void Widget::save(QImage &img, const QString &filePath) {
	QString suffix = QFileInfo(filePath).suffix();
	img.save(filePath, suffix.toStdString().c_str());
}

Widget::~Widget(){
}
