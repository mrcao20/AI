#include "McOutput.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>

McOutputStream::McOutputStream() : QObject(qApp) {
}

McOutputStream * McOutputStream::m_instance = 0;

McOutputStream * McOutputStream::instance()
{
	static QMutex mutex;
	if (!m_instance) {
		QMutexLocker locker(&mutex);
		if (!m_instance)
			m_instance = new McOutputStream;
	}
	
	return m_instance;
}

McOutput::~McOutput() {
	if (!--stream->ref) {
		if (stream->space && stream->buffer.endsWith(QLatin1Char(' ')))
			stream->buffer.chop(1);
		if (!stream->buffer.isEmpty())
			McOutputInstance->flush(stream->buffer);
		delete stream;
	}
}

void McOutput::putUcs4(uint ucs4) {
	if (ucs4 < 0x20) {
		stream->ts << "\\x" << hex << ucs4 << reset;
	}
	else if (ucs4 < 0x80) {
		stream->ts << char(ucs4);
	}
	else {
		if (ucs4 < 0x10000)
			stream->ts << "\\u" << qSetFieldWidth(4);
		else
			stream->ts << "\\U" << qSetFieldWidth(8);
		stream->ts << hex << qSetPadChar(QLatin1Char('0')) << ucs4 << reset;
	}
}
