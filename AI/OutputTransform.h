#pragma once
#pragma execution_character_set("utf-8")
#include <qobject.h>
#include <qstringlist.h>
#include <qscopedpointer.h>

struct OutputTransformDataPrivate;
class OutputBox;

class OutputTransform : public QObject{
	Q_OBJECT

public:
	OutputTransform(QObject * = Q_NULLPTR);
	void outputMove(const QPoint &, const QRect &);

public slots:
	void hide();
	void show();
	void handleOutputText(const QString &text);
	void showHistory();
	void outputTitleChange(const QString &title);
	void changedToFixedOutptuBox();
	void changedToNormalOutputBox();

private:
	void outputMessage(const QString &, QString &);
	QString getTextListLast(QStringList) const;

private:
	QScopedPointer<OutputTransformDataPrivate> d;
	
};