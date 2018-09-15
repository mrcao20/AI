#pragma once
#pragma execution_character_set("utf-8")
#include "InputTextEdit.h"
#include <qscopedpointer.h>

QT_BEGIN_NAMESPACE
class QKeyEvent;
QT_END_NAMESPACE

struct StrToolsBoxDataPrivate;

class StrToolsBox : public InputTextEdit{
	Q_OBJECT

public:
	explicit StrToolsBox(QWidget *parent = 0);
	virtual ~StrToolsBox();

	enum EditMode {
		Command,
		Input
	};

	enum ProcessMode{
		StrCmp
	};

protected:
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;

private:
	void init();
	void setText(const QString &text);
	void setTitle(const QString &title = "");
	void entryCommandMode();
	bool entryInputMode(const QString &mode);
	void initCommandModeText();
	int getCursorPos();
	int getLineNumber();
	void handleInput();
	void my_strcmp();
	bool checkInput();
	QString markStr(const QString &str);

private:
	QScopedPointer<StrToolsBoxDataPrivate> d;
};

