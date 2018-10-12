#pragma once
#pragma execution_character_set("utf-8")
#include <QObject>
#include <qscopedpointer.h>

struct TcpServerData;

class TcpServer : public QObject {
	Q_OBJECT

public:
	explicit TcpServer(QObject *parent = 0);
	~TcpServer();

	void init();

private slots:
	void newConnectionSlot();

private:
	void disconnectedAll();

private:
	QScopedPointer<TcpServerData> d;
};
