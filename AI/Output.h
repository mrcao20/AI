#pragma once
#pragma execution_character_set("utf-8")

#include <QtCore/QObject>

#ifndef OUTPUT
#define output *Output::instance()
#endif // !OUTPUT

class Output : public QObject{
	Q_OBJECT

public:
	static Output * instance();
	inline Output &operator<< (const QString& msg) { this->message(msg); return *this; }
	inline Output &operator<< (const char * msg) { this->message(QString(msg)); return *this; }
	inline Output &operator<< (const QChar * msg) { this->message(QString(msg)); return *this; }
	inline Output &operator<< (const QByteArray &msg) { this->message(msg); return *this; }

signals:
	void message(const QString &msg);

private:
	Output();
	static Output * m_instance;
};