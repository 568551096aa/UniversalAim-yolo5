#ifndef DETECT_H_
#define DETECT_H_
#include <vector>
#include <opencv2/opencv.hpp>
#include "types.h"
#pragma comment(lib, "myDetect.lib")
extern "C" void PreDetch_();
extern "C" void ImgDetch_(cv::Rect* rect, float *conf,
	float* classId, cv::Mat &img, int &count);
extern "C" void AfterDetch_();
void InitDet()
{
	PreDetch_();
}

void Detect(cv::Rect* rect, float *conf,
	float* classId, cv::Mat &img, int &count)
{
	if (img.data == nullptr) {
		std::cout << "open file fail" << std::endl;
		return ;
	}
	return ImgDetch_(rect, conf, classId, img, count);
}

void UnInitDet()
{
	AfterDetch_();
}
#endif