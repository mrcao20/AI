#include "Input.h"
#include "Output.h"
#include "InputEdit.h"
#include <qbitmap.h>
#include <qpainter.h>
#include <qapplication.h>
#include <qvector.h>
#include <qdebug.h>
#include <qdir.h>
#include <qprocess.h>

struct InputDataPrivate{
	QTextEdit * m_textEdit;
	QTimer *m_timer;
	QStringList m_command;
	QMap<QString, QStringList> m_cmd_map;
	QMap<QString, QTimer *> m_timer_map;
	QProcess m_cmdProcess;
	bool m_isCmd;
};

Input::Input(QWidget *parent)
	: QWidget(parent)
	, d(new InputDataPrivate) {

	d->m_textEdit = new InputEdit(this);
	d->m_isCmd = false;

	setStyleSheet("QMainWindow{background-color:black;border:none;}QTextEdit{background-color:black;color:rgb(0,255,0);border:none;}");
	setWindowOpacity(0.72);
	//setAttribute(Qt::WA_TranslucentBackground, true); //±³¾°Í¸Ã÷
	setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::SubWindow); //È¥³ý±ß¿ò //SubWindow

	this->resize(d->m_textEdit->size());
	QBitmap bmp(this->size());
	bmp.fill();
	QPainter p(&bmp);
	p.setRenderHint(QPainter::Antialiasing);  // ·´¾â³Ý;
	p.setPen(Qt::NoPen);
	p.setBrush(Qt::black);
	p.drawRoundedRect(bmp.rect(), 10, 10);
	setMask(bmp);

	d->m_timer = new QTimer(this);
	d->m_timer->setInterval(180000);
	connect(d->m_timer, SIGNAL(timeout()), this, SIGNAL(quit()));
	connect(d->m_textEdit, SIGNAL(hasInput(QString)), this, SLOT(handleInput(QString)));
	connect(d->m_textEdit, SIGNAL(completeCommand(QString)), this, SLOT(completeCommand(QString)));

	initCommand();

	show();
}

Input::~Input() {
	if (d->m_isCmd) {
		d->m_cmdProcess.write("exit\r\n");
		d->m_cmdProcess.kill();
		d->m_cmdProcess.waitForFinished();
		emit entryNormalMode();
		emit hideOutputBox();
	}
}

void Input::initCommand() {
	QDir dir;
#ifdef _DEBUG
	dir.setPath("../x64/Release/commands");
#else
	dir.setPath(qApp->applicationDirPath() + "/commands");
#endif // _DEBUG
	QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files);
	for (QFileInfo fileInfo : fileInfoList) {
		d->m_command.append(fileInfo.baseName());
	}
	d->m_command.append("cmd");
	/*d->m_command.append("change");
	d->m_command.append("show");
	d->m_command.append("lock");
	d->m_command.append("unlock");*/
}

bool Input::isExistCmd(const QString &cmd) {
#ifdef _DEBUG
	return QFile::exists(QString("../x64/Release/commands/%1").arg(cmd));
#else
	return QFile::exists(QString(qApp->applicationDirPath() + "/commands/%1").arg(cmd));
#endif // _DEBUG
}

QStringList Input::openCmdFile(const QString &fileName) {
	QString command;
	QString commandFilePath;
#ifdef _DEBUG
	commandFilePath = "../x64/Release/commands/" + fileName;
#else
	commandFilePath = qApp->applicationDirPath() + "/commands/" + fileName;
#endif // _DEBUG
	int charMaxSize = 100;
	QStringList commandList;
	
	command.reserve(charMaxSize);
	QFile file(commandFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		output << QString("can not open this file: %1").arg(commandFilePath);
		return commandList;
	}
	QTextStream in(&file);
	while (in.readLineInto(&command)) {
		if (command.isEmpty())
			continue;
		commandList.append(command);
	}
	file.close();
	return commandList;
}

void Input::addCmd(const QString &cmd) {
	if (!isContainsCmd(cmd)) {
		d->m_cmd_map[cmd] = openCmdFile(cmd);
		createTimeoutTimer(cmd);
	}
	startTimeoutTimer(cmd);
}

bool Input::isContainsCmd(const QString &cmd) {
	return d->m_cmd_map.contains(cmd);
}

void Input::createTimeoutTimer(const QString &cmd) {
	QTimer *cmdTimer = new QTimer(this);
	connect(cmdTimer, &QTimer::timeout, [this, cmdTimer, cmd]() {
		d->m_cmd_map.remove(cmd);
		cmdTimer->stop();
		delete cmdTimer;
		d->m_timer_map.remove(cmd);
	});
	d->m_timer_map[cmd] = cmdTimer;
}

inline void Input::startTimeoutTimer(const QString &cmd) {
	d->m_timer_map[cmd]->start(180000);
}

