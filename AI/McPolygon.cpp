#include "McPolygon.h"
#include <qstack.h>
#include "McGlobal.h"

struct McPolygonData {
	QStack<QPoint> m_polygonPoints;
	QStack<QStack<QPoint>> m_undoPoints;	// 用于撤销拖动的操作，用于存放拖动之前的点
	QStack<QStack<QPoint>> m_redoPoints;	// 用于恢复撤销的操作，用于存放撤销之前的点
	QVector<int> m_relatedIndexs;	// 和当前鼠标坐标靠近的点的数组下标
};

McPolygon::McPolygon(QObject *parent)
	: McAbstractShape(parent)
	, d(new McPolygonData)
{

}

McPolygon::~McPolygon(){
}

void McPolygon::paint(QPainter &paint) {
	paint.setPen(QPen(m_lineColor, m_lineWidth));
	paint.setBrush(m_brushColor);
	QPolygon polygon(d->m_polygonPoints.size());
	for (int i = 0; i < d->m_polygonPoints.size(); ++i){
		polygon.setPoint(i, d->m_polygonPoints.at(i));
	}
	paint.drawPolygon(polygon);
}

int McPolygon::shapeType() {
	return Mc::Polygon;
}

bool McPolygon::undo() {
	if (d->m_undoPoints.isEmpty()) {
		if (d->m_polygonPoints.size() <= 2)
			return true;
		else {
			d->m_redoPoints.push(d->m_polygonPoints);
			d->m_polygonPoints.pop();
		}
	}
	else {
		d->m_redoPoints.push(d->m_polygonPoints);
		d->m_polygonPoints = d->m_undoPoints.pop();
	}
	return false;
}

bool McPolygon::redo() {
	if(d->m_redoPoints.isEmpty())
		return true;
	d->m_polygonPoints = d->m_redoPoints.pop();
	d->m_undoPoints.push(d->m_polygonPoints);
	return false;
}

bool McPolygon::isRelated(const QPoint &point, bool isDragShape) {
	d->m_relatedIndexs.swap(QVector<int>());
	for (int i = 0; i < d->m_polygonPoints.size(); i++) {
		QPoint &p = d->m_polygonPoints[i];
		if (abs(point.x() - p.x()) < m_relationOffset && abs(point.y() - p.y()) < m_relationOffset) {
			if (isDragShape) {
				for (int j = 0; j < d->m_polygonPoints.size(); j++)
					d->m_relatedIndexs.append(j);
			}else
				d->m_relatedIndexs.append(i);
			return true;
		}
	}
	for (int i = 0; i < d->m_polygonPoints.size(); i++) {
		int nextI = (i == d->m_polygonPoints.size() - 1) ? 0 : i + 1;
		if (isLineSegRelated(d->m_polygonPoints.at(i), d->m_polygonPoints.at(nextI), point)) {
			if (isDragShape) {
				for (int j = 0; j < d->m_polygonPoints.size(); j++)
					d->m_relatedIndexs.append(j);
			}
			else {
				d->m_relatedIndexs.append(i);
				d->m_relatedIndexs.append(nextI);
			}
			return true;
		}
	}
	return false;
}

void McPolygon::moveStart() {
	d->m_undoPoints.push(d->m_polygonPoints);
}

void McPolygon::move(const QPoint &pointOffset) {
	for (int i : d->m_relatedIndexs) {
		d->m_polygonPoints[i] += pointOffset;
	}
}

void McPolygon::setStartPos(const QPoint &point) {
	if (d->m_polygonPoints.isEmpty())
		d->m_polygonPoints.push(point);
	else
		d->m_polygonPoints.replace(0, point);
}

void McPolygon::setEndPos(const QPoint &point) {
	d->m_polygonPoints.push(point);
}

void McPolygon::setPos(const QPoint &startPoint, const QPoint &endPoint, bool isStraight) {
	d->m_redoPoints.swap(QStack<QStack<QPoint>>());
	if (d->m_polygonPoints.isEmpty())
		d->m_polygonPoints.push(startPoint);
	else
		d->m_polygonPoints.replace(0, startPoint);

	if (d->m_polygonPoints.size() <= 1)
		d->m_polygonPoints.push(endPoint);
	else
		d->m_polygonPoints.top() = endPoint;
}

QPoint McPolygon::startPos() const {
	if (d->m_polygonPoints.isEmpty())
		return m_startPoint;
	else
		return d->m_polygonPoints.first();
}

QPoint McPolygon::endPos() const {
	if (d->m_polygonPoints.isEmpty())
		return m_endPoint;
	else
		return d->m_polygonPoints.top();
}

void McPolygon::getBorder(int &top, int &bottom, int &left, int &right, const QSize &canvasSize) {
	for (const QPoint &p : d->m_polygonPoints)
		getBorderForPoint(top, bottom, left, right, p);
	checkBorder(top, bottom, left, right, canvasSize);
}
