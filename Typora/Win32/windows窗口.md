![](C:\Users\ASUS\Pictures\博客图片\QQ图片20211123213350.png)

windows下的主函数


![](C:\Users\ASUS\Pictures\博客图片\QQ图片20211123213524.png)

## 正经窗口的创建步骤

- 注册窗口类
- 创建窗口
- 显示窗口
- 刷新窗口
- 消息循环
- 窗口的消息处理函数

~~~c
#pragma warning(disable:4996)
#include <stdio.h>
#include <windows.h>
//标准输出设备句柄
HANDLE hConsole;
int n;

//WM_CREATE 消息 的 处理函数
void on_create(HWND hWnd);

//6666 定时器的定时器处理函数
void timerProc();

//定时器消息处理函数
void OnTimer(WPARAM wParam);

//6 窗口的消息处理函数
LRESULT CALLBACK myWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WinMain(HINSTANCE hInst,
	HINSTANCE hPrevInst,
	char* cmd,
	int show) {

	//窗口类
	WNDCLASSEXA  wc = { 0 };
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = NULL;
	wc.hCursor = NULL;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = hInst;
	wc.lpfnWndProc = myWndProc;
	wc.lpszClassName = "强哥的窗口类";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	//1 注册窗口类
	RegisterClassEx(&wc);

	//2 创建窗口
	HWND hWnd = CreateWindowEx(wc.style,
		wc.lpszClassName, "第一个windows窗口",
		WS_OVERLAPPEDWINDOW, 100, 100, 500, 500,
		NULL, NULL, hInst, NULL);
	if (!hWnd)
	{
		return FALSE;
	}
	//3 显示窗口
	ShowWindow(hWnd, SW_SHOW);
	//4 刷新
	UpdateWindow(hWnd);

#if  1
	AllocConsole();
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	n = 0;
#endif

	//消息包
	MSG msg;

	bool ret = true;
	//5 消息循环
	while (ret) {
		//5.1 获取消息
		ret = GetMessage(&msg, NULL, 0, 0);
		//5.2 翻译消息
		TranslateMessage(&msg);
		//5.3 派发消息
		DispatchMessage(&msg);
	}
	MessageBox(NULL, "消息循环结束", "窗口", MB_OK);
	return 0;
}

//6 窗口的消息处理函数
LRESULT CALLBACK myWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
#if 1
	//MessageBox(hWnd, "欢迎一生平安", "什么梅", MB_OK);
	n++;
	char buff[256] = { 0 };
	sprintf(buff, "第%d个消息\n", n);
	DWORD writenLen = 0;
	WriteConsole(hConsole, buff, strlen(buff), &writenLen, NULL);
#endif
	int r;
	switch (msg)
	{
	case WM_TIMER:
		OnTimer(wParam);
		break;
	case WM_COMMAND:

		break;
	case WM_CREATE:	  on_create(hWnd);	break;
	case WM_DESTROY:
		while (1) {
			r = MessageBox(NULL, "想杀我呀", "box", MB_YESNO);
			if (r == IDYES) {
				MessageBox(NULL, "没门", "^o^", MB_OK);
			}
			else if (r == IDNO) {
				MessageBox(NULL, "不杀我，我自杀", "^o^", MB_OK);
				//发送退出消息
				//ShowWindow(hWnd, SW_SHOW);

				break;
			}
		}
	
		break;
	case WM_NCDESTROY:
		while (1) {
			r = MessageBox(NULL, "hhhe，我还在哦", "box", MB_YESNO);
			if (r == IDYES) {
				MessageBox(NULL, "没门", "^o^", MB_OK);
			}
			else if (r == IDNO) {
				MessageBox(NULL, "拜拜了你勒~", "^o^", MB_OK);
				/*PostQuitMessage(0);*///发送退出消息
				//ShowWindow(hWnd, SW_SHOW);

				break;
			}
		}
	case WM_QUIT:
		while (1) {
			r = MessageBox(NULL, "嘎嘎嘎，表哥我又出来了哦~", "box", MB_YESNO);
			if (r == IDYES) {
				MessageBox(NULL, "嘻嘻，使不得哦~", "^o^", MB_OK);
			}
			else if (r == IDNO) {
				MessageBox(NULL, "表哥我回去了哦~", "^o^", MB_OK);
				//PostQuitMessage(0);//发送退出消息
				//ShowWindow(hWnd, SW_SHOW);

				break;
			}
		}
#if 1
	case WM_CLOSE:
		
		SetWindowPos(hWnd, HWND_TOPMOST, 50, 50, 300, 300, NULL);
		MessageBox(NULL, "想关闭我呀", "box", MB_YESNO);
		break;
#endif
	default:

		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//WM_CREATE 消息 的 处理函数
void on_create(HWND hWnd) {
#if 1
	MessageBox(hWnd, "窗口创建成功!", "什么梅", MB_OK);

	char buff[256] = { 0 };
	sprintf(buff, "窗口创建成功!\n", n);
	WriteConsole(hConsole, buff, strlen(buff), NULL, NULL);
#endif

	//SetTimer(hWnd, 6666, 1000, (TIMERPROC)timerProc);//回调函数
	SetTimer(NULL, 8888, 1000, NULL);//定时器消息
	SetTimer(NULL , 9999, 1000, NULL);//定时器消息
}

//6666 定时器的定时器处理函数
void timerProc() {
	static int num = 0;
	char buff[256] = { 0 };
	sprintf(buff, "定时器6666:%d次调用!\n", num++);
	WriteConsole(hConsole, buff, strlen(buff), NULL, NULL);
}

//定时器消息处理函数
void OnTimer(WPARAM wParam) {
	char buff[256] = { 0 };

	switch (wParam) {
	case 8888:
		sprintf(buff, "定时器消息处理函数：%u!\n", wParam);
		WriteConsole(hConsole, buff, strlen(buff), NULL, NULL);
		break;
	case 9999:

		sprintf(buff, "定时器消息处理函数：%u!\n", wParam);
		WriteConsole(hConsole, buff, strlen(buff), NULL, NULL);
		break;
	}
}
~~~

知识点：

- 当点击右上角关闭按钮是就会产生VM_CLOSE这个消息，如果不自己return 而是用系统默认的窗口处理函数DefWindowProc()函数，这个函数自己会发送一系列消息VM_CLOSE-->VM_DESTROY-->VM_NCDESTROY-->VM_QUIT-->VM_CLOSE-->程序结束(上边代码运行证明了过程，可以自己copy运行一下)

- 如果直接在VM_CLOSE紧接着使用PostWindowProc（0）；会出现问题，正确用法是将PostWindowProc(0)放在VM_DESTROY消息里使用,这个函数被调用后会产生一个VM_QUIT的消息，这个消息无法被捕获，并且返回值是0，循环就会结束了。