void Input::hideInputTextEdit() {
	setVisible(false);
	d->m_timer->start();
	if (d->m_isCmd) {
		emit hideOutputBox();
	}
}

QStringList &Input::getCmdList(const QStringList &list) {
	if (list.size() == 1)
		return d->m_command;
	else {
		QString cmd = list.first();
		addCmd(cmd);
		return d->m_cmd_map[cmd];
	}
}

void Input::completeCommand(const QString &command) {
	if (!command.startsWith(":")) return;

	QStringList list = command.right(command.size() - 1).split(QRegExp("\\s+"), QString::SkipEmptyParts);
	if (list.isEmpty()) return;
	if (list.size() > 2) return;
	if (list.size() > 1 && !d->m_command.contains(list.first()))
		return;

	QStringList commandList = getCmdList(list);
	if (commandList.isEmpty())
		return;
	
	int index = list.size() - 1;
	QString str = list.at(index);
	for (QString tempStr : commandList) {
		if (tempStr.startsWith(str)) {
			list.replace(index, tempStr);
			break;
		}
	}
	QString finalCommand = ":";
	for (QString tempStr : list) {
		finalCommand += tempStr;
		finalCommand += " ";
	}
	d->m_textEdit->setText(finalCommand);
	d->m_textEdit->moveCursor(QTextCursor::End);
}

void Input::displayInput() {
	if (isVisible()) {
		setVisible(false);
		d->m_timer->start();
	}
	else {
		setVisible(true);
		d->m_timer->stop();
		if (d->m_isCmd)
			emit showOutputBox();
	}
}

void Input::handleInput(const QString &str) {
	if (str.startsWith(":"))
		handleCommand(str.right(str.size() - 1));
	else
		handleMessage(str);
}

void Input::handleMessage(const QString &message) {
	if (!d->m_isCmd)
		output << message;
	bool isQuit = false;
	if (message == "exit")
		isQuit = true;
	d->m_cmdProcess.write(message.toUtf8() + "\r\n");
	if (isQuit) {
		d->m_cmdProcess.kill();
		d->m_cmdProcess.waitForFinished();
		emit entryNormalMode();
		output << "entry normal mode";
		d->m_isCmd = false;
	}
}

void Input::handleCommand(QString &command) {
	if (command.isEmpty()) return;
	
	if ("quit" == command.trimmed()) {
		hideInputTextEdit();
		return;
	}
	else if(command.trimmed() == "cmd") {
		if (d->m_isCmd)
			return;
		emit entryCmdMode();
		output << "entry cmd mode";
		d->m_isCmd = true;
		d->m_cmdProcess.start("cmd");
		connect(&d->m_cmdProcess, &QProcess::readyReadStandardOutput, [this]() {
			output << QString::fromLocal8Bit(d->m_cmdProcess.readAllStandardOutput());
		});
		connect(&d->m_cmdProcess, &QProcess::readyReadStandardError, [this]() {
			output << QString::fromLocal8Bit(d->m_cmdProcess.readAllStandardError());
		});
		return;
	}
	else if (command.trimmed() == "exit") {
		handleMessage("exit");
		return;
	}

	QStringList commandList = command.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	QString cmd = commandList.takeFirst();
	
	if (commandList.isEmpty())
		return;

	if (!isExistCmd(cmd)) {
		output << QString("without this command: %1").arg(cmd);
		return;
	}

	addCmd(cmd);

	runCmd(cmd, commandList);
}

void Input::runCmd(const QString &cmd, QStringList &list) {
	QStringList cmdList = d->m_cmd_map[cmd];
	QString sec_cmd = list.takeFirst();
	if (!cmdList.contains(sec_cmd))
		return;
	QString func = (cmd + firstCharToUpper(sec_cmd)).toStdString().c_str();
	if (list.size() > 10)
		output << "there are more than 10 parameters, it's will be aborted which after the 11th";
	QVector<QGenericArgument> arguments;
	arguments.reserve(10);
	for (int i = 0; i < 10; i++) {
		if (i < list.size()) {
			arguments.append(Q_ARG(QString, list.at(i)));
		}
		else {
			arguments.append(QGenericArgument());
		}
	}
	QMetaObject::invokeMethod(this, func.toLatin1(),
		arguments.at(0),
		arguments.at(1),
		arguments.at(2),
		arguments.at(3),
		arguments.at(4),
		arguments.at(5),
		arguments.at(6),
		arguments.at(7),
		arguments.at(8),
		arguments.at(9));
}

QString Input::firstCharToUpper(const QString &str) {
	if (str.isEmpty())
		return "";
	QString upperStr = str.at(0);
	return upperStr.toUpper() + str.right(str.size() - 1);
}