#include "McBgTransparent.h"
#include "ui_BgTransparent.h"
#include <qcolordialog.h>
#include <QtConcurrent>
#include <qevent.h>
#include "McGlobal.h"

struct McBgTransparentData : public Ui::BgTransparent {
	Qt::TransformationMode m_transformMode;
	bool m_canManualProcess;
	QString m_imgCurrState;
	McBgTransparent::ProcState m_procCurrState;
	McBgTransparent::MouseState m_mouseState;
	QImage m_currImage;
	QImage m_alphaImage;
	QImage m_smoothImage;
	QImage m_transparentImage;
};

McBgTransparent::McBgTransparent(QWidget *parent)
	: Widget(parent)
	, d(new McBgTransparentData)
{
	d->setupUi(this);

	m_currentIndex = 0;
	d->m_transformMode = Qt::FastTransformation;
	d->m_mouseState = McBgTransparent::Normal;
	d->m_canManualProcess = true;

	d->imageLabel->installEventFilter(this);
}

McBgTransparent::~McBgTransparent(){
}

void McBgTransparent::init() {
	connect(d->transformCbB, &QComboBox::currentTextChanged, [this](const QString &text) {
		if (!canManualProcess())
			return;
		if (text == "���ٱ任") {
			d->m_transformMode = Qt::FastTransformation;
		}
		else if (text == "ƽ���任") {
			d->m_transformMode = Qt::SmoothTransformation;
		}
		if (!d->imageLabel->pixmap()) {
			return;
		}
		loadImage(m_currentIndex);
	});
	connect(d->pb_previousImage, &QPushButton::clicked, [this]() {
		if (!canManualProcess())
			return;
		m_currentIndex = m_currentIndex == 0 ? m_images.size() - 1 : m_currentIndex - 1;
		loadImage(m_currentIndex);
	});
	connect(d->pb_nextImage, &QPushButton::clicked, [this]() {
		if (!canManualProcess())
			return;
		showNext();
	});
	connect(d->pb_nextProc, &QPushButton::clicked, [this]() {
		if (!canManualProcess())
			return;
		nextProc();
	});
	connect(d->pb_procing, &QPushButton::clicked, [this]() {
		if (!canManualProcess())
			return;
		procing();
	});
	connect(d->pb_bgColor, &QPushButton::clicked, [this]() {
		if (!canManualProcess())
			return;
		QColor c = QColorDialog::getColor();
		if (!c.isValid())
			return;
		d->f_bgColor->setPalette(QPalette(c));
		d->sb_bgGray->setValue(qGray(c.rgb()));
		process(McBgTransparent::CreateAlpha);
	});
	connect(d->sb_colorD_value, qOverload<int>(&QSpinBox::valueChanged), [this](int i) {
		if (!canManualProcess())
			return;
		process(McBgTransparent::CreateAlpha);
	});
	connect(d->sb_bgGray, qOverload<int>(&QSpinBox::valueChanged), [this](int i) {
		if (!canManualProcess())
			return;
		QPalette p = d->f_bgColor->palette();
		if (qGray(p.color(QPalette::Background).rgb()) != i) {
			d->f_bgColor->setPalette(QPalette(QColor(i, i, i)));
		}
		process(McBgTransparent::CreateAlpha);
	});
	connect(d->sb_edgeThreshold, qOverload<int>(&QSpinBox::valueChanged), [this](int i) {
		if (!canManualProcess())
			return;
		process(McBgTransparent::EdgeSmooth);
	});
	connect(d->sb_blurThreshold, qOverload<int>(&QSpinBox::valueChanged), [this](int i) {
		if (!canManualProcess())
			return;
		process(McBgTransparent::EdgeSmooth);
	});
	connect(d->pb_autoProcess, &QPushButton::clicked, [this]() {
		if (!canManualProcess())
			return;
		/*	������ÿ�����һ���߳���������Ҫԭ�����£�
			1.����Ҫ�����ͼƬ��������ʱ��ֱ�Ӵ�����ܻ���ɽ��濨��
			2.ֱ�Ӵ������Ῠס��ֱ������ͼƬ������ϣ���������һ���߳�ʱ���ʵʱˢ�½��棬����������еĴ�������
		*/
		d->imageLabel->setFixedSize(d->frame->size() - QSize(5, 5));
		QtConcurrent::run(this, &McBgTransparent::autoProcess);
	});
}

void McBgTransparent::reset() {
	if (!canManualProcess()) {
		mcOutput << "��ʱ���������ò���";
		return;
	}
	d->imageLabel->clear();
	d->imageLabel->setFixedSize(0, 0);
}

