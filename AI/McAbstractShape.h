#pragma once

#include "McShapeInterface.h"
#include <qobject.h>

class McAbstractShape : public QObject, public McShapeInterface{
	Q_OBJECT

public:
	explicit McAbstractShape(QObject *parent = 0);
	virtual ~McAbstractShape() = default;

	inline void setAntialiasing(bool isAntialiasing) { m_isAntialiasing = isAntialiasing; }

	void setStartPos(const QPoint &point) override { m_startPoint = point; }
	void setEndPos(const QPoint &point) override { m_endPoint = point; }
	void setPos(const QPoint &startPoint, const QPoint &endPoint, bool isStraight) override;
	void setShapeProperty(const QColor &lineColor, const QColor &brushColor, const int &lineWidth) override;

	inline QPoint startPos() const  override { return m_startPoint; }
	inline QPoint endPos() const  override { return m_endPoint; }
	inline QColor lineColor() const  override { return m_lineColor; }
	inline QColor brushColor() const  override { return m_brushColor; }
	inline int lineWidth() const  override { return m_lineWidth; }

	inline bool undo() override { return true; }
	inline bool redo() override { return true; }

	inline bool isRelated(const QPoint &point, bool isDragShape) override { return false; }
	inline void moveStart() override {}
	inline void move(const QPoint &pointOffset) override {}

	void getBorder(int &top, int &bottom, int &left, int &right, const QSize &canvasSize) override;

protected:
	bool isLineSegRelated(const QPoint &p1, const QPoint &p2, const QPoint &checkP);
	void getBorderForPoint(int &top, int &bottom, int &left, int &right, const QPoint &point);
	void checkBorder(int &top, int &bottom, int &left, int &right, const QSize &size);

protected:
	QPoint m_startPoint;
	QPoint m_endPoint;
	QColor m_lineColor;
	QColor m_brushColor;
	int m_lineWidth;
	int m_relationOffset;
	bool m_isAntialiasing;	// 该图形是否使用抗锯齿绘图，在图形创建时指定，图形一旦绘制则无法改变
	
};

