#pragma once
#include "McAbstractShape.h"

class McPen : public McAbstractShape {
	Q_OBJECT

public:
	explicit McPen(QObject *parent = 0);
	virtual ~McPen();

	void paint(QPainter& paint) override;
	int shapeType() override;

};

