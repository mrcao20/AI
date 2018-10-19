#pragma once

#include <QtCore/QObject>
#include <qtextstream.h>
#include <qmetaobject.h>

#define McOutputInstance (McOutputStream::instance())

class McOutputStream : public QObject {
	Q_OBJECT

public:
	static McOutputStream *instance();

signals:
	void message(const QString &msg);

private:
	McOutputStream();
	static McOutputStream * m_instance;

	friend class McOutput;

private:
	void flush(const QString &data) { if (!data.isEmpty()) emit message(data); }

};

#define MC_OUTPUT_ENUM(Class,Enum) \
inline McOutput operator<<(McOutput o, Class::Enum value) \
{ \
    int index = Class::staticMetaObject.indexOfEnumerator(#Enum); \
    o.nospace() << #Class << "::" << Class::staticMetaObject.enumerator(index).valueToKey(value); \
    return o.space(); \
}

class McOutput {
	struct Stream {
		Stream() : ts(&buffer, QIODevice::WriteOnly), ref(1), space(true) {}
		QTextStream ts;
		QString buffer;
		int ref;
		bool space;
	} *stream;
public:
	inline McOutput() : stream(new Stream()) {}
	inline McOutput(const McOutput &o) : stream(o.stream) { ++stream->ref; }
	inline McOutput &operator=(const McOutput &other) {
		if (this != &other) {
			McOutput copy(other);
			qSwap(stream, copy.stream);
		}
		return *this;
	}
	~McOutput();
	inline void swap(McOutput &other) Q_DECL_NOTHROW { qSwap(stream, other.stream); }

	void putUcs4(uint ucs4);

	inline McOutput &space() { stream->space = true; stream->ts << ' '; return *this; }
	inline McOutput &nospace() { stream->space = false; return *this; }
	inline McOutput &maybeSpace() { if (stream->space) stream->ts << ' '; return *this; }

	inline bool autoInsertSpaces() const { return stream->space; }
	inline void setAutoInsertSpaces(bool b) { stream->space = b; }

	inline McOutput &operator<<(QChar t) { putUcs4(t.unicode()); return maybeSpace(); }
	inline McOutput &operator<<(bool t) { stream->ts << (t ? "true" : "false"); return maybeSpace(); }
	inline McOutput &operator<<(char t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(signed short t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(unsigned short t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(char16_t t) { return *this << QChar(ushort(t)); }
	inline McOutput &operator<<(char32_t t) { putUcs4(t); return maybeSpace(); }
	inline McOutput &operator<<(signed int t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(unsigned int t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(signed long t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(unsigned long t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(qint64 t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(quint64 t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(float t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(double t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(const char* t) { stream->ts << QString::fromUtf8(t); return maybeSpace(); }
	inline McOutput &operator<<(const QString & t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(const QStringRef & t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(QStringView s) { stream->ts << s.toString(); return maybeSpace(); }
	inline McOutput &operator<<(QLatin1String t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(const QByteArray & t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(const void * t) { stream->ts << t; return maybeSpace(); }
	inline McOutput &operator<<(std::nullptr_t) { stream->ts << "(nullptr)"; return maybeSpace(); }

};

namespace McPrivate {

template <typename SequentialContainer>
inline McOutput printSequentialContainer(McOutput o, const char *which, const SequentialContainer &c){
	const bool oldSetting = o.autoInsertSpaces();
	o.nospace() << which << '(';
	typename SequentialContainer::const_iterator it = c.begin(), end = c.end();
	if (it != end) {
		o << *it;
		++it;
	}
	while (it != end) {
		o << ", " << *it;
		++it;
	}
	o << ')';
	o.setAutoInsertSpaces(oldSetting);
	return o.maybeSpace();
}

}

template <class T>
inline McOutput operator<<(McOutput o, const QList<T> &list){
	return McPrivate::printSequentialContainer(o, "QList", list);
}

template <typename T>
inline McOutput operator<<(McOutput o, const QVector<T> &vec){
	return McPrivate::printSequentialContainer(o, "QVector", vec);
}

template <typename T, typename Alloc>
inline McOutput operator<<(McOutput o, const std::vector<T, Alloc> &vec){
	return McPrivate::printSequentialContainer(o, "std::vector", vec);
}

template <typename T, typename Alloc>
inline McOutput operator<<(McOutput o, const std::list<T, Alloc> &vec){
	return McPrivate::printSequentialContainer(o, "std::list", vec);
}

template <typename Key, typename T, typename Compare, typename Alloc>
inline McOutput operator<<(McOutput o, const std::map<Key, T, Compare, Alloc> &map){
	return McPrivate::printSequentialContainer(o, "std::map", map); // yes, sequential: *it is std::pair
}

template <typename Key, typename T, typename Compare, typename Alloc>
inline McOutput operator<<(McOutput o, const std::multimap<Key, T, Compare, Alloc> &map){
	return McPrivate::printSequentialContainer(o, "std::multimap", map); // yes, sequential: *it is std::pair
}

template <class Key, class T>
inline McOutput operator<<(McOutput o, const QMap<Key, T> &map){
	const bool oldSetting = o.autoInsertSpaces();
	o.nospace() << "QMap(";
	for (typename QMap<Key, T>::const_iterator it = map.constBegin();
		it != map.constEnd(); ++it) {
		o << '(' << it.key() << ", " << it.value() << ')';
	}
	o << ')';
	o.setAutoInsertSpaces(oldSetting);
	return o.maybeSpace();
}

template <class Key, class T>
inline McOutput operator<<(McOutput o, const QHash<Key, T> &hash){
	const bool oldSetting = o.autoInsertSpaces();
	o.nospace() << "QHash(";
	for (typename QHash<Key, T>::const_iterator it = hash.constBegin();
		it != hash.constEnd(); ++it)
		o << '(' << it.key() << ", " << it.value() << ')';
	o << ')';
	o.setAutoInsertSpaces(oldSetting);
	return o.maybeSpace();
}

template <class T1, class T2>
inline McOutput operator<<(McOutput o, const QPair<T1, T2> &pair){
	const bool oldSetting = o.autoInsertSpaces();
	o.nospace() << "QPair(" << pair.first << ',' << pair.second << ')';
	o.setAutoInsertSpaces(oldSetting);
	return o.maybeSpace();
}

template <class T1, class T2>
inline McOutput operator<<(McOutput o, const std::pair<T1, T2> &pair){
	const bool oldSetting = o.autoInsertSpaces();
	o.nospace() << "std::pair(" << pair.first << ',' << pair.second << ')';
	o.setAutoInsertSpaces(oldSetting);
	return o.maybeSpace();
}

template <typename T>
inline McOutput operator<<(McOutput o, const QSet<T> &set){
	return McPrivate::printSequentialContainer(o, "QSet", set);
}

template <class T>
inline McOutput operator<<(McOutput o, const QSharedPointer<T> &ptr){
	o.nospace() << "QSharedPointer(" << ptr.data() << ")";
	return o;
}
