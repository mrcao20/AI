#pragma once
#pragma execution_character_set("utf-8")

#include <qimage.h>
#include <opencv2/opencv.hpp>
#include <qlist.h>

QImage cvMat2QImage(const cv::Mat& mat);
cv::Mat QImage2cvMat(const QImage &image);

enum SortAlgorithm {
	QuickSort,
	CountingSort
};

void sort(QList<int> &list, SortAlgorithm sa);

// ���ڶ����ͼƬΪһ�飬��ͼƬ���󲿷��ַ���ͬ���������ֵı�����ֲ�ͬ
bool sort(QStringList &list, SortAlgorithm sa);
