#include "OutputBox.h"
#include <qdebug.h>
#include <qregexp.h>
#include <qstringlist.h>
#include <qdebug.h>
#include <qbitmap.h>
#include <qpainter.h>
#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qregexp.h>
#include <qtextedit.h>
#include <qtimer.h>

OutputBox::OutputBox(QWidget *parent)
	: AbstractOutputBox(parent)
	, textEdit(new QTextEdit(this))
	, mbEnter(false)
	, mnTransparent(0)
	, maxWidth(300)
	, maxHeight(150)
	, wordHeight(24) {

	textEdit->setAlignment(Qt::AlignCenter);
	textEdit->setReadOnly(true);

	mpTimer = new QTimer(this);
	mpTimer->setInterval(50);
	connect(mpTimer, &QTimer::timeout, this, &OutputBox::OnTimer);
}

void OutputBox::enterEvent(QEvent *) {
	mbEnter = true;
	mnTransparent = 200;
	setStyleSheet(QString("color:black;background-color:rgba(255, 255, 255, %1);border:none;").arg(mnTransparent));
}

void OutputBox::leaveEvent(QEvent *) {
	mbEnter = false;
}

void OutputBox::OnTimer() {
	if (mbEnter) {
		return;
	}

	mnTransparent -= 3;
	if (mnTransparent > 40) {
		setStyleSheet(QString("color:black;background-color:rgba(255, 255, 255, %1);border:none;").arg(mnTransparent));
	}
	else {
		mpTimer->stop();
		setVisible(false);
	}
}

QSize OutputBox::getStringSize(QString &str) {
	QFontMetrics fm1(font());
	int width = fm1.width(str) + 10;
	int height;
	if ((width % maxWidth) == 0) {
		height = (width / maxWidth) * wordHeight;
	}
	else {
		height = (width / maxWidth + 1) * wordHeight;
	}
	width = width > maxWidth ? maxWidth : width;
	height = height > maxHeight ? maxHeight : height;
	return QSize(width, height);
}

QSize OutputBox::getEditBoxSize(const QString &strMessage) {
	QSize strSize(0, 0);
	QStringList strList = strMessage.split(QRegExp("\\n"));
	bool isMaxWidth = false;
	bool isMaxHeight = false;
	for (auto str : strList) {
		if (isMaxWidth && isMaxHeight) break;
		QSize tempSize = getStringSize(str);
		if (strSize.width() < maxWidth) {
			strSize.setWidth(strSize.width() > tempSize.width() ? strSize.width() : tempSize.width());
			if (strSize.width() >= maxWidth) isMaxWidth = true;
		}
		if (strSize.height() < maxHeight) {
			int height = strSize.height() + tempSize.height();
			if (height >= maxHeight) {
				height = maxHeight;
				isMaxHeight = true;
			}
			strSize.setHeight(height);
		}
	}
	return strSize;
}

void OutputBox::SetMesseage(const QString &strMessage, const QPoint *pPoint) {
	if (strMessage.isEmpty()) {
		return;
	}
	
	QSize strSize = getEditBoxSize(strMessage);
	
	if (mnTransparent >= 120) {
		strSize.setWidth(strSize.width() > size().width() ? strSize.width() : size().width());
		int height = strSize.height() + size().height();
		strSize.setHeight(height > maxHeight ? maxHeight : height);
	}

	setFixedSize(strSize);
	textEdit->setFixedSize(strSize);

	QBitmap bmp(this->size());
	bmp.fill();
	QPainter p(&bmp);
	p.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
	p.setPen(Qt::NoPen);
	p.setBrush(Qt::black);
	p.drawRoundedRect(bmp.rect(), 10, 10);
	setMask(bmp);
	
	if (mnTransparent >= 120) {
		textEdit->append(strMessage);
	}
	else {
		textEdit->setText(strMessage);
	}
	textEdit->moveCursor(QTextCursor::End);

	setVisible(true);
	mnTransparent = 200;
	setStyleSheet(QString("color:black;background-color:rgba(255, 255, 255, %1);border:none;").arg(mnTransparent));
	OutputBoxMove(m_mainWindowPoint, m_mainWindowRect);
	mpTimer->start();
}

QRect OutputBox::getScreenRect() {
	return QApplication::desktop()->screenGeometry();

	//QDesktopWidget* desktopWidget = QApplication::desktop();
	//获取可用桌面大小
	//QRect deskRect = desktopWidget->availableGeometry();
	//获取设备屏幕大小
	//QRect screenRect = desktopWidget->screenGeometry();

	/*g_nActScreenX = screenRect.width();
	g_nActScreenY = screenRect.height();*/
	//g_nActScreenX = deskRect.width();
	//g_nActScreenY = deskRect.height();
}

void OutputBox::clearEdit() {
	textEdit->setText("");
	setVisible(false);
}