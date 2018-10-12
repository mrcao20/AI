#include "GraphicalProcesses.h"
#include "ui_GraphicalProcesses.h"
#include <qfile.h>
#include <qtextstream.h>
#include <qevent.h>
#include <qvector.h>
#include <qfiledialog.h>
#include "Widget.h"
#include "Output.h"

struct GraphicalProcessesData : public Ui::GraphicalProcessesClass{
	QVector<Widget *> m_widgets;
	QStringList m_imageFormats;
};

GraphicalProcesses::GraphicalProcesses(QWidget *parent)
	: QWidget(parent)
	, d(new GraphicalProcessesData)
{
	d->setupUi(this);
}

void GraphicalProcesses::init() {
	d->drawingTool->init();
	d->m_widgets.append(d->bgTransparent);
	d->m_widgets.append(d->drawingTool);

	connect(d->pb_reset, &QPushButton::clicked, [this]() {
		int currIndex = d->tabWidget->currentIndex();
		Widget *w = NULL;
		if(currIndex < d->m_widgets.size() && currIndex >=0)
			d->m_widgets.at(currIndex);
		if(w)
			w->reset();
	});
	connect(d->pb_file, &QPushButton::clicked, [this]() {
		Widget *w = qobject_cast<Widget *>(d->tabWidget->currentWidget());
		if (!w)
			return;
		QString appPath;
#ifdef _DEBUG
		appPath = qApp->applicationDirPath() + "/../Release";
#else
		appPath = qApp->applicationDirPath();
#endif // _DEBUG
		QString path = appPath + "/images/Mode/0.png";
		QString filter;
		for (const QString &format : d->m_imageFormats) {
			filter += QString("Images (*.%1);;").arg(format);
		}
		QList<QUrl> urls = QFileDialog::getOpenFileUrls(this, "Load Image", QUrl::fromLocalFile(path), filter);
		if (urls.isEmpty())
			return;
		w->loadImage(urls);
	});

	QString appPath;
#ifdef _DEBUG
	appPath = qApp->applicationDirPath() + "/../Release";
#else
	appPath = qApp->applicationDirPath();
#endif // _DEBUG
	QFile file(appPath + "/ImageFormat");
	file.open(QFile::ReadOnly | QFile::Text);
	QTextStream stream(&file);
	QString format;
	format.reserve(10);
	while (stream.readLineInto(&format)) {
		d->m_imageFormats.append(format);
	}
	file.close();
	
	for (Widget *w : d->m_widgets) {
		w->init(d->m_imageFormats);
	}
}

void GraphicalProcesses::closeEvent(QCloseEvent *event) {
	event->accept();
	emit quit();
}

GraphicalProcesses::~GraphicalProcesses() {
	output << "图形处理工具关闭";
}
