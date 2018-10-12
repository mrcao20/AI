#pragma once

#include <QPoint>
#include <QPainter>
#include <QColor>

class McShapeInterface{

public:
	virtual ~McShapeInterface() = default;

	virtual void setStartPos(const QPoint &point) = 0;
	virtual void setEndPos(const QPoint &point) = 0;
	virtual void setPos(const QPoint &startPoint, const QPoint &endPoint, bool isStraight) = 0;
	virtual void setShapeProperty(const QColor &lineColor, const QColor &brushColor, const int &lineWidth) = 0;

	virtual QPoint startPos() const = 0;
	virtual QPoint endPos() const = 0;
	virtual QColor lineColor() const = 0;
	virtual QColor brushColor() const = 0;
	virtual int lineWidth() const = 0;

	virtual void paint(QPainter& painter) = 0;
	virtual int shapeType() = 0;

	virtual bool undo() = 0;	// 撤销操作，当需要撤销整个图形时返回true，提示删除该图形，否则返回false，图形内部执行撤销操作
	virtual bool redo() = 0;	// 恢复操作，当该图形已经被彻底恢复时返回true，提示恢复下一个图形，否则返回false，图形内部执行恢复操作

	virtual bool isRelated(const QPoint &point, bool isDragShape) = 0;	// 判断当前点是否在图形上
	virtual void moveStart() = 0;	// 移动开始时调用，主要用于获得撤销操作的点
	virtual void move(const QPoint &pointOffset) = 0;	// 图形移动，参数为移动偏移量

	virtual void getBorder(int &top, int &bottom, int &left, int &right, const QSize &canvasSize) = 0;	// 得到一个边框值，以覆盖所有图形

};
