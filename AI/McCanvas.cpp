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
	QStack<McShapeInterface *> m_redoShapeStack;	//撤销恢复用
	bool m_isDrawing;   //标志是否正在绘图
	McShapeInterface *m_currShape;
	bool m_isStraight;
	int m_lineWidth;
	QColor m_lineColor;
	QColor m_brushColor;
	bool m_isStopPolygon;	// 绘制多边形是否结束
	bool m_isUndoStopPolygon;	// 撤销操作时，保存当前是否绘制多边形结束的状态
	bool m_isCtrlPress;
	bool m_isShiftPress;	// 用于和ctrl键一起判断是否为拖动整个图形
	bool m_isLeftButtonPress;
	QPoint m_shapeMoveStartPos;
	int m_penStartIndex;	// 使用pen直接画图时图形的起始下标
	bool m_isAntialiasing;	// 是否使用抗锯齿
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
	/*	撤销操作是先撤销再判断是否撤销下一个
		所以先判断Pen的状态，再执行撤销
	*/
	// 如果需要撤销的图形是pen，那么寻找临界值，撤销整个图形
	if (d->m_currShape->shapeType() == Mc::Pen) {
		while (!d->m_shapeStack.isEmpty()) {
			d->m_shapeStack.pop();
			d->m_redoShapeStack.push(d->m_currShape);
			d->m_currShape = d->m_shapeStack.top();
			if (d->m_currShape->startPos() == QPoint(-1, -1) && d->m_currShape->endPos() == QPoint(-1, -1))
				break;
		}
	}
	/*	因为当前图形并未弹出，所以size-1仍然是当前图形
		当下一个图形(size-2)是Pen时，查找Pen的开始点
	*/
	if (d->m_shapeStack.size() >= 2 && d->m_shapeStack.at(d->m_shapeStack.size() - 2)->shapeType() == Mc::Pen) {
		for (int i = d->m_shapeStack.size() - 3; i >= 0; i--) {	// 因为Pen的第一个点和最后一个点都是标志位，所以应该剔除
			if (d->m_shapeStack.at(i)->startPos() == QPoint(-1, -1) && d->m_shapeStack.at(i)->endPos() == QPoint(-1, -1)) {
				d->m_penStartIndex = i;
				break;
			}
		}
	}
	if (d->m_currShape->undo()) {	// 返回true，表示撤销该图形
		if (d->m_redoShapeStack.isEmpty()) {
			d->m_isUndoStopPolygon = d->m_isStopPolygon;	// 保存多边形是否绘制结束的状态，以做恢复使用
			d->m_isStopPolygon = true;
		}
		d->m_shapeStack.pop();	// 去掉最上面一个图形，并推入恢复栈
		d->m_redoShapeStack.push(d->m_currShape);
		d->m_currShape = d->m_shapeStack.isEmpty() ? NULL : d->m_shapeStack.top();	// 获取新的最上面的图形
	}
	// 清空画板，并重新绘制所有图形
	drawPixmap();
	update();
}

