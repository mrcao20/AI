#pragma once

#include "McAbstractShape.h"
#include <qscopedpointer.h>

struct McPolygonData;

class McPolygon : public McAbstractShape{
	Q_OBJECT

public:
	explicit McPolygon(QObject *parent = 0);
	virtual ~McPolygon();

	void paint(QPainter& paint) override;
	int shapeType() override;

	bool undo() override;
	bool redo() override;

	bool isRelated(const QPoint &point, bool isDragShape) override;
	void moveStart() override;
	void move(const QPoint &pointOffset) override;

	void setStartPos(const QPoint &point) override;
	void setEndPos(const QPoint &point) override;
	void setPos(const QPoint &startPoint, const QPoint &endPoint, bool isStraight) override;

	QPoint startPos() const  override;
	QPoint endPos() const  override;

private:
	QScopedPointer<McPolygonData> d;
};

