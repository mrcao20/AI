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

	virtual bool undo() = 0;	// ��������������Ҫ��������ͼ��ʱ����true����ʾɾ����ͼ�Σ����򷵻�false��ͼ���ڲ�ִ�г�������
	virtual bool redo() = 0;	// �ָ�����������ͼ���Ѿ������׻ָ�ʱ����true����ʾ�ָ���һ��ͼ�Σ����򷵻�false��ͼ���ڲ�ִ�лָ�����

	virtual bool isRelated(const QPoint &point, bool isDragShape) = 0;	// �жϵ�ǰ���Ƿ���ͼ����
	virtual void moveStart() = 0;	// �ƶ���ʼʱ���ã���Ҫ���ڻ�ó��������ĵ�
	virtual void move(const QPoint &pointOffset) = 0;	// ͼ���ƶ�������Ϊ�ƶ�ƫ����

	virtual void getBorder(int &top, int &bottom, int &left, int &right, const QSize &canvasSize) = 0;	// �õ�һ���߿�ֵ���Ը�������ͼ��

};