bool McBgTransparent::hasInputImage(const QList<QImage> &images) {
	if (!canManualProcess()) {
		mcOutput << "��ʱ����������";
		return false;
	}
	if (images.isEmpty()) {
		mcOutput << "����ͼƬΪ��";
		return false;
	}
	m_currentIndex = m_images.size();
	m_images.append(images);
	if (m_currentIndex >= m_images.size()) {
		m_currentIndex = 0;
	}
	loadImage(m_currentIndex);
	return true;
}

bool McBgTransparent::beingSaved() {
	if (!canManualProcess()) {
		mcOutput << "ͼƬ���ڴ�����ʱ��������";
		return false;
	}
	updateState("���ڱ���...");
	return true;
}

bool McBgTransparent::saveCompleted() {
	updateState("ȫ���������");
	d->imageLabel->clear();
	m_images.clear();
	return true;
}

bool McBgTransparent::saveCompleted(int index) {
	if (m_images.size() == 1) {
		saveCompleted();
		return false;
	}
	updateState("�������");
	m_images.removeAt(m_currentIndex);
	loadImage(m_currentIndex);
	return true;
}

void McBgTransparent::resizeEvent(QResizeEvent *event) {
	if (!hasPixmap()) {
		QWidget::resizeEvent(event);
		return;
	}
	QImage currentImage = d->imageLabel->pixmap()->toImage();
	// ��label�ϵ�ͼƬ�Ȼ���ͼƬСʱ����ʾԭlabel��С��������ʱ��Ҫ�������룬�����ȣ���ʾͼƬδ�����任����������
	if (currentImage.size() == d->m_currImage.size()) {
		QWidget::resizeEvent(event);
		return;
	}
	loadImage(d->m_currImage);
	Widget::resizeEvent(event);
}

bool McBgTransparent::eventFilter(QObject *watched, QEvent *event) {
	if (watched == d->imageLabel) {
		static bool isMouseLeftPress = false;
		static bool isCtrlPress = false;
		if (event->type() == QEvent::KeyPress) {
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			if (keyEvent->key() == Qt::Key_Control) {
				isCtrlPress = true;
				d->m_mouseState = McBgTransparent::Erase;
			}
		}
		else if (event->type() == QEvent::KeyRelease) {
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			if (keyEvent->key() == Qt::Key_Control) {
				isCtrlPress = false;
				d->m_mouseState = McBgTransparent::Normal;
			}
		}
		else if (event->type() == QEvent::MouseButtonPress) {
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			const QPointF &pos = mouseEvent->localPos();
			if (mouseEvent->button() == Qt::LeftButton) {
				isMouseLeftPress = true;
				if(d->m_mouseState == McBgTransparent::Normal)
					showColor(pos.toPoint());
				else if (d->m_mouseState == McBgTransparent::Erase) {
					if (isCtrlPress) {
						setCursor(pos.toPoint());
						imageErase(pos);
					}
				}
			}
		}
		else if (event->type() == QEvent::MouseButtonRelease) {
			if (static_cast<QMouseEvent *>(event)->button() == Qt::LeftButton) {
				isMouseLeftPress = false;
				unsetCursor();
			}
		}
		else if (event->type() == QEvent::MouseMove) {
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			const QPointF &pos = mouseEvent->localPos();
			if(isMouseLeftPress && d->m_mouseState == McBgTransparent::Normal)
				showColor(pos.toPoint());
			else if (isMouseLeftPress && d->m_mouseState == McBgTransparent::Erase) {
				if(isCtrlPress)
					imageErase(pos);
			}
		}
	}
	return QWidget::eventFilter(watched, event);
}

void McBgTransparent::setCursor(const QPoint &pos) {
	if (!hasPixmap())
		return;
	QImage img = d->imageLabel->pixmap()->toImage();
	QRgb rgb = img.pixel(pos);
	int gray = qGray(rgb);
	if(gray <= 50)
		Widget::setCursor(QCursor(QPixmap(":/GraphProc/images/graphProc/bgTransparent/eraser1.png", "png")));
	else
		Widget::setCursor(QCursor(QPixmap(":/GraphProc/images/graphProc/bgTransparent/eraser.png", "png")));
}

void McBgTransparent::showColor(const QPoint &pos) {
	if (!hasPixmap())
		return;
	QImage img = d->imageLabel->pixmap()->toImage();
	QRgb rgb = img.pixel(pos);
	int red = qRed(rgb);
	int green = qGreen(rgb);
	int blue = qBlue(rgb);
	int gray = qGray(rgb);
	mcOutput << QString("red:%1 green:%2 blue:%3 gray:%4").arg(
		QString::number(red),
		QString::number(green),
		QString::number(blue),
		QString::number(gray));
}

