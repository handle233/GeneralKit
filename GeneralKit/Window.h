#pragma once
/**
* project : �������Ⱦģ��
* file    : CommonHead.h
* Copyright 2021 handle All rights reserved.
* �廳Ԭ¡ƽԺʿ
**/
#include "CommonHead.h"
#include "Container.h"

void nullproc();
class Window {
private:
	HWND hwnd;
	typedef void (*CyclingFunc)();
public:
	/*
	* Window�����������������ڵ�
	*/
	Window(HINSTANCE hInst, const WCHAR* ClassName, const WCHAR* WindowName, int Width, int Height, WNDPROC WinProc, const WCHAR* Icon = nullptr);
	/*
	* ������Ϣѭ��
	*/
	void MessageLoop(CyclingFunc CycFun = nullproc);
	/*
	* ���쿽������
	*/
	Window(const Window& Win) {
		hwnd = Win.hwnd;
	}
	/*
	* HWND to Window
	*/
	Window(HWND win) {
		hwnd = win;
	}
	/*
	* Window to HWND
	*/
	operator HWND() {
		return hwnd;
	}



	/*
	* �����򿪵��Դ���
	*/
	static void OpenConsole();
};

class WindowProcess {
private:
	HWND hwnd;
	struct sKeyState {
		bool Down;
		bool Hit;
	}Keys[255];
	sKeyState LeftButton, RightButton;
	short Wheel;
	void KeyDownProc(WPARAM wpm, LPARAM lpm);
	void KeyUpProc(WPARAM wpm, LPARAM lpm);
	void MouseDownProc(UINT MouseMessage);
	void MouseUpProc(UINT MouseMessage);
public:
	WindowProcess() {
		hwnd = NULL;
		memset(Keys, 0, sizeof(sKeyState) * 255);
		memset(&LeftButton, 0, sizeof(sKeyState));
		memset(&RightButton, 0, sizeof(sKeyState));
	}
	/*����ɸѡ��Ϣ*/
	bool WindowMessageFilter(HWND hwnd, UINT msg, WPARAM wpm, LPARAM lpm);
	/*���̰������*/
	bool IsKeyDown(int code);
	/*���̰������*/
	bool IsKeyHit(int code);
	/*��갴�����*/
	bool IsButtonHit(bool IsRight = false);
	/*��갴�����*/
	bool IsButtonDown(bool IsRight = false);
	/*���������*/
	int IsRoll();
	/*��ȡ���λ��*/
	Point GetMousePos();
	/*��ȡ���ھ���*/
	RECT GetRect();
};

class Text;
class Bitmap;
class Screen {
public:
	friend class GraphObject;
private:
	HWND Win;
	RECT& rect;
	RECT Client;
	HDC dc;
	HBITMAP Bitmap;
public:
	BYTE Alpha;
	Screen(HWND hwnd);
	Screen(Screen& Scr);
	Screen(Screen& Scr, ::Bitmap& Pict);
	~Screen();
	void Clear(unsigned Colorgb);
	void Resize();
	void operator << (Screen& Scr);
	void operator += (Screen& Scr);

	void ReFormatBitmap(::Bitmap& Picture,int NewWidth,int NewHeight);
	void operator + (::Bitmap& Picture);
};
class GraphObject {
protected:
public:
	HDC& dc;//debug
	GraphObject(Screen& Scr);
};

class Text :public GraphObject {
public:
	HFONT hFont;
	Text(Screen& Scr);
	~Text();
};
class Bitmap{
	friend class Screen;
private:
	int OriX, OriY;
public:
	const int& OriginWidth = OriX, & OriginHeight = OriY;
	int x,y,Width, Height;
	int Transrgb;
	BYTE Alpha;
	HBITMAP Photo;
	Bitmap(const TCHAR* sFile);
	~Bitmap();
};

class Timer {
private:
	unsigned Space;
	unsigned LstTime;
public:
	Timer(int TimeSpace) {
		Space = TimeSpace;
		LstTime = timeGetTime();
	}
	operator bool() {
		if (timeGetTime() - LstTime > Space) {
			LstTime = timeGetTime();
			return true;
		}
		return false;
	}
};


class FPSCounter {
private:
	Timer TCounter;
	int FPS;
public:
	int LastFPS;
	FPSCounter();
	int UpdateFps();
};

class InvalidPro :public Exception {
public:
	InvalidPro() :Exception(EXPT_ERROR, "�Ҳ���ID��Ӧ�Ĺ���") {

	}
};

DWORD WINAPI ThreadPoolGlobalProc(PVOID pPool);
class ThreadPool {
	friend DWORD WINAPI ThreadPoolGlobalProc(PVOID pPool);
private:
	DWORD ThreadPoolProc();
	struct ProSel{
		int ProID;
		void (*Pro)(void*);
	};
	struct ProMission {
		int ProID;
		void *pData;
	};
	ThreadSync Sync;
	List<ProSel> ProList;
	List<ProMission> Missions;
	HANDLE* ThreadList;
	int ThreadCounter;
	bool Quit;
public:
	ThreadPool(int ThreadNum);
	~ThreadPool();
	int GetMissionNum();
	void Suspend();
	void Resume();
	void RegisterProc(int ProID, void (*Pro)(void*));
	void GiveProcess(int ProID, void* pData);
};