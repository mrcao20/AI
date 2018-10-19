#pragma once
#pragma execution_character_set("utf-8")

#include "Widget.h"
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
class QResizeEvent;
QT_END_NAMESPACE

struct McBgTransparentData;

class McBgTransparent : public Widget{
	Q_OBJECT

public:
	enum ProcState {
		Init,
		CreateAlpha,
		EdgeSmooth,
		AddAlpha
	};

	enum MouseState {
		Normal,
		Erase
	};

	explicit McBgTransparent(QWidget *parent = 0);
	virtual ~McBgTransparent();

	void init();
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

private:
	void setCursor(const QPoint &pos);
	void showColor(const QPoint &pos);
	void imageErase(const QPointF &pos);
	bool canManualProcess();
	void loadImage(int index);
	void loadImage(const QImage &image);
	QImage scaledImage(const QImage &image);
	bool isSmallSize(const QImage &image);
	void updateState(const QString &state);
	QImage createAlpha(QImage& img, int threshold, int variations);
	QImage edgeSmooth(QImage &img, QImage &alpha, int edgeThreshold, int blurThreshold);
	QImage addAlpha(QImage &img, QImage &alpha);
	QImage addAlpha(QImage &img, cv::Mat &alpha);
	cv::Mat getAlpha(const QImage &img);
	void nextProc();
	void procing();
	void process(ProcState state);	// 当参数变化时调用该函数
	void showNext();
	void autoProcess();
	bool hasPixmap();

private:
	QScopedPointer<McBgTransparentData> d;
};
