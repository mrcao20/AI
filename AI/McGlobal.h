#pragma once
#pragma execution_character_set("utf-8")

#define OPENCV

#include <qimage.h>
#include <qlist.h>
#include "McOutput.h"

#define mcOutput McOutput()

#ifdef _DEBUG
	#ifndef DEBUG
		#define DEBUG
	#endif
#else
	#ifndef RELEASE
		#define RELEASE
	#endif
#endif // _DEBUG

namespace Mc {

enum ShapeType{
	Rect,
	Polygon,
	Ellipse,
	Line,
	Pen
};

enum SortAlgorithm {
	QuickSort,
	CountingSort
};

void sort(QList<int> &list, SortAlgorithm sa);

// ���ڶ����ͼƬΪһ�飬��ͼƬ���󲿷��ַ���ͬ���������ֵı�����ֲ�ͬ
bool sort(QStringList &list, SortAlgorithm sa);

}

#ifdef OPENCV
#include <opencv2/opencv.hpp>

namespace Mc {
	QImage cvMat2QImage(const cv::Mat& mat);
	cv::Mat QImage2cvMat(const QImage &image);
}
#endif // OPENCV
