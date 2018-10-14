#include "McAbstractShape.h"

McAbstractShape::McAbstractShape(QObject *parent)
	: QObject(parent)
	, m_startPoint(QPoint(0, 0))
	, m_endPoint(QPoint(0, 0))
	, m_relationOffset(10)
	, m_isAntialiasing(true)
{

}

void McAbstractShape::setPos(const QPoint &startPoint, const QPoint &endPoint, bool isStraight) {
	m_startPoint = startPoint;
	m_endPoint = endPoint;
	if (isStraight){
		int dis;
		if (abs(startPoint.x() - endPoint.x()) < abs(startPoint.y() - endPoint.y()))
			dis = abs(startPoint.x() - endPoint.x());
		else
			dis = abs(startPoint.y() - endPoint.y());
		if (startPoint.x() - endPoint.x() > 0)
			m_endPoint.setX(startPoint.x() - dis);
		else
			m_endPoint.setX(startPoint.x() + dis);

		if (startPoint.y() - endPoint.y() > 0)
			m_endPoint.setY(startPoint.y() - dis);
		else
			m_endPoint.setY(startPoint.y() + dis);
	}
}

void McAbstractShape::setShapeProperty(const QColor &lineColor, const QColor &brushColor, const int &lineWidth) {
	m_lineColor = lineColor;
	m_brushColor = brushColor;
	m_lineWidth = lineWidth;
}

bool McAbstractShape::isLineSegRelated(const QPoint &p1, const QPoint &p2, const QPoint &checkP) {
	QPoint vectorA = checkP - p1;	// 起始点和判断点构成的向量
	QPoint vectorB = p2 - p1;	// 起始点和结束点构成的向量
	qreal lengthA = sqrt(pow(vectorA.x(), 2) + pow(vectorA.y(), 2));
	qreal lengthB = sqrt(pow(vectorB.x(), 2) + pow(vectorB.y(), 2));
	qint32 value_AMultiplyB = vectorA.x() * vectorB.x() + vectorA.y() * vectorB.y();
	qreal cosTheta = value_AMultiplyB / (lengthA * lengthB);
	if (cosTheta > 1 || cosTheta <= 0)
		return false;
	qreal sinTheta = sqrt(1 - pow(cosTheta, 2));
	qreal offset = lengthA * sinTheta;
	if (offset < m_relationOffset)
		return true;
	return false;
}

void McAbstractShape::getBorder(int &top, int &bottom, int &left, int &right, const QSize &canvasSize) {
	if (m_startPoint == QPoint(-1, -1) && m_endPoint == QPoint(-1, -1))
		return;
	getBorderForPoint(top, bottom, left, right, m_startPoint);
	getBorderForPoint(top, bottom, left, right, m_endPoint);
	checkBorder(top, bottom, left, right, canvasSize);
}

void McAbstractShape::getBorderForPoint(int &top, int &bottom, int &left, int &right, const QPoint &point) {
	top = (top == -1 || top > point.y()) ? point.y() - 1 : top;
	bottom = (bottom == -1 || bottom < point.y()) ? point.y() + 1 : bottom;
	left = (left == -1 || left > point.x()) ? point.x() - 1 : left;
	right = (right == -1 || right < point.x()) ? point.x() + 1 : right;
}

void McAbstractShape::checkBorder(int &top, int &bottom, int &left, int &right, const QSize &size) {
	top = (top < 0) ? 0 : top;
	bottom = (bottom > size.height()) ? size.height() : bottom;
	left = (left < 0) ? 0 : left;
	right = (right > size.width()) ? size.width() : right;
}
