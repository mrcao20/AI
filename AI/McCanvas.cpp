#include "McCanvas.h"
#include <qpixmap.h>
#include <qevent.h>
#include <QToolTip>
#include "McGlobal.h"
#include "McShapeFactory.h"
#include "Output.h"

struct McCanvasData {
	Mc::ShapeType m_shapeType;
	McShapeFactory m_shapeFactory;
	QPixmap m_pixmap;
	QStack<McShapeInterface *> m_shapeStack;
	QStack<McShapeInterface *> m_redoShapeStack;	//�����ָ���
	bool m_isDrawing;   //��־�Ƿ����ڻ�ͼ
	McShapeInterface *m_currShape;
	bool m_isStraight;
	int m_lineWidth;
	QColor m_lineColor;
	QColor m_brushColor;
	bool m_isStopPolygon;	// ���ƶ�����Ƿ����
	bool m_isUndoStopPolygon;	// ��������ʱ�����浱ǰ�Ƿ���ƶ���ν�����״̬
	bool m_isCtrlPress;
	bool m_isShiftPress;	// ���ں�ctrl��һ���ж��Ƿ�Ϊ�϶�����ͼ��
	bool m_isLeftButtonPress;
	QPoint m_shapeMoveStartPos;
	int m_penStartIndex;	// ʹ��penֱ�ӻ�ͼʱͼ�ε���ʼ�±�
	bool m_isAntialiasing;	// �Ƿ�ʹ�ÿ����
};

McCanvas::McCanvas(QWidget *parent)
	: QFrame(parent)
	, d(new McCanvasData)
{
	d->m_isCtrlPress = false;
	d->m_isShiftPress = false;
	d->m_isLeftButtonPress = false;
	setMouseTracking(true);
}

McCanvas::~McCanvas() {
}

void McCanvas::init() {
	d->m_currShape = NULL;
	d->m_isStraight = false;
	d->m_brushColor = Qt::transparent;
	d->m_lineColor = Qt::black;
	d->m_isStopPolygon = true;
	d->m_pixmap = QPixmap(size());
	d->m_pixmap.fill(Qt::transparent);
	update();
}

void McCanvas::reset() {
	shapeStackClear(d->m_shapeStack);
	shapeStackClear(d->m_redoShapeStack);
	init();
}

void McCanvas::undo() {
	if (d->m_shapeStack.isEmpty())
		return;
	d->m_currShape = d->m_shapeStack.top();
	/*	�����������ȳ������ж��Ƿ�����һ��
		�������ж�Pen��״̬����ִ�г���
	*/
	// �����Ҫ������ͼ����pen����ôѰ���ٽ�ֵ����������ͼ��
	if (d->m_currShape->shapeType() == Mc::Pen) {
		while (!d->m_shapeStack.isEmpty()) {
			d->m_shapeStack.pop();
			d->m_redoShapeStack.push(d->m_currShape);
			d->m_currShape = d->m_shapeStack.top();
			if (d->m_currShape->startPos() == QPoint(-1, -1) && d->m_currShape->endPos() == QPoint(-1, -1))
				break;
		}
	}
	/*	��Ϊ��ǰͼ�β�δ����������size-1��Ȼ�ǵ�ǰͼ��
		����һ��ͼ��(size-2)��Penʱ������Pen�Ŀ�ʼ��
	*/
	if (d->m_shapeStack.size() >= 2 && d->m_shapeStack.at(d->m_shapeStack.size() - 2)->shapeType() == Mc::Pen) {
		for (int i = d->m_shapeStack.size() - 3; i >= 0; i--) {	// ��ΪPen�ĵ�һ��������һ���㶼�Ǳ�־λ������Ӧ���޳�
			if (d->m_shapeStack.at(i)->startPos() == QPoint(-1, -1) && d->m_shapeStack.at(i)->endPos() == QPoint(-1, -1)) {
				d->m_penStartIndex = i;
				break;
			}
		}
	}
	if (d->m_currShape->undo()) {	// ����true����ʾ������ͼ��
		if (d->m_redoShapeStack.isEmpty()) {
			d->m_isUndoStopPolygon = d->m_isStopPolygon;	// ���������Ƿ���ƽ�����״̬�������ָ�ʹ��
			d->m_isStopPolygon = true;
		}
		d->m_shapeStack.pop();	// ȥ��������һ��ͼ�Σ�������ָ�ջ
		d->m_redoShapeStack.push(d->m_currShape);
		d->m_currShape = d->m_shapeStack.isEmpty() ? NULL : d->m_shapeStack.top();	// ��ȡ�µ��������ͼ��
	}
	// ��ջ��壬�����»�������ͼ��
	drawPixmap();
	update();
}

