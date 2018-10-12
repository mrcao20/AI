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

	void setStraight(bool isStraight);
	void setShapeType(int shapeType);
	void setLineWidth(int lineWidth);
	void setAlpha(int alpha);
	void setLineColor(const QColor &color);
	void setBrushColor(const QColor &color);

protected:
	void paintEvent(QPaintEvent *) override;
	void mouseMoveEvent(QMouseEvent *) override;
	void mousePressEvent(QMouseEvent *) override;
	void mouseReleaseEvent(QMouseEvent *) override;
	void resizeEvent(QResizeEvent * event) override;
	void keyPressEvent(QKeyEvent *) override;	//�����¼�
	void keyReleaseEvent(QKeyEvent *) override;	//�����¼�

private:
	void shapeStackClear(QStack<McShapeInterface *> &stack);
	void drawPixmap();
	void paint(QPainter& painter);

private:
	QScopedPointer<McCanvasData> d;
};
