#include "OutputTransform.h"
#include <qtimer.h>
#include <QtTextToSpeech\qtexttospeech.h>
#include "OutputBox.h"
#include "OutputBigBox.h"
#include "McOutput.h"
#include "Output.h"

struct OutputTransformDataPrivate{
	QScopedPointer<QTextToSpeech> m_textSpeech;
	QScopedPointer<AbstractOutputBox> m_outputBox;
	QTimer * m_timer;
	QTimer *m_speechTimer;
	QTimer *m_textTimer;
	QStringList m_outputTextList;
	QString m_title;
	QString m_changedTitle;
	bool m_isCmd;
};

OutputTransform::OutputTransform(QObject *parent)
	: QObject(parent)
	, d(new OutputTransformDataPrivate) {
	d->m_textSpeech.reset();
	d->m_outputBox.reset(new OutputBox());
	d->m_timer = new QTimer(this);
	d->m_title = "text";
	d->m_isCmd = false;

	d->m_speechTimer = new QTimer(this);
	d->m_textTimer = new QTimer(this);

	connect(d->m_speechTimer, &QTimer::timeout, [this]() {
		if (d->m_textSpeech != nullptr) d->m_textSpeech.reset();
		d->m_speechTimer->stop();
	});
	connect(d->m_textTimer, &QTimer::timeout, [this]() {
		if (d->m_outputBox != nullptr) d->m_outputBox.reset();
		d->m_textTimer->stop();
	});

	d->m_timer->setInterval(30000);
	connect(d->m_timer, &QTimer::timeout, [this]() {
		d->m_outputTextList.removeFirst();
		if (d->m_outputTextList.empty()) d->m_timer->stop();
	});
	connect(McOutputInstance, SIGNAL(message(QString)), this, SLOT(handleOutputText(QString)));
	connect(Output::instance(), SIGNAL(message(QString)), this, SLOT(handleOutputText(QString)));
}

void OutputTransform::outputMove(const QPoint &point, const QRect &rect) {
	d->m_outputBox->OutputBoxMove(point, rect);
}

QString OutputTransform::getTextListLast(QStringList stringList) const {
	if (stringList.empty()) return nullptr;
	return stringList.last();
}

void OutputTransform::hide() {
	if (d->m_outputBox)
		d->m_outputBox->hide();
}

void OutputTransform::show() {
	if (d->m_outputBox)
		d->m_outputBox->show();
}

void OutputTransform::changedToFixedOutptuBox() {
	QRect mainWindowRect = d->m_outputBox->mainWindowRect();
	QPoint mainWindowPoint = d->m_outputBox->mainWindowPoint();
	d->m_isCmd = true;
	d->m_changedTitle = d->m_title;
	outputTitleChange("text");
	d->m_outputBox.reset(new OutputBigBox);
	d->m_outputBox->setMainWindowInfo(mainWindowRect, mainWindowPoint);
	d->m_outputBox->OutputBoxMove(mainWindowPoint, mainWindowRect);
}

void OutputTransform::changedToNormalOutputBox() {
	QRect mainWindowRect = d->m_outputBox->mainWindowRect();
	QPoint mainWindowPoint = d->m_outputBox->mainWindowPoint();
	d->m_isCmd = false;
	outputTitleChange(d->m_changedTitle);
	d->m_outputBox.reset(new OutputBox());
	d->m_outputBox->setMainWindowInfo(mainWindowRect, mainWindowPoint);
}

void OutputTransform::handleOutputText(const QString &text) {
	if (text == nullptr || text.isEmpty()) return;
	d->m_outputTextList.append(text);
	if (d->m_outputTextList.size() > 10) d->m_outputTextList.removeFirst();
	if (!d->m_timer->isActive()) d->m_timer->start();

	outputMessage(text, d->m_title);
}

void OutputTransform::showHistory() {
	for (QString str : d->m_outputTextList) {
		outputMessage(str, d->m_title);
	}
}

void OutputTransform::outputMessage(const QString &str, QString &title) {
	if (str == nullptr || str.isEmpty()) return;
	if ("S T" == title) {
		d->m_outputBox->SetMesseage(str);
		d->m_textSpeech->say(str);
	}
	else if ("speech" == title) {
		d->m_textSpeech->say(str);
	}
	else {
		d->m_outputBox->SetMesseage(str);
	}
}

void OutputTransform::outputTitleChange(const QString &title) {
	if (d->m_isCmd && d->m_title == "text") return;
	d->m_title = title;
	if ("S T" == d->m_title) {
		if (d->m_textSpeech == nullptr) d->m_textSpeech.reset(new QTextToSpeech());
		if (d->m_outputBox == nullptr) d->m_outputBox.reset(new OutputBox());
		this->outputMessage(getTextListLast(d->m_outputTextList), d->m_title);
		d->m_speechTimer->stop();
		d->m_textTimer->stop();
	}
	else if ("speech" == d->m_title) {
		if (d->m_textSpeech == nullptr) d->m_textSpeech.reset(new QTextToSpeech());
		this->outputMessage(getTextListLast(d->m_outputTextList), d->m_title);
		d->m_speechTimer->stop();
		d->m_textTimer->start(60000);
	}
	else if ("text" == d->m_title) {
		if (d->m_outputBox == nullptr) d->m_outputBox.reset(new OutputBox());
		this->outputMessage(getTextListLast(d->m_outputTextList), d->m_title);
		d->m_textTimer->stop();
		d->m_speechTimer->start(60000);
	}
}