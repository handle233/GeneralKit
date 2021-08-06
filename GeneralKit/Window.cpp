/**
* project : 物理和渲染模型
* file    : Window.cpp
* Copyright 2021 handle All rights reserved.
* 缅怀袁隆平院士
**/
#include "Window.h"

Window::Window(HINSTANCE hInst,const WCHAR* ClassName, 
	const WCHAR* WindowName, int Width, int Height,WNDPROC WinProc, const WCHAR* Icon){
	WNDCLASSEX WndClass;
	memset(&WndClass, 0, sizeof(WNDCLASSEX));
	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = Icon == nullptr ? LoadIcon(NULL, IDI_APPLICATION) : (HICON)LoadImage(NULL, Icon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
	WndClass.hInstance = hInst;
	WndClass.lpszClassName = ClassName;
	WndClass.style = 0;
	WndClass.lpfnWndProc = WinProc;

	if (!RegisterClassEx(&WndClass)) {
		//exception here
		exit(-1);
	}

	hwnd = CreateWindow(ClassName, WindowName, WS_SYSMENU | WS_CAPTION, CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, nullptr, nullptr, hInst, nullptr);
	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);
}
void Window::MessageLoop(CyclingFunc CycFun){
	MSG msg = { 0 };
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg,nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			CycFun();
		}
	}
}
void Window::OpenConsole(){
	AllocConsole();

	long StdHandle = NULL, ConHandle = NULL;
	FILE* fp = NULL;

	//bind OUT
	StdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	ConHandle = _open_osfhandle(StdHandle, _O_TEXT);
	fp = _fdopen(ConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
	//bind IN
	StdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	ConHandle = _open_osfhandle(StdHandle, _O_TEXT);
	fp = _fdopen(ConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	//bind ERR
	StdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	ConHandle = _open_osfhandle(StdHandle, _O_TEXT);
	fp = _fdopen(ConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	printf("C Output Initalize completed.\n");
	cout << "C++ Output Initalize completed." << endl;
#pragma message("如果需要使用命令行需要添加参数，参见OpenConsole函数")
	/*
	项目 -> 属性 -> 生成事件 ->后期生成事件 -> 命令行” 中添加：
	editbin /SUBSYSTEM:CONSOLE $(OutDir)\$(ProjectName).exe
	*/
}

void WindowProcess::KeyDownProc(WPARAM wpm, LPARAM lpm){
	if (!Keys[wpm].Down) {
		Keys[wpm].Hit = true;
	}
	Keys[wpm].Down = true;
}
void WindowProcess::KeyUpProc(WPARAM wpm, LPARAM lpm){
	Keys[wpm].Hit = false;
	Keys[wpm].Down = false;
}
void WindowProcess::MouseDownProc(UINT MouseMessage){
	sKeyState* pConKey = nullptr;
	if (MouseMessage == WM_LBUTTONDOWN) {
		pConKey = &LeftButton;
	}
	else if (MouseMessage == WM_RBUTTONDOWN) {
		pConKey = &RightButton;
	}
	else {
		return;
	}
	if (!pConKey->Down) {
		pConKey->Hit = true;
	}
	pConKey->Down = true;
}
void WindowProcess::MouseUpProc(UINT MouseMessage){
	sKeyState* pConKey = nullptr;
	if (MouseMessage == WM_LBUTTONUP) {
		pConKey = &LeftButton;
	}
	else if (MouseMessage == WM_RBUTTONUP) {
		pConKey = &RightButton;
	}
	else {
		return;
	}
	pConKey->Hit = false;
	pConKey->Down = false;
}
bool WindowProcess::WindowMessageFilter(HWND hwnd, UINT msg, WPARAM wpm, LPARAM lpm){
	this->hwnd = hwnd;
	switch (msg) {
	case WM_KEYDOWN:
		KeyDownProc(wpm, lpm);
		break;
	case WM_KEYUP:
		KeyUpProc(wpm, lpm);
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		MouseDownProc(msg);
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		MouseUpProc(msg);
		break;
	case WM_MOUSEWHEEL:
		Wheel = HIWORD(wpm);
		break;
	default:
		return false;
	}
	return true;
}
bool WindowProcess::IsKeyDown(int code)
{
	return Keys[code].Down;
}
bool WindowProcess::IsKeyHit(int code)
{
	register bool tmp = Keys[code].Hit;
	Keys[code].Hit = false;
	return tmp;
}
bool WindowProcess::IsButtonHit(bool IsRight)
{
	if (IsRight) {
		register bool tmp = RightButton.Hit;
		RightButton.Hit = false;
		return tmp;
	}
	else {
		register bool tmp = LeftButton.Hit;
		LeftButton.Hit = false;
		return tmp;
	}
}
bool WindowProcess::IsButtonDown(bool IsRight)
{
	return IsRight ? RightButton.Down : LeftButton.Down;
}
int WindowProcess::IsRoll()
{
	if (Wheel < 0) {
		Wheel+=120;
		return -1;
	}
	if (Wheel > 0) {
		Wheel-=120;
		return 1;
	}
	return 0;
}
Point WindowProcess::GetMousePos()
{
	if (hwnd == nullptr) {
		throw Exception(EXPT_ERROR, "在使用WindowProcess GetMousePos时，hwnd未初始化");
	}
	POINT Pt;
	GetCursorPos(&Pt);
	ScreenToClient(hwnd, &Pt);
	return Pt;
}
RECT WindowProcess::GetRect()
{
	if (hwnd == nullptr) {
		return RECT();
	}
	RECT rect;
	GetClientRect(hwnd, &rect);
	return rect;
}
void nullproc(){ }

Screen::Screen(HWND hwnd):rect(Client),Win(hwnd)
{
	Alpha = 255;
	dc = GetDC(hwnd);
	SetStretchBltMode(dc, HALFTONE);
	GetClientRect(hwnd, &Client);
	Bitmap = nullptr;
}
Screen::Screen(Screen& Scr):rect(Scr.rect)
{
	Win = nullptr;
	Alpha = 255;
	Client = { 0 };
	Client.top = -1;
	dc = CreateCompatibleDC(Scr.dc);
	SetStretchBltMode(dc, HALFTONE);
	Bitmap = CreateCompatibleBitmap(Scr.dc, rect.right, rect.bottom);
	SelectObject(dc, Bitmap);
}
Screen::Screen(Screen& Scr, ::Bitmap& Pict) :rect(Client)
{
	Win = nullptr;
	Alpha = 255;
	Bitmap = nullptr;

	Client.right = Pict.Width;
	Client.top = -1;
	Client.bottom = Pict.Height;

	dc = CreateCompatibleDC(Scr.dc);
	SelectObject(dc, Pict.Photo);
}
Screen::~Screen()
{
	if (Win) {
		ReleaseDC(Win, dc);
	}
	else {
		DeleteDC(dc);
	}
	if (Bitmap) {
		DeleteObject(Bitmap);
	}
	dc = nullptr;
	Bitmap = nullptr;
}
void Screen::Clear(unsigned Colorgb)
{
	HBRUSH hBrush = CreateSolidBrush(Colorgb);
	FillRect(dc, &rect, hBrush);
	DeleteObject(hBrush);
}
void Screen::Resize()
{
	if (Client.top == -1) {
		return;
	}
	if (Win != nullptr) {
		GetClientRect(Win, &Client);
		return;
	}
	DeleteObject(Bitmap);
	Bitmap = CreateCompatibleBitmap(dc, rect.right, rect.bottom);
	SelectObject(dc, Bitmap);
}
void Screen::operator<<(Screen& Scr)
{
	if (Scr.Alpha == 255) {
		BitBlt(dc, 0, 0, rect.right, rect.bottom, Scr.dc, 0, 0, SRCCOPY);
	}
	else {
		BLENDFUNCTION bf = { 0 };
		bf.BlendOp = AC_SRC_OVER;
		bf.SourceConstantAlpha = Scr.Alpha;
		AlphaBlend(dc, 0, 0, rect.right, rect.bottom,
			Scr.dc, 0, 0, rect.right, rect.bottom, bf);
	}
}
void Screen::operator+=(Screen& Scr)
{
	if (Scr.Alpha == 255) {
		StretchBlt(dc, 0, 0, rect.right, rect.bottom, Scr.dc, 0, 0, Scr.rect.right, Scr.rect.bottom, SRCCOPY);
	}
	else {
		BLENDFUNCTION bf = { 0 };
		bf.BlendOp = AC_SRC_OVER;
		bf.SourceConstantAlpha = Scr.Alpha;
		AlphaBlend(dc, 0, 0, rect.right, rect.bottom, Scr.dc, 0, 0, Scr.rect.right, Scr.rect.bottom,
			bf);
	}
}
void Screen::ReFormatBitmap(::Bitmap& Picture, int NewWidth, int NewHeight)
{
	Screen Memory(*this);
	Screen Select(*this, Picture);
	HBITMAP hTemp = Memory.Bitmap;
	HBITMAP hMem = CreateCompatibleBitmap(Memory.dc, NewWidth, NewHeight);
	SelectObject(Memory.dc, hMem);

	StretchBlt(Memory.dc, 0, 0, NewWidth, NewHeight,
		Select.dc, 0, 0, Picture.OriginWidth, Picture.OriginHeight, SRCCOPY);
	
	DeleteObject(Picture.Photo);
	Picture.Photo = hMem;
	SelectObject(Memory.dc, hTemp);
	Memory.Bitmap = hTemp;

	Picture.OriX = NewWidth;
	Picture.OriY = NewHeight;
}
void Screen::operator+(::Bitmap& Picture)
{
	Screen Select(*this,Picture);
	if ((Picture.Transrgb == -1)&&(Picture.Alpha == 255)) {
		BitBlt(dc, Picture.x, Picture.y, Picture.Width, Picture.Height, Select.dc, 0, 0, SRCCOPY);
	}
	else if(Picture.Transrgb != -1 && Picture.Alpha == 255) {
		TransparentBlt(dc, Picture.x, Picture.y, Picture.Width, Picture.Height,
			Select.dc, 0, 0, Picture.OriginWidth, Picture.OriginHeight, Picture.Transrgb);
	}
	else if (Picture.Transrgb == -1 && Picture.Alpha != 255) {
		BLENDFUNCTION bf = { 0 };
		bf.BlendOp = AC_SRC_OVER;
		bf.SourceConstantAlpha = Picture.Alpha;
		AlphaBlend(dc, 0, 0, Picture.Width, Picture.Height,
			Select.dc, 0, 0, Picture.OriginWidth, Picture.OriginHeight, bf);
	}
	else if (Picture.Transrgb != -1 && Picture.Alpha != 255) {
		Screen Mem(*this);
		Mem << *this;
		TransparentBlt(Mem.dc, Picture.x, Picture.y, Picture.Width, Picture.Height,
			Select.dc, 0, 0, Picture.OriginWidth, Picture.OriginHeight, Picture.Transrgb);
		BLENDFUNCTION bf = { 0 };
		bf.BlendOp = AC_SRC_OVER;
		bf.SourceConstantAlpha = Picture.Alpha;
		AlphaBlend(dc, 0, 0, rect.right, rect.bottom,
			Mem.dc, 0, 0, rect.right, rect.bottom, bf);
	}
}
GraphObject::GraphObject(Screen& Scr):dc(Scr.dc)
{
	
}

Text::Text(Screen& Scr) : GraphObject(Scr)
{
	hFont = nullptr;
}
Text::~Text()
{
	if (hFont) {
		DeleteObject(hFont);
	}
	hFont = nullptr;
}

Bitmap::Bitmap(const TCHAR* sFile)
{
	Alpha = 255;
	Transrgb = -1;
	x = 0; y = 0;
	Photo = (HBITMAP)LoadImage(NULL, sFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	BITMAP hInfo = { 0 };
	GetObject(Photo, sizeof(BITMAP), &hInfo);

	OriX = hInfo.bmWidth;
	OriY = hInfo.bmHeight;
	Width = hInfo.bmWidth;
	Height = hInfo.bmHeight;
}
Bitmap::~Bitmap()
{
	DeleteObject(Photo);
}

FPSCounter::FPSCounter() :TCounter(1000) {
	LastFPS = 0;
	FPS = 0;
}
int FPSCounter::UpdateFps() {
	if (TCounter) {
		LastFPS = FPS;
		FPS = 0;
	}
	FPS++;
	return LastFPS;
}

DWORD WINAPI ThreadPoolGlobalProc(PVOID pPool)
{
	ThreadPool* p_Pool = (ThreadPool*)pPool;
	return p_Pool->ThreadPoolProc();
}

DWORD ThreadPool::ThreadPoolProc()
{
	while (!Quit) {
		Sync.Lock();
		if (Missions.length > 0) {
			ProMission Mis = Missions[0];
			Missions.Del(0);
			Sync.Free();
			//todo

			int Index = -1;
			for (int a = 0; a < ProList.length; a++) {
				if (ProList[a].ProID == Mis.ProID) {
					Index = a;
				}
			}
			if (Index == -1) {
				throw InvalidPro();
				continue;
			}
			ProList[Index].Pro(Mis.pData);

		}
		else {
			Sync.Free();
		}
	}
	return 0;
}

ThreadPool::ThreadPool(int ThreadNum)
{
	Quit = false;
	ThreadList = new HANDLE[ThreadNum];
	for (int a = 0; a < ThreadNum; a++) {
		ThreadList[a] = CreateThread(nullptr,0,ThreadPoolGlobalProc,this,
			0,nullptr);
	}
	ThreadCounter = ThreadNum;
}

ThreadPool::~ThreadPool()
{
	Quit = true;
	for (int a = 0; a < ThreadCounter; a++) {
		WaitForSingleObject(ThreadList[a], INFINITE);
	}
}

int ThreadPool::GetMissionNum()
{
	return Missions.length;
}

void ThreadPool::Suspend()
{
	for (int a = 0; a < ThreadCounter; a++) {
		SuspendThread(ThreadList[a]);
	}
}
void ThreadPool::Resume()
{
	for (int a = 0; a < ThreadCounter; a++) {
		ResumeThread(ThreadList[a]);
	}
}
void ThreadPool::RegisterProc(int ProID, void (*Pro)(void*))
{
	ProSel APro;
	APro.ProID = ProID;
	APro.Pro = Pro;
	ProList.Add(-1,APro);
}
void ThreadPool::GiveProcess(int ProID, void* pData)
{
	ProMission AMis;
	AMis.ProID = ProID;
	AMis.pData = pData;
	Missions.Add(-1, AMis);
}
