#include "StrToolsBox.h"
#include <qtextlayout.h>
#include <QTextBlock>
#include <qevent.h>
#include <qregularexpression.h>
#include <qvector.h>
#include <qdebug.h>
#include "Output.h"

struct StrToolsBoxDataPrivate{
	StrToolsBox::EditMode m_editMode;
	StrToolsBox::ProcessMode m_processMode;
	bool m_isPressCtrl;
};

StrToolsBox::StrToolsBox(QWidget *parent) :
	InputTextEdit(parent),
	d(new StrToolsBoxDataPrivate) {

	d->m_isPressCtrl = false;

	init();

	resize(600, 400);
	entryCommandMode();

	setFocus();
}

StrToolsBox::~StrToolsBox(){
}

void StrToolsBox::init() {
	QFont font = this->font();
	font.setPixelSize(15);
	setFont(font);
	setCurrentCharFormat(QTextCharFormat());
}

void StrToolsBox::setText(const QString &text) {
	InputTextEdit::setPlainText(text);
	moveCursor(QTextCursor::End);
}

void StrToolsBox::setTitle(const QString &title) {
	setWindowTitle("StrTools: " + title);
}

void StrToolsBox::entryCommandMode() {
	init();
	setTitle("command");
	d->m_editMode = Command;
	initCommandModeText();
}

bool StrToolsBox::entryInputMode(const QString &mode) {
	bool isEntrySuccess = false;
	if (mode == "strcmp") {
		clear();
		setTitle(mode);
		d->m_editMode = Input;
		d->m_processMode = StrCmp;
		isEntrySuccess = true;
	}
	return isEntrySuccess;
}

inline void StrToolsBox::initCommandModeText() {
	setText(":");
}

int StrToolsBox::getCursorPos() {
	QTextCursor tc = textCursor();
	//当前光标在本BLOCK内的相对位置
	int nCurpos = tc.position() - tc.block().position();
	return nCurpos;
}

int StrToolsBox::getLineNumber() {
	QTextCursor tc = textCursor();
	QTextLayout *pLayout = tc.block().layout();
	int nCurpos = getCursorPos();
	int nTextline = pLayout->lineForTextPosition(nCurpos).lineNumber() + tc.block().firstLineNumber();
	return nTextline;
}

void StrToolsBox::handleInput() {
	switch (d->m_processMode)
	{
	case StrCmp:
		my_strcmp();
		break;
	default:
		break;
	}
}

QString StrToolsBox::markStr(const QString &str) {
	QString rs;
	rs = QString(R"(<span style='color:#FF4500;'>%1</span>)").arg(str);
	return rs;
}

void StrToolsBox::my_strcmp() {
	if (!checkInput()) {
		return;
	}
	QString text = toPlainText();
	QStringList strList = text.split(QRegularExpression(R"(\n+)"), QString::SkipEmptyParts);
	QString firstStr = strList.at(0);
	QString secondStr = strList.at(1);
	int len = firstStr.size() < secondStr.size() ? firstStr.size() : secondStr.size();
	QVector<int> differentIndexs;
	for (int i = 0; i < len; i++) {
		if (firstStr.at(i) != secondStr.at(i)) {
			differentIndexs.append(i);
		}
	}
	int startIndex = 0;
	for (int k = 0; k < 2; k++) {
		if (k == 1) {
			startIndex = firstStr.size() + 2;
		}
		for (int index : differentIndexs) {
			QTextCursor cur = textCursor();
			cur.setPosition(index + startIndex, QTextCursor::MoveAnchor);
			cur.setPosition(index + 1 + startIndex, QTextCursor::KeepAnchor);
			//cur.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor, 1);//移动结束位置
			//cur.select(QTextCursor::WordUnderCursor);//选择
			setTextCursor(cur);
			QTextCharFormat fmt;
			fmt.setForeground(Qt::red);
			mergeCurrentCharFormat(fmt);
		}
	}
	moveCursor(QTextCursor::End);
}

bool StrToolsBox::checkInput() {
	QString text = toPlainText();
	QStringList strList = text.split(QRegularExpression(R"(\n+)"), QString::SkipEmptyParts);
	if (strList.size() > 2) {
		QStringList tempList = text.split(QRegularExpression(R"(\n\n)"), QString::SkipEmptyParts);
		for (int i = 0; i < tempList.size(); i++) {
			QStringList tmptmpList = tempList.at(i).split(QRegularExpression(R"(\n)"), QString::SkipEmptyParts);
			QString rs = "";
			for (int j = 0; j < tmptmpList.size(); j++) {
				rs += tmptmpList.at(j);
			}
			tempList.replace(i, rs);
		}
		strList.swap(tempList);
	}
	if (strList.size() != 2) {
		output << QString("input error, size: %1").arg(strList.size());
		return false;
	}
	setText(strList.at(0) + "\n\n" + strList.at(1));
	return true;
}

void StrToolsBox::keyPressEvent(QKeyEvent *event) {
	int key = event->key();
	if (key == Qt::Key_Backspace) {
		if (d->m_editMode == Command) {
			int cursorPos = getCursorPos();
			int lineNum = getLineNumber();
			if (lineNum == 0 && cursorPos == 1)
				return;
		}
	}
	else if (key == Qt::Key_Return || key == Qt::Key_Enter) {
		if (d->m_editMode == Command) {
			QString text = toPlainText();
			setText(text.left(1));
			if (entryInputMode(text.right(text.size() - 1))) {
				return;
			}
		}
		else if (d->m_editMode == Input) {
			if (d->m_isPressCtrl) {
				handleInput();
			}
		}
	}
	else if (key == Qt::Key_Q) {
		if (d->m_isPressCtrl) {
			entryCommandMode();
		}
	}
	else if (key == Qt::Key_Control) {
		d->m_isPressCtrl = true;
	}
	InputTextEdit::keyPressEvent(event);
}

void StrToolsBox::keyReleaseEvent(QKeyEvent *event) {
	int key = event->key();
	if (key == Qt::Key_Control) {
		d->m_isPressCtrl = false;
	}
	else if (key == Qt::Key_Backspace) {
		if (d->m_editMode == Command && toPlainText().isEmpty()) {
			initCommandModeText();
		}
	}
	InputTextEdit::keyReleaseEvent(event);
}
