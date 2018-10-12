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
	// ����ͼƬ������ʱ����
	bool hasInputImage(const QList<QImage> &images) override;
	// ��ͼƬ������ʱ����
	bool beingSaved() override;
	// ��ִ��ȫ�������ұ������ʱ����
	bool saveCompleted() override;
	// ��ִ�е�һ�����ұ������ʱ����
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
