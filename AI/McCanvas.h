#pragma once

#include <QFrame>
#include <qstack.h>
#include <qscopedpointer.h>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QMouseEvent;
class QResizeEvent;
class QKeyEvent;
QT_END_NAMESPACE

class McShapeInterface;

struct McCanvasData;

class McCanvas : public QFrame{
	Q_OBJECT

public:
	explicit McCanvas(QWidget *parent = 0);
	~McCanvas();

	void init();
	void reset();
	void undo();
	void redo();

	void setAntialiasing(bool isAntialiasing);

	void setStraight(bool isStraight);
	void setShapeType(int shapeType);
	void setLineWidth(int lineWidth);
	void setAlpha(int alpha);
	void setBgColor(const QColor &color);
	void setLineColor(const QColor &color);
	void setBrushColor(const QColor &color);

signals:
	void hasImage(const QImage &image);

protected:
	void paintEvent(QPaintEvent *) override;
	void mouseMoveEvent(QMouseEvent *) override;
	void mousePressEvent(QMouseEvent *) override;
	void mouseReleaseEvent(QMouseEvent *) override;
	void resizeEvent(QResizeEvent * event) override;
	void keyPressEvent(QKeyEvent *) override;	//键盘事件
	void keyReleaseEvent(QKeyEvent *) override;	//键盘事件

private:
	void shapeStackClear(QStack<McShapeInterface *> &stack);
	void drawPixmap(QPixmap &pix, const QColor &color, bool isDrawAll = false);
	void paint(QPainter& painter);

private:
	QScopedPointer<McCanvasData> d;
};
