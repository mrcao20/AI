#include "McEllipse.h"
#include "McGlobal.h"

McEllipse::McEllipse(QObject *parent)
	: McAbstractShape(parent)
{

}

McEllipse::~McEllipse(){
}

void McEllipse::paint(QPainter &paint) {
	paint.setPen(QPen(m_lineColor, m_lineWidth));
	paint.setBrush(m_brushColor);
	if (m_isAntialiasing)	// 默认为不使用抗锯齿，如果需要指定再指定即可
		paint.setRenderHint(QPainter::Antialiasing);
	paint.drawEllipse(m_startPoint, m_endPoint.rx() - m_startPoint.rx(), m_endPoint.ry() - m_startPoint.ry());
} 

int McEllipse::shapeType() {
	return Mc::Ellipse;
}

void McEllipse::getBorder(int &top, int &bottom, int &left, int &right, const QSize &canvasSize) {
	QPoint startPoint(m_startPoint.rx() * 2 - m_endPoint.rx(), m_startPoint.ry() * 2 - m_endPoint.ry());
	getBorderForPoint(top, bottom, left, right, startPoint);
	getBorderForPoint(top, bottom, left, right, m_endPoint);
	checkBorder(top, bottom, left, right, canvasSize);
}