void McBgTransparent::imageErase(const QPointF &pos) {
	if (!hasPixmap())
		return;
	QPixmap p = cursor().pixmap();
	qreal scaled = d->m_currImage.width() / (d->imageLabel->pixmap()->width() * 1.0);
	int width = p.width();
	int height = p.height();
	int x = (pos.x() - (width / (2 * 1.0))) * scaled;
	int y = (pos.y() - (height / (2 * 1.0))) * scaled;
	width *= scaled;
	height *= scaled;
	x = x < 0 ? 0 : x;
	y = y < 0 ? 0 : y;
	if (d->m_procCurrState == McBgTransparent::CreateAlpha) {
		for (int i = 0; i < height; i++) { // y
			int tempY = (i + y);
			if (tempY >= d->m_alphaImage.height())
				break;
			for (int j = 0; j < width; j++) {	// x
				int tempX = (j + x);
				if (tempX >= d->m_alphaImage.width())
					break;
				d->m_alphaImage.setPixel(tempX, tempY, 255);
			}
		}
		
		//cv::Mat src = Mc::QImage2cvMat(d->m_currProcImage);
		//for (int i = 0; i < height; i++) {	// row
		//	for (int j = 0; j < width; j++) {	// col
		//		int tempRow = (i + y);
		//		int tempCol = (j + x);
		//		src.at<uchar>(tempRow, tempCol) = 255;
		//	}
		//}
		//d->m_currProcImage = Mc::cvMat2QImage(src);
		loadImage(d->m_alphaImage);
	}
	else if (d->m_procCurrState == McBgTransparent::EdgeSmooth) {
		for (int i = 0; i < height; i++) { // y
			int tempY = (i + y);
			if (tempY >= d->m_smoothImage.height())
				break;
			for (int j = 0; j < width; j++) {	// x
				int tempX = (j + x);
				if (tempX >= d->m_smoothImage.width())
					break;
				d->m_smoothImage.setPixel(tempX, tempY, 255);
				if(tempY < d->m_alphaImage.height() && tempX < d->m_alphaImage.width())
					d->m_alphaImage.setPixel(tempX, tempY, 255);
			}
		}
		loadImage(d->m_smoothImage);
	}
	else if (d->m_procCurrState == McBgTransparent::AddAlpha) {
		cv::Mat cvAlpha = Mc::QImage2cvMat(d->m_smoothImage);
		for (int i = 0; i < height; i++) { // rows
			int tempRow = (i + y);
			if (tempRow >= d->m_transparentImage.height())
				break;
			for (int j = 0; j < width; j++) {	// cols
				int tempCol = (j + x);
				if (tempCol >= d->m_transparentImage.width())
					break;
				cvAlpha.at<uchar>(tempRow, tempCol) = 255;
			}
		}
		d->m_smoothImage = Mc::cvMat2QImage(cvAlpha);
		d->m_transparentImage = addAlpha(d->m_transparentImage, cvAlpha);
		loadImage(d->m_transparentImage);
	}
}

// �ж��Ƿ�ɲ���
bool McBgTransparent::canManualProcess() {
	if (d->m_canManualProcess)
		return true;
	mcOutput << "��ʱ�������ֶ��Ķ�";
	return false;
}

// ����ͼƬ������ʼ��״̬
void McBgTransparent::loadImage(int index) {
	if (index >= m_images.size() || index < 0)
		return;
	QImage image = m_images.at(index);
	d->m_imgCurrState = "��ʼͼƬ";
	d->m_procCurrState = McBgTransparent::Init;
	d->m_currImage = image;
	loadImage(image);
}

void McBgTransparent::loadImage(const QImage &image) {
	QImage img;
	// ֻҪ����ͼƬ������Ҫ�ж��Ƿ���Ҫ���ţ������Ҫ����ͼƬС�ڵ���frame����ֱ�����룬������ڣ�����Ҫ����
	img = isSmallSize(image) ? image : scaledImage(image);
	/*	Ϊʵ�������ͼƬ��ȡͼƬ��rgbֵ��label��С����img��С�䶯���䶯
		���Զ�����ʱ��label�ᱻ�̶�Ϊframe�Ĵ�С
	*/
	if (d->m_canManualProcess)
		d->imageLabel->setFixedSize(img.size());
	d->imageLabel->setPixmap(QPixmap::fromImage(img));
	updateState(d->m_imgCurrState);
}