void McCanvas::redo() {
	static bool isShapeStackEmpty = false;	// 如果图形栈为空，则一次只恢复第一个图形
	// 获取当前最上层图形
	d->m_currShape = d->m_shapeStack.isEmpty() ? NULL : d->m_shapeStack.top();
	if (!d->m_currShape) {
		d->m_currShape = d->m_redoShapeStack.isEmpty() ? NULL : d->m_redoShapeStack.pop();
		if (!d->m_currShape)
			return;
		isShapeStackEmpty = true;
		d->m_shapeStack.push(d->m_currShape);
	}
	if (d->m_currShape->redo() && !isShapeStackEmpty) {	// 返回true，表示恢复下一个图形
		d->m_currShape = d->m_redoShapeStack.isEmpty() ? NULL : d->m_redoShapeStack.pop();
		if (d->m_redoShapeStack.isEmpty() && d->m_isStopPolygon) {	// 如果本身就正在画多边形，则不再进入
			d->m_isStopPolygon = d->m_isUndoStopPolygon;
			d->m_isUndoStopPolygon = true;
		}
		if (!d->m_currShape) {
			d->m_currShape = d->m_shapeStack.top();
			return;
		}
		d->m_shapeStack.push(d->m_currShape);
	}
	/*	恢复操作需要先将需要恢复的图形压入栈，再判断
		所以需要先执行恢复操作，再判断Pen的状态
	*/
	// 如果需要恢复的图形是pen，那么寻找临界值，撤销整个图形
	if (d->m_currShape->shapeType() == Mc::Pen) {
		/*	因为恢复操作是撤销的逆操作，所以顺序和撤销相反
			所以恢复操作和画图的操作顺序一致，所以被恢复的第一个点就是Pen的开始点
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
	static QPixmap pix;		// 定义一个静态的QPixmap，用于存放当前多边形未绘制结束时，之前的图像信息
	if (d->m_currShape) {
		// Polygon特殊处理
		if (!d->m_isStopPolygon && pix.isNull()) {	// 当前多边形正在绘制
			// 如果之前图像不存在，则表示多边形才刚刚绘制，存储之前图像
			pix = d->m_pixmap;
		}
		else if (!d->m_isStopPolygon && !pix.isNull()) {
			// 如果之前图像存在，表示多边形已经进行过绘制，去掉当前多边形，还原之前图像
			d->m_pixmap = pix;
		}
		else if (d->m_isStopPolygon && !pix.isNull()) {	// 如果之前图像存在，而多边形结束表示为true，则表示多边形绘制结束，还原图像，并清空之前图像
			if(d->m_shapeType == Mc::Polygon)	// 如果最新绘制的仍然为多边形，则还原之前图像，否则不还原，只置空
				d->m_pixmap = pix;
			pix = QPixmap();
		}
		if (d->m_isDrawing) { //如果正在绘图，就在辅助画布上绘制
			//将以前pix中的内容复制到tempPix中，保证以前的内容不消失
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
	////////////Polygon特殊处理/////////////////
	if (event->button() == Qt::RightButton && !d->m_isStopPolygon){	// 如果正在绘制多边形则进入，否则不进入
		d->m_isStopPolygon = true;
		update();
		return;
	}
	//鼠标左键按下，但ctrl键未按下，执行绘图操作
	if (event->button() == Qt::LeftButton && !d->m_isCtrlPress) { 
		d->m_isDrawing = true;   //正在绘图
		if (d->m_shapeType == Mc::Polygon) {	// 如果正在绘制多边形则特殊处理
			if (!d->m_isStopPolygon && !d->m_shapeStack.isEmpty()) {	// 如果正在绘制多边形
				if (d->m_shapeStack.top()->shapeType() != Mc::Polygon) {	//正常状态不会触发
				//if (typeid(*d->m_shapeStack.top()) != typeid(McPolygon)) // 不使用运行时检测，根据返回的shapeType检测
					d->m_isStopPolygon = true;
					update();
					return;
				}
				d->m_currShape->setEndPos(event->pos());	// 添加结束点
				update();
				return;
			}else	// 如果刚开始绘制多边形，则把标志位置true
				d->m_isStopPolygon = false;
		}else // 如果当前绘制的图形不是多边形，则标志位置false
			d->m_isStopPolygon = true;
		// 利用工厂得到图形
		d->m_currShape = d->m_shapeFactory.getShape(d->m_shapeType, d->m_isAntialiasing, this);
		if (!d->m_currShape)	// 不存在此类型的图形，则直接返回
			return;
		if (d->m_shapeType == Mc::Pen){
			McShapeInterface *zero = d->m_shapeFactory.getShape(d->m_shapeType, d->m_isAntialiasing, this);
			zero->setPos(QPoint(-1, -1), QPoint(-1, -1), false);
			d->m_shapeStack.push(zero);
			d->m_penStartIndex = d->m_shapeStack.size() - 1;
		}//Pen 制造间隔点
		// 设置图形属性
		d->m_currShape->setShapeProperty(d->m_lineColor, d->m_brushColor, d->m_lineWidth);
		// 设置开始和结束点与是否正图形
		d->m_currShape->setPos(event->pos(), event->pos() + QPoint(0, 1), d->m_isStraight);
		// 将图形添加进图形管理栈，让程序增加撤销还原功能
		d->m_shapeStack.push(d->m_currShape);
		update();
	}
	//鼠标左键按下，ctrl键也按下，执行拖动改变大小和拖动整个图形操作
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
	if (d->m_currShape && d->m_isDrawing) {	// 如果图形存在并且当前正在绘制
		d->m_currShape->setPos(d->m_currShape->startPos(), event->pos(), d->m_isStraight);
		////////////Pen特殊处理/////////////////
		if (d->m_shapeType == Mc::Pen) {
			d->m_currShape = d->m_shapeFactory.getShape(d->m_shapeType, d->m_isAntialiasing, this);
			d->m_currShape->setShapeProperty(d->m_lineColor, d->m_brushColor, d->m_lineWidth);
			d->m_currShape->setPos(event->pos(), event->pos(), false);
			d->m_shapeStack.push(d->m_currShape);
		}//Pen 制造间隔点
		update();
	}
	QFrame::mouseMoveEvent(event);
}

void McCanvas::mouseReleaseEvent(QMouseEvent *event){
	if (event->button() == Qt::LeftButton && !d->m_isCtrlPress && d->m_isDrawing) { //鼠标左键释放
		d->m_isDrawing = false;    //结束绘图
		////////////Pen特殊处理/////////////////
		if (d->m_shapeType == Mc::Pen) {
			McShapeInterface *zero = d->m_shapeFactory.getShape(d->m_shapeType, d->m_isAntialiasing, this);
			zero->setPos(QPoint(-1, -1), QPoint(-1, -1), false);
			d->m_shapeStack.push(zero);
		}//Pen 制造间隔点
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
		setStraight(true);	// 画正图
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
		setStraight(false);	// 取消画正图
	else if (event->key() == Qt::Key_Shift && d->m_isCtrlPress) {
		d->m_isShiftPress = false;
	}
	else if (event->key() == Qt::Key_Control) {
		d->m_isCtrlPress = false;
		d->m_isShiftPress = false;
		d->m_currShape = d->m_shapeStack.isEmpty() ? NULL : d->m_shapeStack.top();	// 获取新的最上面的图形
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
	for (int i = 0; i < d->m_shapeStack.size() - 1; i++) {	// 最后一个图形不在此处画，将使用paintEvent画
		// 如果该Pen是当前最后一个图形，则不画，当后面还存在任意一个图形时，则该Pen仍然会画
		if (i == d->m_penStartIndex && d->m_shapeStack.at(d->m_shapeStack.size() - 1)->shapeType() == Mc::Pen)
			break;
		// 这里的paint函数不是常函数，并且没有改变shape的任何状态，只是传入的pp参数发生了改变，故此处使用at函数
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
