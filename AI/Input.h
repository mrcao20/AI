#pragma once
#pragma execution_character_set("utf-8")
#include <qmainwindow.h>
#include <qtextedit.h>
#include <qevent.h>
#include <qtimer.h>
#include <qprocess.h>
#include <qscopedpointer.h>

struct InputDataPrivate;

class Input : public QWidget {
	Q_OBJECT

public:
	explicit Input(QWidget * = Q_NULLPTR);
	~Input();

	void displayInput();

protected:

signals :
	void quit();
	void entryCmdMode();
	void entryNormalMode();
	void hideOutputBox();
	void showOutputBox();
	void showHistory();
	void changeCharacterState(const QString &);
	void lockScreen();
	void unlockScreen(const QString &);
	void exitApplication(const QString &);
	void setHook();
	void unsetHook(const QString &);

private slots :
	void handleInput(const QString &);
	void completeCommand(const QString &);

private:
	QScopedPointer<InputDataPrivate> d;

private:
	void initCommand();

	QStringList &getCmdList(const QStringList &);

	bool isExistCmd(const QString &);
	QStringList openCmdFile(const QString &);
	void addCmd(const QString &);
	bool isContainsCmd(const QString &);
	void createTimeoutTimer(const QString &);
	void startTimeoutTimer(const QString &);

	void runCmd(const QString &, QStringList &);

	QString firstCharToUpper(const QString &);

	void handleMessage(const QString &message);
	void handleCommand(QString &);
	void hideInputTextEdit();

};