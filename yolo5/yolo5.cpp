#ifndef YOLO5_H_
#define YOLO5_H_
#include "ScreenShot.h"
#include <thread>
#include "detect.h"
#include "windows.h"
#include "libloaderapi.h"
#include <iostream>
#include <mutex>
#include <atomic>
typedef int(*pDD_btn)(int btn);
typedef int(*pDD_whl)(int whl);
typedef int(*pDD_key)(int keycode, int flag);
typedef int(*pDD_mov)(int x, int y);
typedef int(*pDD_str)(char *str);
typedef int(*pDD_todc)(int vk);
typedef int(*pDD_movR)(int dx, int dy);

pDD_btn      DD_btn;          //Mouse button
pDD_whl      DD_whl;		     //Mouse wheel
pDD_key      DD_key;		     //Mouse move abs.
pDD_mov    DD_mov;		 //Mouse move rel.
pDD_str       DD_str;			 //Keyboard
pDD_todc    DD_todc;		 //Input visible char
pDD_movR   DD_movR;	     //VK to ddcode
HMODULE hDll = nullptr;
int InitMouseHid()
{
	hDll = LoadLibraryW(L"DDHID64.dll");    //App x64
	if (hDll == nullptr) {
		std::cout << "load mouse hid fail " << std::endl;
		return -1;
	}
	DD_btn = (pDD_btn)GetProcAddress(hDll, "DD_btn");
	DD_whl = (pDD_whl)GetProcAddress(hDll, "DD_whl");
	DD_key = (pDD_key)GetProcAddress(hDll, "DD_key");
	DD_mov = (pDD_mov)GetProcAddress(hDll, "DD_mov");
	DD_str = (pDD_str)GetProcAddress(hDll, "DD_str");
	DD_todc = (pDD_todc)GetProcAddress(hDll, "DD_todc");
	DD_movR = (pDD_movR)GetProcAddress(hDll, "DD_movR");

	if (!(DD_btn && DD_whl && DD_key && DD_mov && DD_str  && DD_todc && DD_movR)) {
		std::cout << "Initialize mouse hid fail " << std::endl;
		return -2;
	}
	int st = DD_btn(0);
	if (st != 1) {
		//DD Initialize Error
		std::cout << "Initialize mouse hid fail " << std::endl;
		return st;
	}

	return 1;
}

void UnInitMouseHid()
{
	if (hDll != nullptr) {
		FreeLibrary(hDll);
	}
}

//#include<cstring>
SYSTEMTIME systime;
// W 87 V 86 f10 121 L 76
HHOOK hook = nullptr;
DibCaptureHelper shotHelp;
std::recursive_mutex mut;
std::atomic<bool> sigSwitch = false;
std::atomic<bool> ft = false;

void ShotAndDet()
{
	while (true) {
		std::lock_guard<std::recursive_mutex> lk(mut);
		if (!sigSwitch.load()) {
			Sleep(40);
			continue;
		}
		//clock_t start, finish;
		//start = clock();
		cv::Mat img;
		cv::Rect rect[10];
		float conf[10] = { 0 };
		float classId[10] = { 0 };
		int count = 0;
		shotHelp.Capture(img);

		Detect(rect, conf, classId, img, count);

		if (count == 0) {
			continue;
		}
		int lastMovePosX = 0, lastMovePosY = 0;
		int centerX = 0, centerY = 0;
		cv::Point post;
		float lastDistance = 99 * 99;
		for (int i = 0; i < count; i++) {
			if (conf[i] <= 0.55 || (int)classId[i] == ft.load()) {
				continue;
			}

			post = cv::Point(shotHelp.posx_ + rect[i].tl().x + rect[i].width / 2,
				shotHelp.posy_ + rect[i].tl().y + rect[i].height / 2);
			centerX = shotHelp.posx_ + shotHelp.rectsize_ / 2;
			centerY = shotHelp.posy_ + shotHelp.rectsize_ / 2;
			cv::Point pos = cv::Point(post.x - (shotHelp.posx_ + shotHelp.rectsize_ / 2),
				post.y - (shotHelp.posy_ + shotHelp.rectsize_ / 2));
			if ((pos.x * pos.x + pos.y * pos.y) < lastDistance) {
				lastDistance = pos.x * pos.x + pos.y * pos.y;
				lastMovePosX = pos.x;
				lastMovePosY = pos.y;
			}
		}
		if (lastDistance == 99 * 99) {
			continue;
		}
		if (fabs(lastMovePosX) <= 1 && fabs(lastMovePosY) <= 1) {
			continue;
		}
		// std::cout << "move" << lastMovePos.x  << " " << lastMovePos.y << std::endl;
		//std::thread([=]() {DD_movR(lastMovePos.x, lastMovePos.y); }).detach();
		//DD_mov(centerX, centerY);
		//Sleep(1000);
		std::thread([=]() {DD_movR(lastMovePosX * 1.9, lastMovePosY * 1.9); }).detach();
		/*int cx = post.x;
		int cy = post.y;
		Sleep(1000);
		DD_mov(centerX, centerY);
		Sleep(1000);
		DD_mov(cx, cy);*/

		//finish = clock();
		//printf("%f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
	}

}

LRESULT CALLBACK Proc(int code, WPARAM w, LPARAM l) {
	KBDLLHOOKSTRUCT *T = (KBDLLHOOKSTRUCT*)l;
	DWORD x = T->vkCode;
	if (w == WM_KEYDOWN) {
		switch (x) {
		case 76:
			ft.store(!ft);
			break;
		case 86:
			//std::cout << "down V" << std::endl;
			if (!sigSwitch.load()) {
				sigSwitch.store(true);
			}
			break;
		case 121:
			std::cout << "down F10" << std::endl;
			UnInitMouseHid();
			UnInitDet();
			UnhookWindowsHookEx(hook);
			exit(0);
			break;
		}
	}
	else if (w == WM_KEYUP) {
		switch (x) {
		case 86:
			//std::cout << "up v" << std::endl;
			sigSwitch.store(false);
			break;
		case 121:
			//std::cout << "up f10" << std::endl;
			break;
		}
	}
	return CallNextHookEx(NULL, code, w, l);
}

int main() {

	InitMouseHid();
	UnInitMouseHid();
	InitDet();
	shotHelp.Init(320);
	std::thread(ShotAndDet).detach();
	hook = ::SetWindowsHookEx(WH_KEYBOARD_LL, Proc, 0, 0);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
#endif