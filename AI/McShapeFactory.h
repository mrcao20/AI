#pragma once

#include "McShapeInterface.h"
#include <qobject.h>

class McShapeFactory{

public:
	McShapeInterface *getShape(int shapeType, bool isAntialiasing, QObject *parent = 0);
};

