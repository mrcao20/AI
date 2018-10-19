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
			mcOutput << "暂时不允许手动改动";
			return;
		}
		if (text == "快速变换") {
			d->m_transformMode = Qt::FastTransformation;
		}
		else if (text == "平滑变换") {
			d->m_transformMode = Qt::SmoothTransformation;
		}
		if (!d->imageLabel->pixmap()) {
			return;
		}
		loadImage(m_currentIndex);
	});
	connect(d->pb_previousImage, &QPushButton::clicked, [this]() {
		if (!d->m_canManualProcess) {
			mcOutput << "暂时不允许手动改动";
			return;
		}
		m_currentIndex = m_currentIndex == 0 ? m_images.size() - 1 : m_currentIndex - 1;
		loadImage(m_currentIndex);
		d->pb_transparent->setText("开始");
	});
	connect(d->pb_nextImage, &QPushButton::clicked, [this]() {
		if (!d->m_canManualProcess) {
			mcOutput << "暂时不允许手动改动";
			return;
		}
		showNext();
	});
	connect(d->pb_transparent, &QPushButton::clicked, [this]() {
		if (!d->m_canManualProcess) {
			mcOutput << "暂时不允许手动改动";
			return;
		}
		imageTransparent();
	});
	connect(d->pb_bgColor, &QPushButton::clicked, [this]() {
		if (!d->m_canManualProcess) {
			mcOutput << "暂时不允许手动改动";
			return;
		}
		QColor c = QColorDialog::getColor();
		if (!c.isValid())
			return;
		d->f_bgColor->setPalette(QPalette(c));
		d->sb_bgGray->setValue(qGray(c.rgb()));
		if (d->m_imgCurrState != "alpha通道图片")
			return;
		d->m_imgCurrState = "起始图片";
		imageTransparent();
	});
	connect(d->sb_variations, qOverload<int>(&QSpinBox::valueChanged), [this](int i) {
		if (!d->m_canManualProcess) {
			mcOutput << "暂时不允许手动改动";
			return;
		}
		d->m_variations = i;
		if (d->m_imgCurrState != "alpha通道图片")
			return;
		d->m_imgCurrState = "起始图片";
		imageTransparent();
	});
	connect(d->sb_bgGray, qOverload<int>(&QSpinBox::valueChanged), [this](int i) {
		if (!d->m_canManualProcess) {
			mcOutput << "暂时不允许手动改动";
			return;
		}
		d->m_alphaThreshold = i;
		QPalette p = d->f_bgColor->palette();
		if (qGray(p.color(QPalette::Background).rgb()) != i) {	
			d->f_bgColor->setPalette(QPalette(QColor(i, i, i)));
		}
		if (d->m_imgCurrState != "alpha通道图片")
			return;
		d->m_imgCurrState = "起始图片";
		imageTransparent();
	});
	connect(d->sb_edgeThreshold, qOverload<int>(&QSpinBox::valueChanged), [this](int i) {
		if (!d->m_canManualProcess) {
			mcOutput << "暂时不允许手动改动";
			return;
		}
		d->m_edgeThreshold = i;
		if (d->m_imgCurrState != "边缘平滑")
			return;
		d->m_imgCurrState = "alpha通道图片";
		imageTransparent();
	});
	connect(d->sb_blurThreshold, qOverload<int>(&QSpinBox::valueChanged), [this](int i) {
		if (!d->m_canManualProcess) {
			mcOutput << "暂时不允许手动改动";
			return;
		}
		d->m_blurThreshold = i;
		if (d->m_imgCurrState != "边缘平滑")
			return;
		d->m_imgCurrState = "alpha通道图片";
		imageTransparent();
	});
	connect(d->pb_autoProcess, &QPushButton::clicked, [this]() {
		if (!d->m_canManualProcess) {
			mcOutput << "暂时不允许手动改动";
			return;
		}
		/*	这里采用开辟另一条线程来处理，主要原因如下：
			1.当需要处理的图片数量过多时，直接处理可能会造成界面卡死
			2.直接处理界面会卡住，直到所有图片处理完毕，但开辟另一条线程时则会实时刷新界面，可以浏览所有的处理流程
		*/
		d->imageLabel->setFixedSize(d->frame->size() - QSize(5, 5));
		QtConcurrent::run(this, &BgTransparent::autoProcess);
	});
}

void BgTransparent::reset() {
	if (!d->m_canManualProcess) {
		mcOutput << "暂时不允许重置操作";
		return;
	}
	d->imageLabel->clear();
	d->imageLabel->setFixedSize(0, 0);
}

bool BgTransparent::hasInputImage(const QList<QImage> &images) {
	if (!d->m_canManualProcess) {
		mcOutput << "图片正在处理，暂时不允许载入";
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
		mcOutput << "图片正在处理，暂时不允许保存";
		return false;
	}
	updateState("正在保存...");
	return true;
}

bool BgTransparent::saveCompleted() {
	updateState("全部保存完毕");
	d->imageLabel->clear();
	m_images.clear();
	return true;
}