void McCanvas::redo() {
	static bool isShapeStackEmpty = false;	// ���ͼ��ջΪ�գ���һ��ֻ�ָ���һ��ͼ��
	// ��ȡ��ǰ���ϲ�ͼ��
	d->m_currShape = d->m_shapeStack.isEmpty() ? NULL : d->m_shapeStack.top();
	if (!d->m_currShape) {
		d->m_currShape = d->m_redoShapeStack.isEmpty() ? NULL : d->m_redoShapeStack.pop();
		if (!d->m_currShape)
			return;
		isShapeStackEmpty = true;
		d->m_shapeStack.push(d->m_currShape);
	}
	if (d->m_currShape->redo() && !isShapeStackEmpty) {	// ����true����ʾ�ָ���һ��ͼ��
		d->m_currShape = d->m_redoShapeStack.isEmpty() ? NULL : d->m_redoShapeStack.pop();
		if (d->m_redoShapeStack.isEmpty() && d->m_isStopPolygon) {	// �����������ڻ�����Σ����ٽ���
			d->m_isStopPolygon = d->m_isUndoStopPolygon;
			d->m_isUndoStopPolygon = true;
		}
		if (!d->m_currShape) {
			d->m_currShape = d->m_shapeStack.top();
			return;
		}
		d->m_shapeStack.push(d->m_currShape);
	}
	/*	�ָ�������Ҫ�Ƚ���Ҫ�ָ���ͼ��ѹ��ջ�����ж�
		������Ҫ��ִ�лָ����������ж�Pen��״̬
	*/
	// �����Ҫ�ָ���ͼ����pen����ôѰ���ٽ�ֵ����������ͼ��
	if (d->m_currShape->shapeType() == Mc::Pen) {
		/*	��Ϊ�ָ������ǳ����������������˳��ͳ����෴
			���Իָ������ͻ�ͼ�Ĳ���˳��һ�£����Ա��ָ��ĵ�һ�������Pen�Ŀ�ʼ��
		*/
		d->m_penStartIndex = d->m_shapeStack.size() - 1;
		while (!d->m_redoShapeStack.isEmpty()) {
			d->m_currShape = d->m_redoShapeStack.pop();
			d->m_shapeStack.push(d->m_currShape);
			if (d->m_currShape->startPos() == QPoint(-1, -1) && d->m_currShape->endPos() == QPoint(-1, -1))
				break;
		}
	}
	isShapeStackEmpty = false;
	drawPixmap();
	update();
}

void McCanvas::setAntialiasing(bool isAntialiasing) {
	d->m_isAntialiasing = isAntialiasing;
}

void McCanvas::setStraight(bool isStraight) {
	d->m_isStraight = isStraight;
}

void McCanvas::setShapeType(int shapeType) {
	switch (shapeType) {
	case Mc::Rect:
		d->m_shapeType = Mc::Rect;
		break;
	case Mc::Polygon:
		d->m_shapeType = Mc::Polygon;
		break;
	case Mc::Ellipse:
		d->m_shapeType = Mc::Ellipse;
		break;
	case Mc::Line:
		d->m_shapeType = Mc::Line;
		break;
	case Mc::Pen:
		d->m_shapeType = Mc::Pen;
		break;
	}
}

