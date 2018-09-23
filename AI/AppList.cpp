#include "AppList.h"
#include <qfileiconprovider.h>
#include <qdir.h>
#include <qevent.h>
#include <qprocess.h>
#include <qaction.h>
#include <qlist.h>
#include <qdesktopwidget.h>
#include <qmimedata.h>
#include <qmessagebox.h>
#include "Output.h"

struct AppListData {
	QString m_shortcutDir;
	QString m_shortcutPos;
	bool m_isCreateShortcut;
};

AppList::AppList(QWidget *parent)
	: QWidget(parent)
	, d(new AppListData)
	, menu(new QMenu(this)){
	
	ui.setupUi(this);

	d->m_isCreateShortcut = false;
	d->m_shortcutDir = qApp->applicationDirPath();		// 获得文件夹结尾不会有/或\\，需手动加入
#ifdef _DEBUG
	d->m_shortcutDir += "/../Release/AppShortcut";
#else
	d->m_shortcutDir += "/AppShortcut";
#endif // _DEBUG

	connect(ui.AppTableWidget, &QTableWidget::customContextMenuRequested, this, &AppList::handleContextMenuRequested);
	connect(ui.AppTableWidget, &QTableWidget::itemDoubleClicked, this, &AppList::openApp);

	loadApp(d->m_shortcutDir);
	resizeTableWidget();
	addApp();

	QAction *open_action = menu->addAction("open");
	connect(open_action, &QAction::triggered, [this]() {
		auto itemList = ui.AppTableWidget->selectedItems();
		for (QTableWidgetItem *item : itemList) {
			openApp(item);
		}
	});
	menu->addSeparator();
	QAction *delete_action = menu->addAction("delete");
	connect(delete_action, &QAction::triggered, [this]() {
		auto itemList = ui.AppTableWidget->selectedItems();
		if (QMessageBox::information(NULL, "delete", QString("delete %1 app?").arg(itemList.size()), 
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
			return;
		for (QTableWidgetItem *item : itemList) {
			deleteFile(item);
			deleteItem(item);
		}
		reloadApp();
	});

	QRect rect = QApplication::desktop()->screenGeometry();
	move(rect.width() / 2 - width() / 2, rect.height() / 2 - height() / 2);

	setAcceptDrops(true);

	show();
}

void AppList::loadApp(const QString &dirPath) {
	QDir dir(dirPath);
	QFileInfoList fileInfoList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::DirsLast | QDir::Time);
	
	QFileIconProvider fileIconPro;
	for (int i = 0; i < fileInfoList.size(); i++) {
		QFileInfo fileInfo = fileInfoList.at(i);
		if (fileInfo.isDir()) {
			loadApp(fileInfo.absoluteFilePath());
			continue;
		}
		QString filePath = fileInfo.absoluteFilePath();
		QIcon icon = fileIconPro.icon(QFileInfo(getTargetFile(filePath)));
		QTableWidgetItem *item = new QTableWidgetItem(icon, fileInfo.baseName());
		items.insert(filePath, item);
	}
}

void AppList::reloadApp() {
	QMap<QString, QTableWidgetItem *> tempItems;
	for (auto iter = items.begin(); iter != items.end(); iter++) {
		tempItems.insert(iter.key(), iter.value()->clone());
	}
	items.swap(tempItems);
	deleteItems(tempItems);
	ui.AppTableWidget->clear();

	resizeTableWidget();
	addApp();
}

void AppList::addApp() {
	int row = 0, col = 0;
	for (auto iter = items.begin(); iter != items.end(); iter++) {
		ui.AppTableWidget->setItem(row, col, iter.value());
		if (++col >= ui.AppTableWidget->columnCount()) {
			col = 0;
			row++;
		}
	}
}

void AppList::resizeTableWidget() {
	int newColCount = ui.AppTableWidget->size().width() / 110;
	int newRowCount = ceil(items.size() / (newColCount * 1.0));
	ui.AppTableWidget->setColumnCount(newColCount);
	ui.AppTableWidget->setRowCount(newRowCount);
}

QString AppList::getTargetFile(const QString &filePath) {
	QFileInfo fileInfo(filePath);
	if (fileInfo.isSymLink())
		return fileInfo.symLinkTarget();
	else
		return fileInfo.absoluteFilePath();
}

void AppList::deleteFile(QTableWidgetItem *item) {
	QString filePath = items.key(item);
	if (!QFile::exists(filePath))
		return;
	QFileInfo fileInfo(filePath);
	QString appName = fileInfo.baseName();
	if (QFile::remove(filePath))
		output << QString("%1: delete successful").arg(appName);
	else
		output << QString("%1: delete failed").arg(appName);
}

void AppList::deleteItem(QTableWidgetItem *item) {
	items.remove(items.key(item));
	delete item;
}

void AppList::resizeEvent(QResizeEvent *event) {
	ui.AppTableWidget->resize(size());
	
	if (ui.AppTableWidget->columnCount() == ui.AppTableWidget->size().width() / 110) {
		event->ignore();
		return;
	}

	reloadApp();
	
	event->accept();
}

void AppList::handleContextMenuRequested(const QPoint &pos) {
	if (ui.AppTableWidget->selectedItems().isEmpty())
		return;
	menu->exec(QCursor::pos());
}

void AppList::openApp(QTableWidgetItem *item) {
	QString filePath = items.key(item);
	QFileInfo fileInfo(filePath);
	QString appName = fileInfo.baseName();
	if(QProcess::startDetached(getTargetFile(filePath), QStringList()))
		output << QString("%1: open successful").arg(appName);
	else
		output << QString("%1: open failed").arg(appName);
}

void AppList::deleteItems(QMap<QString, QTableWidgetItem *> &items) {
	for (auto iter = items.begin(); iter != items.end(); iter++)
		delete iter.value();
}

void AppList::closeEvent(QCloseEvent *event) {
	emit quit();
}

void AppList::dragEnterEvent(QDragEnterEvent *event) {
	if (!d->m_isCreateShortcut)
		return;
	//如果为文件，则支持拖放
	if (event->mimeData()->hasFormat("text/uri-list"))
		event->acceptProposedAction();
}

void AppList::dropEvent(QDropEvent *event) {
	//注意：这里如果有多文件存在，意思是用户一下子拖动了多个文件，而不是拖动一个目录
	//如果想读取整个目录，则在不同的操作平台下，自己编写函数实现读取整个目录文件名
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty())
		return;

	//逐个添加链接文件
	foreach(QUrl url, urls) {
		QString file_name = url.toLocalFile();
		createShortcut(file_name);
	}
	deleteItems(items);
	items.clear();
	loadApp(d->m_shortcutDir);
	addApp();
}

void AppList::createShortcut(const QString &filePath) {
	if (QFileInfo(filePath).isDir()) {
		QDir dir(filePath);
		QStringList fileList = dir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::LocaleAware);
		for (const QString &file : fileList) {
			createShortcut(file);
		}
	}
	QString tempDir = d->m_shortcutDir;
	if (d->m_shortcutPos == "in") {
		tempDir += "/others";
	}
	QFile file(filePath);
	QFileInfo fileInfo(file);
	QString baseName = fileInfo.baseName();
	QString newFile = QString("%1/%2.lnk").arg(tempDir, baseName);
	if (fileInfo.isSymLink()) {
		file.copy(newFile);
	}
	else {
		file.link(newFile);
	}
}

void AppList::allowCreateShortcut(const QString &pos) {
	d->m_shortcutPos = pos;
	d->m_isCreateShortcut = true;
}

AppList::~AppList() {
	deleteItems(items);
}