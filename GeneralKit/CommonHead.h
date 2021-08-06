#pragma once
/**
* project : 物理和渲染模型
* file    : CommonHead.h
* Copyright 2021 handle All rights reserved.
* 缅怀袁隆平院士
**/
#include <io.h>
#include <time.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <Windows.h>
#include <iostream>

#include "Defines.h"
#include "Exception.h"
#include "MathSupport.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"msimg32.lib")

#pragma warning(disable:4800)
#pragma warning(disable:4244)
#pragma warning(disable:4996)
#pragma warning(disable:26812)

using namespace std;

class ThreadSync {
private:
	CRITICAL_SECTION Section;
public:
	ThreadSync() {
		InitializeCriticalSection(&Section);
	}
	void Lock() {
		EnterCriticalSection(&Section);
	}
	void Free() {
		LeaveCriticalSection(&Section);
	}
	~ThreadSync() {
		DeleteCriticalSection(&Section);
	}
};