void McCanvas::setLineWidth(int lineWidth) {
	d->m_lineWidth = lineWidth;
}

void McCanvas::setAlpha(int alpha) {
	d->m_brushColor.setAlpha(alpha);
}

void McCanvas::setLineColor(const QColor &color) {
	d->m_lineColor = color;
}

void McCanvas::setBrushColor(const QColor &color) {
	int tmp;
	int a;
	d->m_brushColor.getRgb(&tmp, &tmp, &tmp, &a);
	d->m_brushColor = color;
	d->m_brushColor.setAlpha(a);
}

void McCanvas::paintEvent(QPaintEvent *event){
	QPainter painter(this);
	static QPixmap pix;		// ����һ����̬��QPixmap�����ڴ�ŵ�ǰ�����δ���ƽ���ʱ��֮ǰ��ͼ����Ϣ
	if (d->m_currShape) {
		// Polygon���⴦��
		if (!d->m_isStopPolygon && pix.isNull()) {	// ��ǰ��������ڻ���
			// ���֮ǰͼ�񲻴��ڣ����ʾ����βŸոջ��ƣ��洢֮ǰͼ��
			pix = d->m_pixmap;
		}
		else if (!d->m_isStopPolygon && !pix.isNull()) {
			// ���֮ǰͼ����ڣ���ʾ������Ѿ����й����ƣ�ȥ����ǰ����Σ���ԭ֮ǰͼ��
			d->m_pixmap = pix;
		}
		else if (d->m_isStopPolygon && !pix.isNull()) {	// ���֮ǰͼ����ڣ�������ν�����ʾΪtrue�����ʾ����λ��ƽ�������ԭͼ�񣬲����֮ǰͼ��
			if(d->m_shapeType == Mc::Polygon)	// ������»��Ƶ���ȻΪ����Σ���ԭ֮ǰͼ�񣬷��򲻻�ԭ��ֻ�ÿ�
				d->m_pixmap = pix;
			pix = QPixmap();
		}
		if (d->m_isDrawing) { //������ڻ�ͼ�����ڸ��������ϻ���
			//����ǰpix�е����ݸ��Ƶ�tempPix�У���֤��ǰ�����ݲ���ʧ
			QPixmap tempPix = d->m_pixmap;
			QPainter pp(&tempPix);
			paint(pp);
			painter.drawPixmap(0, 0, tempPix);
		}
		else {
			QPainter pp(&d->m_pixmap);
			paint(pp);
			painter.drawPixmap(0, 0, d->m_pixmap);
			int top = -1, bottom = -1, left = -1, right = -1;
			for (McShapeInterface *shape : d->m_shapeStack)
				shape->getBorder(top, bottom, left, right, size());
			int w = right - left;
			int h = bottom - top;
			QPixmap pix = d->m_pixmap.copy(left, top, w, h);
			hasImage(pix.toImage());
		}
	}
	else {
		painter.drawPixmap(0, 0, d->m_pixmap);
		if (!pix.isNull())
			pix = QPixmap();
	}
}

