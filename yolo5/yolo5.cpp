
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#pragma comment(lib, "myDetect.lib")
extern "C" void PreDetch_();
extern "C" void ImgDetch_(std::vector<cv::Rect> &rect, std::vector<float> &conf,
	std::vector<float> &classId, cv::Mat &img);
extern "C" void AfterDetch_();
int main() 
{
	PreDetch_();
	std::vector<cv::Rect> rects;
	std::vector<float> conf, classId;
	cv::Mat img = cv::imread("7.png");
	if (img.data == nullptr) {
		std::cout << "open file fail" << std::endl;
		return 0;
	}
	ImgDetch_(rects, conf, classId, img);
	AfterDetch_();
	return 0;
}