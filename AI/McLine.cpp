#include "McLine.h"
#include <qstack.h>
#include "McGlobal.h"

struct McLineData {
	QStack<QStack<QPoint>> m_undoPoints;	// 用于撤销拖动的操作，用于存放拖动之前的点
	QStack<QStack<QPoint>> m_redoPoints;	// 用于恢复撤销的操作，用于存放撤销之前的点
	QVector<int> m_relatedIndexs;	// 和当前鼠标坐标靠近的点的数组下标
};

McLine::McLine(QObject *parent)
	: McAbstractShape(parent)
	, d(new McLineData)
{

}

McLine::~McLine(){
}

void McLine::paint(QPainter &paint) {
	paint.setPen(QPen(m_lineColor, m_lineWidth));
	paint.setBrush(m_brushColor);
	paint.drawLine(m_startPoint, m_endPoint);
}

int McLine::shapeType() {
	return Mc::Line;
}

bool McLine::undo() {
	if (d->m_undoPoints.isEmpty()) {
		return true;
	}
	QStack<QPoint> points;
	points.push(m_startPoint);
	points.push(m_endPoint);
	d->m_redoPoints.push(points);
	// 由于入栈时是开始点先入栈，所以出栈时是结束点先出栈
	m_endPoint = d->m_undoPoints.top().pop();
	m_startPoint = d->m_undoPoints.top().pop();
	d->m_undoPoints.pop();
	return false;
}

bool McLine::redo() {
	if (d->m_redoPoints.isEmpty())
		return true;
	QStack<QPoint> points;
	points.push(m_startPoint);
	points.push(m_endPoint);
	d->m_undoPoints.push(points);
	// 由于入栈时是开始点先入栈，所以出栈时是结束点先出栈
	m_endPoint = d->m_redoPoints.top().pop();
	m_startPoint = d->m_redoPoints.top().pop();
	d->m_redoPoints.pop();
	return false;
}

bool McLine::isRelated(const QPoint &point, bool isDragShape) {
	d->m_relatedIndexs.swap(QVector<int>());
	if (isRelated(m_startPoint, point, isDragShape, 0))
		return true;
	if (isRelated(m_endPoint, point, isDragShape, 1))
		return true;
	if (isLineSegRelated(m_startPoint, m_endPoint, point)) {
		d->m_relatedIndexs.append(0);
		d->m_relatedIndexs.append(1);
		return true;
	}
	return false;
}

void McLine::moveStart() {
	QStack<QPoint> points;
	points.push(m_startPoint);
	points.push(m_endPoint);
	d->m_undoPoints.push(points);
}

void McLine::move(const QPoint &pointOffset) {
	for (int i : d->m_relatedIndexs) {
		if (i == 0)
			m_startPoint += pointOffset;
		else if (i == 1)
			m_endPoint += pointOffset;
	}
}

bool McLine::isRelated(const QPoint &point, const QPoint &checkP, bool isDragShape, int i) {
	if (abs(checkP.x() - point.x()) >= m_relationOffset || abs(checkP.y() - point.y()) >= m_relationOffset) {
		return false;
	}
	if (isDragShape) {
		d->m_relatedIndexs.append(0);
		d->m_relatedIndexs.append(1);
	}
	else
		d->m_relatedIndexs.append(i);
	return true;
}
