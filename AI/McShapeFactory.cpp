#include "McShapeFactory.h"
#include "McGlobal.h"
#include "McRect.h"
#include "McPolygon.h"
#include "McEllipse.h"
#include "McLine.h"
#include "McPen.h"

McShapeInterface *McShapeFactory::getShape(int shapeType, QObject *parent) {
	McShapeInterface *shape = NULL;
	if (shapeType == Mc::Rect) {
		McRect *rect = new McRect(parent);
		shape = rect;
	}
	else if (shapeType == Mc::Polygon) {
		McPolygon *polygon = new McPolygon(parent);
		shape = polygon;
	}
	else if (shapeType == Mc::Ellipse) {
		McEllipse *ellipse = new McEllipse(parent);
		shape = ellipse;
	}
	else if (shapeType == Mc::Line) {
		McLine *line = new McLine(parent);
		shape = line;
	}
	else if (shapeType == Mc::Pen) {
		McPen *pen = new McPen(parent);
		shape = pen;
	}
	return shape;
}
