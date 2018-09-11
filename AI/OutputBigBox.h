#pragma once

#include "AbstractOutputBox.h"

struct OutputBoxNormalDataPrivate;

class OutputBigBox : public AbstractOutputBox {

public:
	explicit OutputBigBox(QWidget *parent = 0);
	virtual ~OutputBigBox();

	virtual void SetMesseage(const QString &strMessage, const QPoint *pPoint = nullptr);

private:
	QScopedPointer<OutputBoxNormalDataPrivate> d;

};