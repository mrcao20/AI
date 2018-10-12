#pragma once

#include "McShapeInterface.h"
#include <qobject.h>

class McShapeFactory{

public:
	McShapeInterface *getShape(int shapeType, QObject *parent = 0);
};

