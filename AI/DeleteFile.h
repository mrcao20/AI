#pragma once
#pragma execution_character_set("utf-8")

#include <qobject.h>

class DeleteFile : public QObject{
	Q_OBJECT

public:
	DeleteFile(QObject * = 0);

	void setFilePath(QString &filePath);
	void setFilterString(QString &filterString);

signals:
	void finished();

public slots:
	void process();

private:
	bool DelDir(const QString &path);

private:
	QString filePath;
	QString filterString;

};