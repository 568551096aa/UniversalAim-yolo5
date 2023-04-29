#ifndef DETECT_H_
#define DETECT_H_
#include <vector>
#include <opencv2/opencv.hpp>
#pragma comment(lib, "myDetect.lib")
extern "C" void PreDetch_();
extern "C" void ImgDetch_(std::vector<cv::Rect> &rect, std::vector<float> &conf,
	std::vector<float> &classId, cv::Mat &img);
extern "C" void AfterDetch_();
void InitDet()
{
	PreDetch_();
}

void Detect(std::vector<cv::Rect> &rect, std::vector<float> &conf,
	std::vector<float> &classId, cv::Mat &img)
{
	if (img.data == nullptr) {
		std::cout << "open file fail" << std::endl;
		return;
	}
	ImgDetch_(rect, conf, classId, img);
}

void UnInitDet()
{
	AfterDetch_();
}
#endif