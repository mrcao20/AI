#pragma once
#pragma execution_character_set("utf-8")

#include "Widget.h"

QT_BEGIN_NAMESPACE
class QResizeEvent;
QT_END_NAMESPACE

struct BgTransparentData;

class BgTransparent : public Widget {
	Q_OBJECT

public:
	explicit BgTransparent(QWidget *parent = 0);
	~BgTransparent();

	void reset() override;

protected:
	// 当有图片被载入时调用
	bool hasInputImage(const QList<QImage> &images) override;
	// 当图片被保存时调用
	bool beingSaved() override;
	// 当执行全部保存且保存完毕时调用
	bool saveCompleted() override;
	// 当执行单一保存且保存完毕时调用
	bool saveCompleted(int index) override;
	void resizeEvent(QResizeEvent *event) override;
	bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
	void imageTransparent();

private:
	void loadImage(int index);
	void loadImage(const QImage &image);
	QImage scaledImage(const QImage &image);
	bool isSmallSize(const QImage &image);
	void updateState(const QString &state);
	QImage createAlpha(QImage& img, int threshold, int variations);
	QImage edgeSmooth(QImage &img, QImage &alpha, int edgeThreshold, int blurThreshold);
	QImage addAlpha(QImage &img, QImage &alpha);
	void showNext();
	void autoProcess();

private:
	QScopedPointer<BgTransparentData> d;
};