void McCanvas::mousePressEvent(QMouseEvent *event){
	while (!d->m_redoShapeStack.isEmpty()){
		McShapeInterface *shape = d->m_redoShapeStack.pop();
		delete shape;
	}
	////////////Polygon���⴦��/////////////////
	if (event->button() == Qt::RightButton && !d->m_isStopPolygon){	// ������ڻ��ƶ��������룬���򲻽���
		d->m_isStopPolygon = true;
		update();
		return;
	}
	//���������£���ctrl��δ���£�ִ�л�ͼ����
	if (event->button() == Qt::LeftButton && !d->m_isCtrlPress) { 
		d->m_isDrawing = true;   //���ڻ�ͼ
		if (d->m_shapeType == Mc::Polygon) {	// ������ڻ��ƶ���������⴦��
			if (!d->m_isStopPolygon && !d->m_shapeStack.isEmpty()) {	// ������ڻ��ƶ����
				if (d->m_shapeStack.top()->shapeType() != Mc::Polygon) {	//����״̬���ᴥ��
				//if (typeid(*d->m_shapeStack.top()) != typeid(McPolygon)) // ��ʹ������ʱ��⣬���ݷ��ص�shapeType���
					d->m_isStopPolygon = true;
					update();
					return;
				}
				d->m_currShape->setEndPos(event->pos());	// ��ӽ�����
				update();
				return;
			}else	// ����տ�ʼ���ƶ���Σ���ѱ�־λ��true
				d->m_isStopPolygon = false;
		}else // �����ǰ���Ƶ�ͼ�β��Ƕ���Σ����־λ��false
			d->m_isStopPolygon = true;
		// ���ù����õ�ͼ��
		d->m_currShape = d->m_shapeFactory.getShape(d->m_shapeType, d->m_isAntialiasing, this);
		if (!d->m_currShape)	// �����ڴ����͵�ͼ�Σ���ֱ�ӷ���
			return;
		if (d->m_shapeType == Mc::Pen){
			McShapeInterface *zero = d->m_shapeFactory.getShape(d->m_shapeType, d->m_isAntialiasing, this);
			zero->setPos(QPoint(-1, -1), QPoint(-1, -1), false);
			d->m_shapeStack.push(zero);
			d->m_penStartIndex = d->m_shapeStack.size() - 1;
		}//Pen ��������
		// ����ͼ������
		d->m_currShape->setShapeProperty(d->m_lineColor, d->m_brushColor, d->m_lineWidth);
		// ���ÿ�ʼ�ͽ��������Ƿ���ͼ��
		d->m_currShape->setPos(event->pos(), event->pos() + QPoint(0, 1), d->m_isStraight);
		// ��ͼ����ӽ�ͼ�ι���ջ���ó������ӳ�����ԭ����
		d->m_shapeStack.push(d->m_currShape);
		update();
	}
	//���������£�ctrl��Ҳ���£�ִ���϶��ı��С���϶�����ͼ�β���
	else if (event->button() == Qt::LeftButton && d->m_isCtrlPress) {
		if (d->m_currShape) {
			d->m_shapeStack.removeOne(d->m_currShape);
			d->m_shapeStack.push(d->m_currShape);
			d->m_currShape->moveStart();
			d->m_isLeftButtonPress = true;
			d->m_shapeMoveStartPos = event->pos();
		}
	}
	QFrame::mousePressEvent(event);
}

void McCanvas::mouseMoveEvent(QMouseEvent *event){
	if (d->m_isCtrlPress && !d->m_isLeftButtonPress) {
		QString posStr = QString("%1,%2").arg(event->pos().x()).arg(event->pos().y());
		QToolTip::showText(event->globalPos(), posStr, this);
		for (McShapeInterface *shape : d->m_shapeStack) {
			if (shape->isRelated(event->pos(), d->m_isShiftPress)) {
				d->m_currShape = shape;
				d->m_isStopPolygon = true;
				update();
				break;
			}
		}
	}
	else if (d->m_isCtrlPress && d->m_isLeftButtonPress) {
		if (d->m_currShape) {
			d->m_currShape->move(event->pos() - d->m_shapeMoveStartPos);
			d->m_shapeMoveStartPos = event->pos();
			drawPixmap();
			update();
		}
	}
	if (d->m_currShape && d->m_isDrawing) {	// ���ͼ�δ��ڲ��ҵ�ǰ���ڻ���
		d->m_currShape->setPos(d->m_currShape->startPos(), event->pos(), d->m_isStraight);
		////////////Pen���⴦��/////////////////
		if (d->m_shapeType == Mc::Pen) {
			d->m_currShape = d->m_shapeFactory.getShape(d->m_shapeType, d->m_isAntialiasing, this);
			d->m_currShape->setShapeProperty(d->m_lineColor, d->m_brushColor, d->m_lineWidth);
			d->m_currShape->setPos(event->pos(), event->pos(), false);
			d->m_shapeStack.push(d->m_currShape);
		}//Pen ��������
		update();
	}
	QFrame::mouseMoveEvent(event);
}

