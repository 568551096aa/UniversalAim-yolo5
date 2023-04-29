#ifndef SCREENSHOT_H_
#define SCREENSHOT_H_
#include <windows.h>
#include <string>
#include <opencv2/opencv.hpp>
using std::string;


class DibCaptureHelper
{
public:
	DibCaptureHelper();
	virtual ~DibCaptureHelper();

	bool Init(int rectSize);
	void Cleanup();
	bool Capture(cv::Mat &img);

	const RECT& GetWindowRect() const { return windowRect_; }
	const RECT& GetClientRect() const { return clientRect_; }
	int GetBitmapDataSize() const { return bmpDataSize_; }
	HBITMAP GetBitmap() const { return bitmap_; }
	void* GetBitmapAddress() const { return bitsPtr_; }
	int posx_;
	int posy_;
	int rectsize_;
private:
	HWND hwnd_;
	HDC scrDc_;
	HDC memDc_;
	HBITMAP bitmap_;
	HBITMAP oldBitmap_;
	void* bitsPtr_;

	RECT windowRect_;
	RECT clientRect_;
	POINT bitbltStartPoint_;
	int bmpDataSize_;


	HDC disDc_;
};
#endif