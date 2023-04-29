#include "ScreenShot.h"

#include"time.h"


DibCaptureHelper::DibCaptureHelper() : hwnd_(nullptr)
	, scrDc_(nullptr)
	, memDc_(nullptr)
	, bitmap_(nullptr)
	, oldBitmap_(nullptr)
	, bitsPtr_(nullptr)
	, windowRect_{ 0, 0, 0, 0 }
	, clientRect_{ 0, 0, 0, 0 }
	, bitbltStartPoint_{ 0,0 }
	, bmpDataSize_(0)
{
}


DibCaptureHelper::~DibCaptureHelper()
{
	Cleanup();
}



bool DibCaptureHelper::Init(int rectSize)
{
	rectsize_ = rectSize;
	hwnd_ = ::GetDesktopWindow();
	//获取窗口大小
	if (!::GetWindowRect(hwnd_, &windowRect_) || !::GetClientRect(hwnd_, &clientRect_))
	{
		return false;
	}
	const auto clientRectWidth = clientRect_.right - clientRect_.left;
	const auto clientRectHeight = clientRect_.bottom - clientRect_.top;
	std::cout << "width : " << clientRectWidth <<std::endl;
	std::cout << "height : " << clientRectHeight << std::endl;
	posx_ = clientRectWidth / 2 - rectSize / 2;
	posy_ = clientRectHeight / 2 - rectSize / 2;
	bmpDataSize_ = rectSize * rectSize * 4;
	disDc_ = ::CreateDC(L"DISPLAY", 0, 0, 0); //  创建一个DC(设备上下文)，第一个参数固定写法
	bitbltStartPoint_.x = 0;
	bitbltStartPoint_.y = 0;

	//位图信息
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo);
	bitmapInfo.bmiHeader.biWidth = rectSize;
	bitmapInfo.bmiHeader.biHeight = rectSize;
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 32;
	bitmapInfo.bmiHeader.biSizeImage = rectSize * rectSize;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

	scrDc_ = ::GetWindowDC(hwnd_); //获取窗口DC
	memDc_ = ::CreateCompatibleDC(scrDc_); //缓冲内存DC
	bitmap_ = ::CreateDIBSection(memDc_, &bitmapInfo, DIB_RGB_COLORS, &bitsPtr_, nullptr, 0);

	if (bitmap_ == nullptr)
	{
		::DeleteDC(memDc_);
		::ReleaseDC(hwnd_, scrDc_);
		return false;
	}
	SelectObject(memDc_, bitmap_); // 将画布选入内存DC
	return true;
}

void DibCaptureHelper::Cleanup()
{
	if (bitmap_ == nullptr)
	{
		return;
	}

	//删除用过的对象
	::SelectObject(memDc_, oldBitmap_);
	::DeleteObject(bitmap_);
	::DeleteDC(memDc_);
	::ReleaseDC(hwnd_, scrDc_);
	disDc_ = nullptr;
	hwnd_ = nullptr;
	scrDc_ = nullptr;
	memDc_ = nullptr;
	bitmap_ = nullptr;
	oldBitmap_ = nullptr;
	bitsPtr_ = nullptr;
}

bool DibCaptureHelper::Capture(cv::Mat &img)
{
	if (bitmap_ == nullptr || memDc_ == nullptr || scrDc_ == nullptr)
	{
		return false;
	}

	clock_t start, finish;

	start = clock();
	
	// 将桌面截图画到画布上
	BitBlt(memDc_, 0, 0, rectsize_, rectsize_, disDc_, posx_, posy_, SRCCOPY); // 整个桌面截图就已经画到了memDc上
	
	BITMAP bmp;
	GetObject(bitmap_, sizeof(BITMAP), &bmp);
	img.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, 3));
	GetBitmapBits(bitmap_, bmp.bmHeight*bmp.bmWidth * 3, img.data);
	//std::cout << img.channels() << std::endl;
	//cv::imshow("img", img);
	//cv::waitKey(0);
	finish = clock();
	printf("%f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
	return true;
}