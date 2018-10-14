#include "McDrawingTools.h"
#include <qcolordialog.h>
#include <qevent.h>
#include "ui_DrawingTools.h"
#include "McGlobal.h"

#include "qdebug.h"

struct DrawingToolsData : public Ui::DrawingTools {
	
};

McDrawingTools::McDrawingTools(QWidget *parent)
	: Widget(parent)
	, d(new DrawingToolsData)
{
	d->setupUi(this);

	setAcceptDrops(false);

	QButtonGroup *buttonGroup = new QButtonGroup(d->groupBox);
	buttonGroup->addButton(d->pb_drawRect, Mc::Rect);
	buttonGroup->addButton(d->pb_drawPolygon, Mc::Polygon);
	buttonGroup->addButton(d->pb_drawEllipse, Mc::Ellipse);
	buttonGroup->addButton(d->pb_drawLine, Mc::Line);
	buttonGroup->addButton(d->pb_drawWithPen, Mc::Pen);

	connect(d->pb_undo, &QPushButton::clicked, [this]() {
		d->f_canvas->undo();
	});
	connect(d->pb_redo, &QPushButton::clicked, [this]() {
		d->f_canvas->redo();
	});
	connect(d->pb_reset, &QPushButton::clicked, [this]() {
		d->d_lineWidth->setValue(1);
		d->d_alpha->setValue(0);
		d->f_canvas->reset();
		setWidgetPalette(d->pb_lineColor, QPalette::Button, Qt::black);
		setWidgetPalette(d->pb_brushColor, QPalette::Button, Qt::transparent);
	});

	connect(d->d_lineWidth, &QDial::valueChanged, [this](int i) {
		d->l_lineWidth->setText("线宽: " + QString::number(i));
		if (d->f_canvas)
			d->f_canvas->setLineWidth(i);
	});
	connect(d->d_alpha, &QDial::valueChanged, [this](int i) {
		d->l_alpha->setText("alpha值: " + QString::number(i));
		if (d->f_canvas)
			d->f_canvas->setAlpha(i);
	});

	connect(d->cb_antialiasing, &QCheckBox::clicked, d->f_canvas, &McCanvas::setAntialiasing);

	connect(buttonGroup, qOverload<int>(&QButtonGroup::buttonClicked), [this](int id) {
		if (d->f_canvas)
			d->f_canvas->setShapeType(id);
	});

	connect(d->pb_lineColor, &QPushButton::clicked, [this]() {
		QColor color = QColorDialog::getColor(Qt::white, this, "线条颜色");
		if (!color.isValid())
			return;
		setWidgetPalette(d->pb_lineColor, QPalette::Button, color);
		d->f_canvas->setLineColor(color);
	});
	connect(d->pb_brushColor, &QPushButton::clicked, [this]() {
		QColor color = QColorDialog::getColor(Qt::white, this, "填充颜色");
		if (!color.isValid())
			return;
		setWidgetPalette(d->pb_brushColor, QPalette::Button, color);
		d->f_canvas->setBrushColor(color);
	});

	connect(d->f_canvas, &McCanvas::hasImage, [this](const QImage &image) {
		if (m_images.isEmpty())
			m_images.append(image);
		else
			m_images.replace(0, image);
	});
}

void McDrawingTools::init() {
	d->f_canvas->init();
	d->pb_drawRect->click();
	d->f_canvas->setLineWidth(1);
	d->f_canvas->setAlpha(0);
	d->f_canvas->setAntialiasing(d->cb_antialiasing->isChecked());
	d->pb_lineColor->setAutoFillBackground(true);
	d->pb_brushColor->setAutoFillBackground(true);

	setWidgetPalette(d->pb_lineColor, QPalette::Button, Qt::black);
	setWidgetPalette(d->pb_brushColor, QPalette::Button, Qt::transparent);
}

void McDrawingTools::reset() {
	d->pb_drawRect->click();
	d->pb_reset->click();
}

bool McDrawingTools::hasInputImage(const QList<QImage> &images) {
	
	return true;
}

bool McDrawingTools::beingSaved() {
	
	return true;
}

bool McDrawingTools::saveCompleted() {
	
	return true;
}

bool McDrawingTools::saveCompleted(int index) {
	
	return true;
}

void McDrawingTools::setWidgetPalette(QWidget *w, QPalette::ColorRole acr, const QColor &acolor) {
	QPalette pal = w->palette();
	pal.setColor(acr, acolor);
	w->setPalette(pal);
}
