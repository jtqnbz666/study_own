GUI   :  Graphics  User   Interface
				   用户
GDI   :  Graphics  Device Interface
				   设备
显卡
HDC      Handle of Device Context
		  句柄       设备  上下文

Graphics  图形界面 
	目之所及   

显示器：
	像素 

绘图：
	一个个像素去画
	像素制游戏：魂斗罗

	一个个点去画：画笔     画笔设备
		创建画笔：CreatePen
		画笔交给画家： 
		画家画画：
	一片一片去画：画刷     画刷设备
		创建画刷：CreateSolidBrush CreateHatchBrush 
		画刷交给画家： 
		画家画画：

贴图： 文件形式存在
	BMP   JPG  JPEG  PNG  

	贴图游戏：DNF  





描述像素的协议：
	RGB协议：4个字节来描述一个像素
#define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))		
	RRGGBB
	RRGGBBAA


BegPaint  EndPaint

GetDC   ReleaseDC


贴图：
1. 加载图片     文件 ---> 内存
	LoadImage
		LR_LOADFROMFILE		图片文件加载
		LR_LOADTRANSPARENT  资源方式加载
2. 创建兼容dc
3. 把图片设置到兼容dc中
4. 把兼容dc中的图片绘制到hdc中

多级缓存：
	1. 加载图片
	2. 创建兼容位图   CreateCompatibleBitmap
	3. 根据需要创建兼容dc   dc1 dc2 dc3
	4. 把兼容位图设置到兼容dc2中
	5. 把图片1 放到兼容dc1中，把兼容dc1中的图片画到兼容dc2中
	6. 把图片2 放到兼容dc1中，把兼容dc1中的图片画到兼容dc2中
	。。。。。。
	7. 把兼容dc2中的图片画到dc中