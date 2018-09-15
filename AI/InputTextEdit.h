#pragma once

#include <qplaintextedit.h>
#include <qscopedpointer.h>

QT_BEGIN_NAMESPACE
class QMouseEvent;
QT_END_NAMESPACE

struct MyTextEditDataPrivate;

class InputTextEdit : public QPlainTextEdit
{
	Q_OBJECT

public:
	explicit InputTextEdit(QWidget *parent = 0);
	virtual ~InputTextEdit();

protected:
	void mouseDoubleClickEvent(QMouseEvent *) override;
	void mouseReleaseEvent(QMouseEvent *) override;
	void mouseMoveEvent(QMouseEvent *) override;

private:
	QScopedPointer<MyTextEditDataPrivate> d;
};
