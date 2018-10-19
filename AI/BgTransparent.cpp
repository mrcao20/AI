#include "BgTransparent.h"
#include <qevent.h>
#include <qmimedata.h>
#include <qevent.h>
#include <QtConcurrent>
#include <qcolordialog.h>
#include <qcolor.h>
#include <opencv2/opencv.hpp>
#include "ui_BgTransparent.h"
#include "McGlobal.h"

struct BgTransparentData : public Ui::BgTransparent {
	Qt::TransformationMode m_transformMode;
	QImage m_currImage;
	QImage m_alphaImage;
	QImage m_smoothImage;
	QImage m_transparentImage;
	QString m_imgCurrState;
	int m_alphaThreshold;
	int m_variations;
	int m_edgeThreshold;
	int m_blurThreshold;
	bool m_canManualProcess;
};

BgTransparent::BgTransparent(QWidget *parent)
	: Widget(parent)
	, d(new BgTransparentData)
{
	d->setupUi(this);
	m_currentIndex = 0;
	d->m_transformMode = Qt::FastTransformation;
	d->m_variations = 0;
	d->m_alphaThreshold = 255;
	d->m_edgeThreshold = 200;
	d->m_blurThreshold = 150;
	d->m_canManualProcess = true;

	d->imageLabel->installEventFilter(this);

	connect(d->transformCbB, &QComboBox::currentTextChanged, [this](const QString &text) {
		if (!d->m_canManualProcess) {
			mcOutput << "��ʱ�������ֶ��Ķ�";
			return;
		}
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
		if (!d->m_canManualProcess) {
			mcOutput << "��ʱ�������ֶ��Ķ�";
			return;
		}
		m_currentIndex = m_currentIndex == 0 ? m_images.size() - 1 : m_currentIndex - 1;
		loadImage(m_currentIndex);
		d->pb_transparent->setText("��ʼ");
	});
	connect(d->pb_nextImage, &QPushButton::clicked, [this]() {
		if (!d->m_canManualProcess) {
			mcOutput << "��ʱ�������ֶ��Ķ�";
			return;
		}
		showNext();
	});
	connect(d->pb_transparent, &QPushButton::clicked, [this]() {
		if (!d->m_canManualProcess) {
			mcOutput << "��ʱ�������ֶ��Ķ�";
			return;
		}
		imageTransparent();
	});
	connect(d->pb_bgColor, &QPushButton::clicked, [this]() {
		if (!d->m_canManualProcess) {
			mcOutput << "��ʱ�������ֶ��Ķ�";
			return;
		}
		QColor c = QColorDialog::getColor();
		if (!c.isValid())
			return;
		d->f_bgColor->setPalette(QPalette(c));
		d->sb_bgGray->setValue(qGray(c.rgb()));
		if (d->m_imgCurrState != "alphaͨ��ͼƬ")
			return;
		d->m_imgCurrState = "��ʼͼƬ";
		imageTransparent();
	});
	connect(d->sb_variations, qOverload<int>(&QSpinBox::valueChanged), [this](int i) {
		if (!d->m_canManualProcess) {
			mcOutput << "��ʱ�������ֶ��Ķ�";
			return;
		}
		d->m_variations = i;
		if (d->m_imgCurrState != "alphaͨ��ͼƬ")
			return;
		d->m_imgCurrState = "��ʼͼƬ";
		imageTransparent();
	});
	connect(d->sb_bgGray, qOverload<int>(&QSpinBox::valueChanged), [this](int i) {
		if (!d->m_canManualProcess) {
			mcOutput << "��ʱ�������ֶ��Ķ�";
			return;
		}
		d->m_alphaThreshold = i;
		QPalette p = d->f_bgColor->palette();
		if (qGray(p.color(QPalette::Background).rgb()) != i) {	
			d->f_bgColor->setPalette(QPalette(QColor(i, i, i)));
		}
		if (d->m_imgCurrState != "alphaͨ��ͼƬ")
			return;
		d->m_imgCurrState = "��ʼͼƬ";
		imageTransparent();
	});
	connect(d->sb_edgeThreshold, qOverload<int>(&QSpinBox::valueChanged), [this](int i) {
		if (!d->m_canManualProcess) {
			mcOutput << "��ʱ�������ֶ��Ķ�";
			return;
		}
		d->m_edgeThreshold = i;
		if (d->m_imgCurrState != "��Եƽ��")
			return;
		d->m_imgCurrState = "alphaͨ��ͼƬ";
		imageTransparent();
	});
	connect(d->sb_blurThreshold, qOverload<int>(&QSpinBox::valueChanged), [this](int i) {
		if (!d->m_canManualProcess) {
			mcOutput << "��ʱ�������ֶ��Ķ�";
			return;
		}
		d->m_blurThreshold = i;
		if (d->m_imgCurrState != "��Եƽ��")
			return;
		d->m_imgCurrState = "alphaͨ��ͼƬ";
		imageTransparent();
	});
	connect(d->pb_autoProcess, &QPushButton::clicked, [this]() {
		if (!d->m_canManualProcess) {
			mcOutput << "��ʱ�������ֶ��Ķ�";
			return;
		}
		/*	������ÿ�����һ���߳���������Ҫԭ�����£�
			1.����Ҫ�����ͼƬ��������ʱ��ֱ�Ӵ�����ܻ���ɽ��濨��
			2.ֱ�Ӵ������Ῠס��ֱ������ͼƬ������ϣ���������һ���߳�ʱ���ʵʱˢ�½��棬����������еĴ�������
		*/
		d->imageLabel->setFixedSize(d->frame->size() - QSize(5, 5));
		QtConcurrent::run(this, &BgTransparent::autoProcess);
	});
}

