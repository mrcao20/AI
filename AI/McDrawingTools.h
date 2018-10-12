#pragma once
#pragma execution_character_set("utf-8")
#include "Widget.h"

QT_BEGIN_NAMESPACE
class QResizeEvent;
QT_END_NAMESPACE

struct DrawingToolsData;

class McDrawingTools : public Widget{
	Q_OBJECT

public:
	explicit McDrawingTools(QWidget *parent = 0);

	void init();

	void loadImage(QList<QUrl> urls) override {}
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

private:
	void setWidgetPalette(QWidget *w, QPalette::ColorRole acr, const QColor &acolor);

private:
	QScopedPointer<DrawingToolsData> d;

};

