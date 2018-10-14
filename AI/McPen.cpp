#include "McPen.h"
#include "McGlobal.h"

McPen::McPen(QObject *parent)
	: McAbstractShape(parent)
{

}

McPen::~McPen(){
}

void McPen::paint(QPainter &paint) {
	if (m_startPoint == QPoint(-1, -1) && m_endPoint == QPoint(-1, -1))
		return;
	paint.setPen(QPen(m_lineColor, m_lineWidth));
	paint.setBrush(m_brushColor);
	if (m_isAntialiasing)	// Ĭ��Ϊ��ʹ�ÿ���ݣ������Ҫָ����ָ������
		paint.setRenderHint(QPainter::Antialiasing);
	paint.drawLine(m_startPoint, m_endPoint);
}

int McPen::shapeType() {
	return Mc::Pen;
}
