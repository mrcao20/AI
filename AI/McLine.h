#pragma once
#include "McAbstractShape.h"
#include <qscopedpointer.h>

struct McLineData;

class McLine : public McAbstractShape{
	Q_OBJECT

public:
	explicit McLine(QObject *parent = 0);
	virtual ~McLine();

	void paint(QPainter& paint) override;
	int shapeType() override;

	bool undo() override;
	bool redo() override;

	bool isRelated(const QPoint &point, bool isDragShape) override;
	void moveStart() override;
	void move(const QPoint &pointOffset) override;

private:
	bool isRelated(const QPoint &point, const QPoint &checkP, bool isDragShape, int i);

private:
	QScopedPointer<McLineData> d;
};

