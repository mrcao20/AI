#include "OutputBigBox.h"
#include <qtextedit.h>
#include <qbitmap.h>
#include <qpainter.h>

struct OutputBoxNormalDataPrivate {
	QTextEdit *m_textEdit;
};

OutputBigBox::OutputBigBox(QWidget *parent)
	: AbstractOutputBox(parent) 
	, d(new OutputBoxNormalDataPrivate){

	setWindowOpacity(0.72);
	resize(600, 300);

	d->m_textEdit = new QTextEdit(this);
	d->m_textEdit->resize(size());
	d->m_textEdit->setReadOnly(true);
	d->m_textEdit->setStyleSheet("font-size:15px;font-family:ÐÂËÎÌå;color:rgb(0, 255, 0);background-color:black;border:none;");

	QBitmap bmp(this->size());
	bmp.fill();
	QPainter p(&bmp);
	p.setRenderHint(QPainter::Antialiasing);  // ·´¾â³Ý;
	p.setPen(Qt::NoPen);
	p.setBrush(Qt::black);
	p.drawRoundedRect(bmp.rect(), 10, 10);
	setMask(bmp);

	show();
}

void OutputBigBox::SetMesseage(const QString &strMessage, const QPoint *pPoint) {
	d->m_textEdit->append(strMessage);
	d->m_textEdit->moveCursor(QTextCursor::End);
}

OutputBigBox::~OutputBigBox() {

}