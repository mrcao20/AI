#pragma once

#include <QWidget>
#include <qlist.h>
#include <qurl.h>
#include <qscopedpointer.h>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDragLeaveEvent;
class QDragMoveEvent;
class QDropEvent;
QT_END_NAMESPACE

struct WidgetData;

class Widget : public QWidget {
	Q_OBJECT

public:
	explicit Widget(QWidget *parent = 0);
	~Widget();

	void init(const QStringList &imageFormats);
	void loadImage(QList<QUrl> urls);
	virtual void reset() = 0;

protected:
	// 当有图片被载入时调用
	virtual bool hasInputImage(const QList<QImage> &images) = 0;
	// 当图片被保存时调用
	virtual bool beingSaved() = 0;
	// 当执行全部保存且保存完毕时调用
	virtual bool saveCompleted() = 0;
	// 当执行单一保存且保存完毕时调用
	virtual bool saveCompleted(int index) = 0;

	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragLeaveEvent(QDragLeaveEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent *event) override;
	void keyPressEvent(QKeyEvent * event) override;
	void keyReleaseEvent(QKeyEvent * event) override;
	
	void loadImage(const QString&fileName, QList<QImage> &images);

protected:
	QList<QImage> m_images;
	int m_currentIndex;

private:
	QString getFilePath(const QString &caption);
	void saveAll(const QString &filePath);
	void save(int index);
	void save(QImage &img, const QString &filePath);

private:
	QScopedPointer<WidgetData> d;
};