void BgTransparent::reset() {
	if (!d->m_canManualProcess) {
		mcOutput << "��ʱ���������ò���";
		return;
	}
	d->imageLabel->clear();
	d->imageLabel->setFixedSize(0, 0);
}

bool BgTransparent::hasInputImage(const QList<QImage> &images) {
	if (!d->m_canManualProcess) {
		mcOutput << "ͼƬ���ڴ�����ʱ����������";
		return false;
	}
	if (images.isEmpty()) {
		qDebug() << "error for input image, please check!";
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

bool BgTransparent::beingSaved() {
	if (!d->m_canManualProcess) {
		mcOutput << "ͼƬ���ڴ�����ʱ��������";
		return false;
	}
	updateState("���ڱ���...");
	return true;
}

bool BgTransparent::saveCompleted() {
	updateState("ȫ���������");
	d->imageLabel->clear();
	m_images.clear();
	return true;
}

bool BgTransparent::saveCompleted(int index) {
	if (m_images.size() == 1) {
		saveCompleted();
		return false;
	}
	updateState("�������");
	m_images.removeAt(m_currentIndex);
	loadImage(m_currentIndex);
	return true;
}

void BgTransparent::resizeEvent(QResizeEvent *event) {
	const QPixmap *p = d->imageLabel->pixmap();
	if (!p) {
		QWidget::resizeEvent(event);
		return;
	}
	QImage currentImage = p->toImage();
	// ��label�ϵ�ͼƬ�Ȼ���ͼƬСʱ����ʾԭlabel��С��������ʱ��Ҫ�������룬�����ȣ���ʾͼƬδ�����任����������
	if (currentImage.size() == d->m_currImage.size()) {
		QWidget::resizeEvent(event);
		return;
	}
	loadImage(d->m_currImage);
	QWidget::resizeEvent(event);
}

bool BgTransparent::eventFilter(QObject *watched, QEvent *event) {
	if (watched == d->imageLabel) {
		if (event->type() == QEvent::MouseButtonPress) {
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			QPoint pos = mouseEvent->pos();
			const QPixmap *p = d->imageLabel->pixmap();
			if (p) {
				QImage img = p->toImage();
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
				return true;
			}
		}
	}
	return QWidget::eventFilter(watched, event);
}

// һ������4�Σ���ɱ���͸��
void BgTransparent::imageTransparent() {
	if (d->m_currImage.isNull())
		return;
	// 1.���ڳ�ʼ״̬����ȡalphaͨ��ͼƬ
	if (d->m_imgCurrState == "��ʼͼƬ") {
		d->pb_transparent->setText("��һ��");
		d->m_imgCurrState = "alphaͨ��ͼƬ";
		QImage alpha = createAlpha(d->m_currImage, d->m_alphaThreshold, d->m_variations);
		loadImage(alpha);
		d->m_alphaImage = alpha;
	}
	// 2.����alphaͨ��ͼƬ״̬�����б�Եƽ��
	else if (d->m_imgCurrState == "alphaͨ��ͼƬ") {
		d->m_imgCurrState = "��Եƽ��";
		QImage smooth = edgeSmooth(d->m_currImage, d->m_alphaImage, d->m_edgeThreshold, d->m_blurThreshold);
		loadImage(smooth);
		d->m_smoothImage = smooth;
	}
	// 3.���ڱ�Եƽ��״̬������͸���ں�
	else if (d->m_imgCurrState == "��Եƽ��") {
		d->pb_transparent->setText("����");
		d->m_imgCurrState = "͸���ں�";
		QImage transparent = addAlpha(d->m_currImage, d->m_smoothImage);
		loadImage(transparent);
		d->m_transparentImage = transparent;
	}
	// 4.����͸���ں�״̬����������
	else if (d->m_imgCurrState == "͸���ں�") {
		d->pb_transparent->setText("��ʼ");
		d->m_currImage = d->m_transparentImage;
		m_images.replace(m_currentIndex, d->m_currImage);
	}
}

void BgTransparent::loadImage(int index) {
	if (index >= m_images.size() || index < 0)
		return;
	QImage image = m_images.at(index);
	d->m_imgCurrState = "��ʼͼƬ";
	d->pb_transparent->setText("��ʼ");
	d->m_currImage = image;
	loadImage(image);
}

void BgTransparent::loadImage(const QImage &image) {
	QImage img;
	// ֻҪ����ͼƬ������Ҫ�ж��Ƿ���Ҫ���ţ������Ҫ����ͼƬС�ڵ���frame����ֱ�����룬������ڣ�����Ҫ����
	img = isSmallSize(image) ? image : scaledImage(image);
	/*	Ϊʵ�������ͼƬ��ȡͼƬ��rgbֵ��label��С����img��С�䶯���䶯
		���Զ�����ʱ��label�ᱻ�̶�Ϊframe�Ĵ�С
	*/
	if(d->m_canManualProcess)
		d->imageLabel->setFixedSize(img.size());
	d->imageLabel->setPixmap(QPixmap::fromImage(img));
	updateState(d->m_imgCurrState);
}

QImage BgTransparent::scaledImage(const QImage &image) {
	return image.scaled(d->frame->size() - QSize(5, 5), Qt::KeepAspectRatio, d->m_transformMode);
}

bool BgTransparent::isSmallSize(const QImage &image) {
	if (image.width() < d->frame->width() - 5 && image.height() < d->frame->height() - 5) {
		return true;
	}
	return false;
}

void BgTransparent::updateState(const QString &state) {
	d->l_imageState->setText(QString("%1:%2").arg(QString::number(m_currentIndex), state));
}

/*	threshold��ָ������ɫ��ֵ
	variations����ֵ�ɱ䶯��Χ������Ϊ�㣬��ʾ�Ҷ�ֵ������ֵ����������򽫸�ֵ����2��������ֵ������(0-255)
*/
QImage BgTransparent::createAlpha(QImage& img, int threshold, int variations) {
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
	for (int i = 0; i < src.rows; i++){
		for (int j = 0; j < src.cols; j++){
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
QImage BgTransparent::edgeSmooth(QImage &img, QImage &alpha, int edgeThreshold, int blurThreshold) {
	cv::Mat src = Mc::QImage2cvMat(img);
	cv::Mat cvAlpha = Mc::QImage2cvMat(alpha);
	cv::Mat gray = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);
	cv::Mat newAlpha = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	for (int i = 0; i < src.rows; i++){
		for (int j = 0; j < src.cols; j++){
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
	for (int i = 0; i < newAlpha.rows; i++){
		uchar* p = newAlpha.ptr<uchar>(i);
		for (int j = 0; j < newAlpha.cols; j++){
			if (p[j] > blurThreshold)
				p[j] = 255;
		}
	}
	return Mc::cvMat2QImage(newAlpha);
}

QImage BgTransparent::addAlpha(QImage &img, QImage &alpha){
	cv::Mat src = Mc::QImage2cvMat(img);
	cv::Mat dst;
	cv::Mat cvAlpha = Mc::QImage2cvMat(alpha);
	if (src.channels() == 1){
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
	dstChannels.push_back(cvAlpha);
	//�ϲ�ͨ��
	cv::merge(dstChannels, dst);
	return Mc::cvMat2QImage(dst);
}

void BgTransparent::showNext() {
	m_currentIndex = m_currentIndex == m_images.size() - 1 ? 0 : m_currentIndex + 1;
	loadImage(m_currentIndex);
	d->pb_transparent->setText("��ʼ");
}

void BgTransparent::autoProcess() {
	d->m_canManualProcess = false;
	for (int i = m_currentIndex; i < m_images.size(); i++) {
		imageTransparent();
		imageTransparent();
		imageTransparent();
		imageTransparent();
		showNext();
	}
	d->m_canManualProcess = true;
}

BgTransparent::~BgTransparent(){
}