void McCanvas::mouseReleaseEvent(QMouseEvent *event){
	if (event->button() == Qt::LeftButton && !d->m_isCtrlPress && d->m_isDrawing) { //�������ͷ�
		d->m_isDrawing = false;    //������ͼ
		////////////Pen���⴦��/////////////////
		if (d->m_shapeType == Mc::Pen) {
			McShapeInterface *zero = d->m_shapeFactory.getShape(d->m_shapeType, d->m_isAntialiasing, this);
			zero->setPos(QPoint(-1, -1), QPoint(-1, -1), false);
			d->m_shapeStack.push(zero);
		}//Pen ��������
		update();
	}
	else if (event->button() == Qt::LeftButton && d->m_isCtrlPress) {
		d->m_isLeftButtonPress = false;
	}
	QFrame::mouseReleaseEvent(event);
}

void McCanvas::resizeEvent(QResizeEvent * event) {
	if (height() > d->m_pixmap.height() || width() > d->m_pixmap.width()) {
		QPixmap newPix(size());
		newPix.fill(Qt::transparent);
		QPainter p(&newPix);
		p.drawPixmap(0, 0, d->m_pixmap);
		d->m_pixmap = newPix;
	}
	QFrame::resizeEvent(event);
}

void McCanvas::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Shift && !d->m_isCtrlPress)
		setStraight(true);	// ����ͼ
	else if (event->key() == Qt::Key_Shift && d->m_isCtrlPress) {
		d->m_isShiftPress = true;
	}
	else if (event->key() == Qt::Key_Control) {
		d->m_currShape = NULL;
		d->m_isCtrlPress = true;
	}
	QFrame::keyPressEvent(event);
}

void McCanvas::keyReleaseEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Shift && !d->m_isCtrlPress)
		setStraight(false);	// ȡ������ͼ
	else if (event->key() == Qt::Key_Shift && d->m_isCtrlPress) {
		d->m_isShiftPress = false;
	}
	else if (event->key() == Qt::Key_Control) {
		d->m_isCtrlPress = false;
		d->m_isShiftPress = false;
		d->m_currShape = d->m_shapeStack.isEmpty() ? NULL : d->m_shapeStack.top();	// ��ȡ�µ��������ͼ��
	}
	QFrame::keyReleaseEvent(event);
}

void McCanvas::shapeStackClear(QStack<McShapeInterface *> &stack) {
	while (!stack.isEmpty()) {
		McShapeInterface *shape = stack.pop();
		delete shape;
	}
	stack.swap(QStack<McShapeInterface *>());
}

void McCanvas::drawPixmap() {
	d->m_pixmap = QPixmap(size());
	d->m_pixmap.fill(Qt::transparent);
	QPainter pp(&d->m_pixmap);
	for (int i = 0; i < d->m_shapeStack.size() - 1; i++) {	// ���һ��ͼ�β��ڴ˴�������ʹ��paintEvent��
		// �����Pen�ǵ�ǰ���һ��ͼ�Σ��򲻻��������滹��������һ��ͼ��ʱ�����Pen��Ȼ�ử
		if (i == d->m_penStartIndex && d->m_shapeStack.at(d->m_shapeStack.size() - 1)->shapeType() == Mc::Pen)
			break;
		// �����paint�������ǳ�����������û�иı�shape���κ�״̬��ֻ�Ǵ����pp���������˸ı䣬�ʴ˴�ʹ��at����
		McShapeInterface *shape = d->m_shapeStack.at(i);
		shape->paint(pp);
	}
}

void McCanvas::paint(QPainter& painter) {
	if (d->m_currShape->shapeType() == Mc::Pen) {
		for (int i = d->m_penStartIndex; i < d->m_shapeStack.size(); i++) {
			McShapeInterface *shape = d->m_shapeStack.at(i);
			shape->paint(painter);
		}
	}
	else
		d->m_currShape->paint(painter);
}
