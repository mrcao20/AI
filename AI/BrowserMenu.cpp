#include "BrowserMenu.h"
#include "Output.h"
#include <qfileinfo.h>

BrowserMenu::BrowserMenu(QMenu *parent)
	: QMenu(parent)
	, browser_action(new QAction(QString("browser"), this))
	, clearCache_action(new QAction("clear_cache", this))
	, clearAll_action(new QAction("clear_all", this))
	, thread(nullptr)
	, browser_process(nullptr)
	, deleteFile(nullptr){

	this->setTitle("browser");
	this->addAction(browser_action);
	this->addSeparator();
	this->addAction(clearCache_action);
	this->addAction(clearAll_action);
	connect(browser_action, &QAction::triggered, this, &BrowserMenu::startBrowserProcess);
	connect(clearCache_action, &QAction::triggered, [this]() {
		createThread();
		if (!isDeleteFile()) return;
		output << QString("ÕýÔÚÇå³ý»º´æ");
		createDelete(QString("C:/Users/Administrator/AppData/Local/MyBrowser/"), QString("Cookies"));
		this->thread->start();
	});
	connect(clearAll_action, &QAction::triggered, [this]() {
		createThread();
		if (!isDeleteFile()) return;
		output << QString("ÕýÔÚÇå³ýËùÓÐÊý¾Ý");
		createDelete(QString("C:/Users/Administrator/AppData/Local/MyBrowser/"), "");
		this->thread->start();
	});
}

BrowserMenu::~BrowserMenu(){
	if (deleteFile != nullptr) {
		delete deleteFile;
	}
}

void BrowserMenu::createThread() {
	if (this->thread == nullptr)
		this->thread = new QThread(this);
}

bool BrowserMenu::isDeleteFile() {
	if (deleteFile != nullptr) {
		output << QString("ÉÏÒ»¸öÉ¾³ýÖ¸ÁîÎªÍê³É");
		return false;
	}
	return true;
}

void BrowserMenu::createDelete(QString filePath, QString filterString) {
	deleteFile = new DeleteFile();
	deleteFile->moveToThread(thread);
	connect(thread, SIGNAL(started()), deleteFile, SLOT(process()));
	connect(deleteFile, &DeleteFile::finished, [this]() {
		this->thread->quit();
		delete deleteFile;
		deleteFile = nullptr;
		output << QString("Çå³ýÍê±Ï");
	});
	deleteFile->setFilePath(filePath);
	deleteFile->setFilterString(filterString);
}

void BrowserMenu::startBrowserProcess() {
	if (browser_process != nullptr) return;
	browser_process = new QProcess();
	connect(browser_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [this](int exitCode, QProcess::ExitStatus exitStatus) {
		browser_process->close();
		delete browser_process;
		browser_process = nullptr;
		output << QString("ä¯ÀÀÆ÷¹Ø±Õ");
	});
	connect(browser_process, &QProcess::readyReadStandardOutput, [this]() {
		QString str = browser_process->readAllStandardOutput();
		output << str;
	});
	connect(browser_process, &QProcess::errorOccurred, [this](QProcess::ProcessError error) {
		QString str = "";
		switch (error)
		{
		case QProcess::FailedToStart:
			str = "ä¯ÀÀÆ÷Æô¶¯Ê§°Ü";
			break;
		case QProcess::Crashed:
			str = "ä¯ÀÀÆ÷Òì³£Ïú»Ù";
			break;
		case QProcess::Timedout:
			str = "ä¯ÀÀÆ÷ÏìÓ¦³¬Ê±";
			break;
		case QProcess::ReadError:
			str = "ä¯ÀÀÆ÷¶Á³ö´íÎó";
			break;
		case QProcess::WriteError:
			str = "ä¯ÀÀÆ÷Ð´Èë´íÎó";
			break;
		case QProcess::UnknownError:
			str = "ä¯ÀÀÆ÷Î´Öª´íÎó";
			break;
		default:
			break;
		}
		output << str;
	});
	QFileInfo fileInfo("./MyBrowser.lnk");
	QString fileName = fileInfo.symLinkTarget();
	browser_process->start(fileName);
}