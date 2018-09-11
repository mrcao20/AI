#pragma once

#include "AbstractOutputBox.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

class OutputBox : public AbstractOutputBox {
	Q_OBJECT

public:
	OutputBox(QWidget *parent = Q_NULLPTR);

	void SetMesseage(const QString &strMessage, const QPoint *pPoint = nullptr);

protected:
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);

public slots :
	void clearEdit();

private:
	void OnTimer();
	QSize getStringSize(QString &);
	QSize getEditBoxSize(const QString &strMessage);
	QRect getScreenRect();

private:
	QTextEdit *textEdit;
	QTimer  *mpTimer;
	bool     mbEnter;
	int      mnTransparent;
	int maxWidth;
	int maxHeight;
	int wordHeight;

};