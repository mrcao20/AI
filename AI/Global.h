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

// 由于读入的图片为一组，故图片名大部分字符相同，仅做区分的编号数字不同
bool sort(QStringList &list, SortAlgorithm sa);
