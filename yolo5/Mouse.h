#ifndef MOUSE_H_
#define MOUSE_H_

/*typedef int(*pDD_btn)(int btn);
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
}*/
#endif