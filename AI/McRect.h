#pragma once

#include "McAbstractShape.h"
#include <qscopedpointer.h>

struct McRectData;

class McRect : public McAbstractShape{
	Q_OBJECT

public:
	explicit McRect(QObject *parent = 0);
	virtual ~McRect();

	void setPos(const QPoint &startPoint, const QPoint &endPoint, bool isStraight) override;

	QPoint startPos() const  override;
	QPoint endPos() const  override;

	void paint(QPainter& paint) override;
	int shapeType() override;

	bool undo() override;
	bool redo() override;

	bool isRelated(const QPoint &point, bool isDragShape) override;
	void moveStart() override;
	void move(const QPoint &pointOffset) override;

	void getBorder(int &top, int &bottom, int &left, int &right, const QSize &canvasSize) override;

private:
	QScopedPointer<McRectData> d;
};