QImage McBgTransparent::scaledImage(const QImage &image) {
	return image.scaled(d->frame->size() - QSize(5, 5), Qt::KeepAspectRatio, d->m_transformMode);
}

bool McBgTransparent::isSmallSize(const QImage &image) {
	if (image.width() < d->frame->width() - 5 && image.height() < d->frame->height() - 5) {
		return true;
	}
	return false;
}

void McBgTransparent::updateState(const QString &state) {
	d->l_imageState->setText(QString("%1:%2").arg(QString::number(m_currentIndex), state));
}

/*	threshold��ָ������ɫ��ֵ
	variations����ֵ�ɱ䶯��Χ������Ϊ�㣬��ʾ�Ҷ�ֵ������ֵ����������򽫸�ֵ����2��������ֵ������(0-255)
*/
QImage McBgTransparent::createAlpha(QImage& img, int threshold, int variations) {
	int l, h, m;
	m = variations / 2;
	l = threshold - m;		// ��ֵ����
	l = (l < 0) ? 0 : l;	// ���õ�����
	h = threshold + m;		// ��ֵ����
	h = (h > 255) ? 255 : h;	// ���ø���255
	cv::Mat src = Mc::QImage2cvMat(img);
	cv::Mat alpha = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	cv::Mat gray = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);	// ��QImageת��������ͼ��ΪR G B����˳��
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			uchar temp = gray.at<uchar>(i, j);
			// �������ص�Ҷ�ֵ����ĳһ��ֵ֮��ʱ������͸��
			alpha.at<uchar>(i, j) = (temp >= l && temp <= h) ? 0 : 255;
		}
	}
	return Mc::cvMat2QImage(alpha);		// ����Ϊ8λ��ͨ��ͼ��������ת��
}

/*	edgeThreshold����Ե��ֵ��alphaͨ��ֵλ�ڸ�ֵ���ϵĵ㽫��͸��������ʼֵΪ200
	blurThreshold��ģ����ֵ����˹ģ�����ڸ�ֵ���ϵĵ㽫�ᱻ��Ϊ255������ģ���������㲻����������ģ������ʼֵΪ150
*/
QImage McBgTransparent::edgeSmooth(QImage &img, QImage &alpha, int edgeThreshold, int blurThreshold) {
	cv::Mat src = Mc::QImage2cvMat(img);
	cv::Mat cvAlpha = Mc::QImage2cvMat(alpha);
	cv::Mat gray = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);
	cv::Mat newAlpha = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			uchar temp = cvAlpha.at<uchar>(i, j);
			if (temp == 255) {
				/*	�жϸ����ص��Ƿ���������Ե�������Ǹ����ص����������κ�һ�ߴ���һ�����alphaֵΪ0
					��������ص������ͼƬ��Ե������Ϊ�����ص㴦��������Ե
				*/
				bool left = (j == 0) ? true : cvAlpha.at<uchar>(i, j - 1) == 0;
				bool right = (j == src.cols - 1) ? true : cvAlpha.at<uchar>(i, j + 1) == 0;
				bool above = (i == 0) ? true : cvAlpha.at<uchar>(i - 1, j) == 0;
				bool bottom = (i == src.rows - 1) ? true : cvAlpha.at<uchar>(i + 1, j) == 0;
				if (left || right || above || bottom) {
					if (gray.at<uchar>(i, j) >= edgeThreshold) {
						temp = 0;
					}
				}
			}
			newAlpha.at<uchar>(i, j) = temp;
		}
	}
	// ��˹ģ��
	GaussianBlur(newAlpha, newAlpha, cv::Size(5, 5), 5, 5);
	// ��Եģ����������ֵ����ĳһ��ֵ֮��ʱ����ֵ��Ϊ255������ģ�������򲻴�����ģ��
	for (int i = 0; i < newAlpha.rows; i++) {
		uchar* p = newAlpha.ptr<uchar>(i);
		for (int j = 0; j < newAlpha.cols; j++) {
			if (p[j] > blurThreshold)
				p[j] = 255;
		}
	}
	return Mc::cvMat2QImage(newAlpha);
}

QImage McBgTransparent::addAlpha(QImage &img, QImage &alpha) {
	cv::Mat cvAlpha = Mc::QImage2cvMat(alpha);
	return addAlpha(img, cvAlpha);
}

