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
	// ����ͼƬ������ʱ����
	bool hasInputImage(const QList<QImage> &images) override;
	// ��ͼƬ������ʱ����
	bool beingSaved() override;
	// ��ִ��ȫ�������ұ������ʱ����
	bool saveCompleted() override;
	// ��ִ�е�һ�����ұ������ʱ����
	bool saveCompleted(int index) override;

private:
	void setWidgetPalette(QWidget *w, QPalette::ColorRole acr, const QColor &acolor);

private:
	QScopedPointer<DrawingToolsData> d;

};