bool BgTransparent::saveCompleted(int index) {
	if (m_images.size() == 1) {
		saveCompleted();
		return false;
	}
	updateState("保存完毕");
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
	// 当label上的图片比缓存图片小时，表示原label大小不够，此时需要重新载入，如果相等，表示图片未经过变换，则不做操作
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

// 一共操作4次，完成背景透明
void BgTransparent::imageTransparent() {
	if (d->m_currImage.isNull())
		return;
	// 1.处于初始状态，获取alpha通道图片
	if (d->m_imgCurrState == "起始图片") {
		d->pb_transparent->setText("下一步");
		d->m_imgCurrState = "alpha通道图片";
		QImage alpha = createAlpha(d->m_currImage, d->m_alphaThreshold, d->m_variations);
		loadImage(alpha);
		d->m_alphaImage = alpha;
	}
	// 2.处于alpha通道图片状态，进行边缘平滑
	else if (d->m_imgCurrState == "alpha通道图片") {
		d->m_imgCurrState = "边缘平滑";
		QImage smooth = edgeSmooth(d->m_currImage, d->m_alphaImage, d->m_edgeThreshold, d->m_blurThreshold);
		loadImage(smooth);
		d->m_smoothImage = smooth;
	}
	// 3.处于边缘平滑状态，进行透明融合
	else if (d->m_imgCurrState == "边缘平滑") {
		d->pb_transparent->setText("结束");
		d->m_imgCurrState = "透明融合";
		QImage transparent = addAlpha(d->m_currImage, d->m_smoothImage);
		loadImage(transparent);
		d->m_transparentImage = transparent;
	}
	// 4.处于透明融合状态，结束操作
	else if (d->m_imgCurrState == "透明融合") {
		d->pb_transparent->setText("开始");
		d->m_currImage = d->m_transparentImage;
		m_images.replace(m_currentIndex, d->m_currImage);
	}
}

void BgTransparent::loadImage(int index) {
	if (index >= m_images.size() || index < 0)
		return;
	QImage image = m_images.at(index);
	d->m_imgCurrState = "起始图片";
	d->pb_transparent->setText("开始");
	d->m_currImage = image;
	loadImage(image);
}

void BgTransparent::loadImage(const QImage &image) {
	QImage img;
	// 只要加载图片，都需要判定是否需要缩放，如果需要载入图片小于等于frame，则直接载入，如果大于，则需要缩放
	img = isSmallSize(image) ? image : scaledImage(image);
	/*	为实现鼠标点击图片获取图片上rgb值，label大小将随img大小变动而变动
		当自动处理时，label会被固定为frame的大小
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

/*	threshold：指定的颜色阈值
	variations：阈值可变动范围，可以为零，表示灰度值等于阈值则操作。否则将该值整除2，计算阈值上下限(0-255)
*/
QImage BgTransparent::createAlpha(QImage& img, int threshold, int variations) {
	int l, h, m;
	m = variations / 2;
	l = threshold - m;		// 阈值下限
	l = (l < 0) ? 0 : l;	// 不得低于零
	h = threshold + m;		// 阈值上限
	h = (h > 255) ? 255 : h;	// 不得高于255
	cv::Mat src = Mc::QImage2cvMat(img);
	cv::Mat alpha = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	cv::Mat gray = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);	// 从QImage转换而来的图像为R G B排列顺序
	for (int i = 0; i < src.rows; i++){
		for (int j = 0; j < src.cols; j++){
			uchar temp = gray.at<uchar>(i, j);
			// 当该像素点灰度值处于某一阈值之上时，将其透明
			alpha.at<uchar>(i, j) = (temp >= l && temp <= h) ? 0 : 255;
		}
	}
	return Mc::cvMat2QImage(alpha);		// 这里为8位单通道图，故无需转换
}

/*	edgeThreshold：边缘阈值，alpha通道值位于该值以上的点将被透明化，初始值为200
	blurThreshold：模糊阈值，高斯模糊后处于该值以上的点将会被置为255，即不模糊，其他点不做操作，即模糊，初始值为150
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
				/*	判断该像素点是否处于轮廓边缘，条件是该像素点上下左右任何一边存在一个点的alpha值为0
					如果该像素点存在于图片边缘，则认为该像素点处于轮廓边缘
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
	// 高斯模糊
	GaussianBlur(newAlpha, newAlpha, cv::Size(5, 5), 5, 5);
	// 边缘模糊：当像素值处于某一阈值之上时，该值置为255，即不模糊，否则不处理，即模糊
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
	//分离通道
	cv::split(src, srcChannels);
	dstChannels.push_back(srcChannels[0]);
	dstChannels.push_back(srcChannels[1]);
	dstChannels.push_back(srcChannels[2]);
	//添加透明度通道
	dstChannels.push_back(cvAlpha);
	//合并通道
	cv::merge(dstChannels, dst);
	return Mc::cvMat2QImage(dst);
}

void BgTransparent::showNext() {
	m_currentIndex = m_currentIndex == m_images.size() - 1 ? 0 : m_currentIndex + 1;
	loadImage(m_currentIndex);
	d->pb_transparent->setText("开始");
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