QImage McBgTransparent::addAlpha(QImage &img, cv::Mat &alpha) {
	cv::Mat src = Mc::QImage2cvMat(img);
	cv::Mat dst;
	if (src.channels() == 1) {
		cv::cvtColor(src, src, cv::COLOR_GRAY2RGB);
	}
	dst = cv::Mat(src.rows, src.cols, CV_8UC4);
	std::vector<cv::Mat> srcChannels;
	std::vector<cv::Mat> dstChannels;
	srcChannels.reserve(4);
	dstChannels.reserve(4);
	//����ͨ��
	cv::split(src, srcChannels);
	dstChannels.push_back(srcChannels[0]);
	dstChannels.push_back(srcChannels[1]);
	dstChannels.push_back(srcChannels[2]);
	//���͸����ͨ��
	dstChannels.push_back(alpha);
	//�ϲ�ͨ��
	cv::merge(dstChannels, dst);
	return Mc::cvMat2QImage(dst);
}

cv::Mat McBgTransparent::getAlpha(const QImage &img) {
	cv::Mat src = Mc::QImage2cvMat(img);
	cv::Mat dst;
	if (src.channels() == 1) {
		cv::cvtColor(src, src, cv::COLOR_GRAY2RGBA);
	}
	else if (src.channels() == 3) {
		cv::cvtColor(src, src, cv::COLOR_BGR2RGBA);
	}
	std::vector<cv::Mat> srcChannels;
	std::vector<cv::Mat> dstChannels;
	srcChannels.reserve(4);
	dstChannels.reserve(1);
	cv::split(src, srcChannels);
	dstChannels.push_back(srcChannels[3]);
	cv::merge(dstChannels, dst);
	return dst;
}

// ���ִ���״̬�л������Զ����ô�����
void McBgTransparent::nextProc() {
	if (!hasPixmap() || d->m_currImage.isNull())
		return;
	switch (d->m_procCurrState){
	case Init:
		d->m_procCurrState = McBgTransparent::CreateAlpha;
		d->m_imgCurrState = "alphaͨ��ͼƬ";
		break;
	case CreateAlpha:
		d->m_procCurrState = McBgTransparent::EdgeSmooth;
		d->m_imgCurrState = "��Եƽ��";
		break;
	case EdgeSmooth:
		d->m_procCurrState = McBgTransparent::AddAlpha;
		d->m_imgCurrState = "ͼ���ں�";
		break;
	case AddAlpha:
		d->m_procCurrState = McBgTransparent::Init;
		d->m_imgCurrState = "�������";
		d->m_currImage = d->m_transparentImage;
		m_images.replace(m_currentIndex, d->m_currImage);
		loadImage(d->m_currImage);
		break;
	default:
		break;
	}
	procing();
}

// ���ݵ�ǰ״̬����ͼ�񣬲����浱ǰ���������ͼ��
void McBgTransparent::procing() {
	if (d->m_currImage.isNull())
		return;
	switch (d->m_procCurrState) {
	case Init:
		break;
	case CreateAlpha:
		d->m_alphaImage = createAlpha(d->m_currImage, d->sb_bgGray->value(), d->sb_colorD_value->value());
		loadImage(d->m_alphaImage);
		break;
	case EdgeSmooth:
		d->m_smoothImage = edgeSmooth(d->m_currImage, d->m_alphaImage, d->sb_edgeThreshold->value(), d->sb_blurThreshold->value());
		loadImage(d->m_smoothImage);
		break;
	case AddAlpha:
		d->m_transparentImage = addAlpha(d->m_currImage, d->m_smoothImage);
		loadImage(d->m_transparentImage);
		break;
	default:
		break;
	}
}

void McBgTransparent::process(ProcState state) {
	if (d->m_procCurrState != state && d->m_procCurrState != (state + 1))
		return;
	if (d->m_procCurrState == state) {
		procing();
	}
	else if (d->m_procCurrState == (state + 1)) {
		d->m_procCurrState = state;
		procing();
		nextProc();
	}
}

void McBgTransparent::showNext() {
	m_currentIndex = m_currentIndex == m_images.size() - 1 ? 0 : m_currentIndex + 1;
	loadImage(m_currentIndex);
}

// �Զ�����
void McBgTransparent::autoProcess() {
	d->m_canManualProcess = false;
	for (int i = m_currentIndex; i < m_images.size(); i++) {
		procing();
		procing();
		procing();
		procing();
		showNext();
	}
	d->m_canManualProcess = true;
}

bool McBgTransparent::hasPixmap() {
	const QPixmap *p = d->imageLabel->pixmap();
	if (!p)
		return false;
	return true;
}
