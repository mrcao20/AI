#ifndef _QT_GBK_H
#define _QT_GBK_H
#include <QString>
#include <QTextCodec>
#include <string>
using std::string;
class GBK
{
public:
	// QString(Unicode) -> std::string (GBK)
	static string FromUnicode(const QString& qstr)
	{
		QTextCodec* pCodec = QTextCodec::codecForName("gb2312");
		if (!pCodec) return "";
		QByteArray arr = pCodec->fromUnicode(qstr);
		string cstr = arr.data();
		return cstr;
	}
	// std::string (GBK) -> QString(Unicode)
	static QString ToUnicode(const string& cstr)
	{
		QTextCodec* pCodec = QTextCodec::codecForName("gb2312");
		if (!pCodec) return "";
		/* ע�� ���pCodecΪNULL, ��˵�����Qt��װλ�ò���Ĭ��λ�ã�
		������������Qtƪ�ġ�����������ܡ�! */

		QString qstr = pCodec->toUnicode(cstr.c_str(), cstr.length());
		return qstr;
	}
	// QString(Unicode) -> std::string (GBK)
	static string w2a(const QString& qstr)
	{
		return FromUnicode(qstr);
	}

	// std::string (GBK) -> QString(Unicode)
	static QString a2w(const string& cstr)
	{
		return ToUnicode(cstr);
	}
};
#endif
