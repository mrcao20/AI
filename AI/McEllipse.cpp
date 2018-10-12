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
	paint.drawEllipse(m_startPoint, m_endPoint.rx() - m_startPoint.rx(), m_endPoint.ry() - m_startPoint.ry());
}

int McEllipse::shapeType() {
	return Mc::Ellipse;
}
