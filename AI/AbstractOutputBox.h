#pragma once

#include <qwidget.h>
#include <qsize.h>

class AbstractOutputBox : public QWidget {

public:
	explicit AbstractOutputBox(QWidget *parent = Q_NULLPTR);
	virtual ~AbstractOutputBox();

	QRect mainWindowRect() { return m_mainWindowRect; }
	QPoint mainWindowPoint() { return m_mainWindowPoint; }
	void setMainWindowInfo(QRect mainWindowRect, QPoint mainWindowPoint){
		m_mainWindowRect = mainWindowRect;
		m_mainWindowPoint = mainWindowPoint;
	}
	void OutputBoxMove(const QPoint &, const QRect &);
	virtual void SetMesseage(const QString &strMessage, const QPoint *pPoint = nullptr) = 0;

protected:
	QRect m_mainWindowRect;
	QPoint m_mainWindowPoint;

};