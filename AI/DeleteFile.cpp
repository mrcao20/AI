#include <DeleteFile.h>
#include <qdir.h>
#include <qregexp.h>

DeleteFile::DeleteFile(QObject * parent)
	: QObject(parent) 
	, filePath("")
	, filterString(""){
}

void DeleteFile::process() {
	DelDir(filePath);
	emit finished();
}

bool DeleteFile::DelDir(const QString &path) {
	if (path.isEmpty()) {
		return false;
	}
	QDir dir(path);
	if (!dir.exists()) {
		return true;
	}
	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //���ù���  
	QFileInfoList fileList = dir.entryInfoList(); // ��ȡ���е��ļ���Ϣ  
	foreach(QFileInfo file, fileList) { //�����ļ���Ϣ  
		if (!filterString.isEmpty() && file.fileName().contains(QRegExp(filterString))) continue;
		if (file.isFile()) { // ���ļ���ɾ��  
			file.dir().remove(file.fileName());
		}
		else { // �ݹ�ɾ��  
			DelDir(file.absoluteFilePath());
		}
	}
	return dir.rmpath(dir.absolutePath()); // ɾ���ļ���  
}

void DeleteFile::setFilePath(QString &filePath) {
	this->filePath = filePath;
}

void DeleteFile::setFilterString(QString &filterString) {
	this->filterString = filterString;
}