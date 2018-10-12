#pragma once
#pragma execution_character_set("utf-8")

#include <QtWidgets/QWidget>
#include <qscopedpointer.h>

QT_BEGIN_NAMESPACE
class QCloseEvent;
QT_END_NAMESPACE

struct GraphicalProcessesData;

class GraphicalProcesses : public QWidget{
	Q_OBJECT

public:
	explicit GraphicalProcesses(QWidget *parent = Q_NULLPTR);
	~GraphicalProcesses();

	void init();

signals:
	void quit();

protected:
	void closeEvent(QCloseEvent *event) override;

private:
	QScopedPointer<GraphicalProcessesData> d;
};
