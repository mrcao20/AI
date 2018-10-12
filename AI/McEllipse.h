#pragma once
#include "McAbstractShape.h"

class McEllipse : public McAbstractShape{
	Q_OBJECT

public:
	explicit McEllipse(QObject *parent = 0);
	virtual ~McEllipse();

	void paint(QPainter& paint) override;
	int shapeType() override;

	void getBorder(int &top, int &bottom, int &left, int &right, const QSize &canvasSize) override;

};

