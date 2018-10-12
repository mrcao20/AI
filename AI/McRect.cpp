#include "McRect.h"
#include <qstack.h>
#include "McGlobal.h"

struct McRectData {
	QVector<QPoint> m_points;	// 矩形的顶点坐标，以左上角开始，顺时针取4个点
	QStack<QVector<QPoint>> m_undoPoints;	// 用于撤销拖动的操作，用于存放拖动之前的点
	QStack<QVector<QPoint>> m_redoPoints;	// 用于恢复撤销的操作，用于存放撤销之前的点
	QVector<int> m_relatedIndexs;	// 和当前鼠标坐标靠近的点的数组下标
};

McRect::McRect(QObject *parent)
	: McAbstractShape(parent)
	, d(new McRectData)
{
	d->m_points.reserve(4);
}

McRect::~McRect(){
}

void McRect::setPos(const QPoint &startPoint, const QPoint &endPoint, bool isStraight) {
	McAbstractShape::setPos(startPoint, endPoint, isStraight);
	QPoint secPoint(m_endPoint.x(), m_startPoint.y());
	QPoint fourthPoint(m_startPoint.x(), m_endPoint.y());
	d->m_points.clear();
	d->m_points.append(m_startPoint);
	d->m_points.append(secPoint);
	d->m_points.append(m_endPoint);
	d->m_points.append(fourthPoint);
}

QPoint McRect::startPos() const {
	if (d->m_points.size() != 4)
		return QPoint(0, 0);
	return d->m_points.at(0);
}

QPoint McRect::endPos() const {
	if (d->m_points.size() != 4)
		return QPoint(0, 0);
	return d->m_points.at(2);
}

void McRect::paint(QPainter &paint){
	if (d->m_points.size() != 4)
		return;
	paint.setPen(QPen(m_lineColor, m_lineWidth));
	paint.setBrush(m_brushColor);
	paint.drawRect(d->m_points.at(0).x(), d->m_points.at(0).y(), d->m_points.at(2).x() - d->m_points.at(0).x(), d->m_points.at(2).y() - d->m_points.at(0).y());
}

int McRect::shapeType(){
	return Mc::Rect;
}

bool McRect::undo() {
	if (d->m_undoPoints.isEmpty())
		return true;
	d->m_points = d->m_undoPoints.pop();
	d->m_redoPoints.push(d->m_points);
	return false;
}

bool McRect::redo() {
	if (d->m_redoPoints.isEmpty())
		return true;
	d->m_points = d->m_redoPoints.pop();
	d->m_undoPoints.push(d->m_points);
	return false;
}

bool McRect::isRelated(const QPoint &point, bool isDragShape) {
	d->m_relatedIndexs.swap(QVector<int>());
	for (int i = 0; i < d->m_points.size(); i++) {
		QPoint &p = d->m_points[i];
		if (abs(point.x() - p.x()) < m_relationOffset && abs(point.y() - p.y()) < m_relationOffset) {
			if (isDragShape) {
				d->m_relatedIndexs.append(0);
				d->m_relatedIndexs.append(2);
			}else
				d->m_relatedIndexs.append(i);
			return true;
		}
	}
	for (int i = 0; i < d->m_points.size(); i++) {
		int nextI = (i == d->m_points.size() - 1) ? 0 : i + 1;
		if (isLineSegRelated(d->m_points.at(i), d->m_points.at(nextI), point)) {
			if (isDragShape) {
				d->m_relatedIndexs.append(0);
				d->m_relatedIndexs.append(2);
			}else
				d->m_relatedIndexs.append(i);
			return true;
		}
	}
	return false;
}

void McRect::moveStart() {
	d->m_undoPoints.push(d->m_points);
}

void McRect::move(const QPoint &pointOffset) {
	QPoint &firstPoint = d->m_points[0];
	QPoint &secPoint = d->m_points[1];
	QPoint &thirdPoint = d->m_points[2];
	QPoint &fourthPoint = d->m_points[3];

	int top = firstPoint.y();
	int bottom = thirdPoint.y();
	int left = firstPoint.x();
	int right = thirdPoint.x();

	for (int i : d->m_relatedIndexs) {
		top += (i == 0 || i == 1) ? pointOffset.y() : 0;
		bottom += (i == 2 || i == 3) ? pointOffset.y() : 0;
		left += (i == 0 || i == 3) ? pointOffset.x() : 0;
		right += (i == 1 || i == 2) ? pointOffset.x() : 0;
	}

	firstPoint.setX(left);
	firstPoint.setY(top);
	secPoint.setX(right);
	secPoint.setY(top);
	thirdPoint.setX(right);
	thirdPoint.setY(bottom);
	fourthPoint.setX(left);
	fourthPoint.setY(bottom);
}

void McRect::getBorder(int &top, int &bottom, int &left, int &right, const QSize &canvasSize) {
	if (d->m_points.isEmpty())
		return;
	m_startPoint = d->m_points.at(0);
	m_endPoint = d->m_points.at(2);
	McAbstractShape::getBorder(top, bottom, left, right, canvasSize);
}
