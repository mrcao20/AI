#include "TcpServer.h"
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <qlist.h>
#include "Output.h"

struct TcpServerData {
	QTcpServer *m_tcpServer;
	QList<QTcpSocket *> m_tcpClients;
};

TcpServer::TcpServer(QObject *parent)
	: QObject(parent)
	, d(new TcpServerData)
{
	
}

void TcpServer::init() {
	d->m_tcpServer = new QTcpServer(this);
	d->m_tcpServer->listen(QHostAddress::Any, 8000);
	connect(d->m_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
}

void TcpServer::newConnectionSlot(){
	QTcpSocket *currentClient = d->m_tcpServer->nextPendingConnection();
	d->m_tcpClients.append(currentClient);
	QString ip_port = QString("%1:%2").arg(currentClient->peerAddress().toString().split("::ffff:")[1])\
		.arg(currentClient->peerPort());
	connect(currentClient, &QTcpSocket::readyRead, [this, currentClient]() {
		QByteArray b = currentClient->readAll();
		output << QString::fromUtf8(QByteArray::fromBase64(b));
	});
	connect(currentClient, &QTcpSocket::disconnected, [this, currentClient, ip_port]() {
		d->m_tcpClients.removeOne(currentClient);
		currentClient->destroyed();
		currentClient->deleteLater();
		output << ip_port + " disconnected";
	});
	output << ip_port + " connected";
}

void TcpServer::disconnectedAll() {
	while(!d->m_tcpClients.isEmpty()) {//断开所有连接
		d->m_tcpClients.first()->disconnectFromHost();
	}
	d->m_tcpServer->close();     //不再监听端口
}

TcpServer::~TcpServer(){
	disconnectedAll();
}
