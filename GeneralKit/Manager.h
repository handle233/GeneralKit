#pragma once
/**
* project : 物理和渲染模型
* file    : Manager.h
* Copyright 2021 handle All rights reserved.
* 缅怀袁隆平院士
**/
#include "CommonHead.h"
#include "Container.h"
#include "Window.h"


class Module {
	friend class Manager;
private:
	int IdentifyCode;
	int UsingCount;
public:
	Module(int ID):IdentifyCode(ID){
		UsingCount = 0;
	}
	virtual~Module() { }
	virtual void Main() = 0;
	virtual void Timer(){ }
};

class ModuleOverflow :public Exception {
public:
	ModuleOverflow() :Exception(EXPT_ERROR, "The mod list has full."){ }
};
class ModuleDefined :public Exception {
public:
	ModuleDefined() :Exception(EXPT_ERROR, "The mod ID has been defined.") { }
};

DWORD WINAPI TimerThreadGlobalProc(LPVOID pManager);
class Manager {
	friend DWORD WINAPI TimerThreadGlobalProc(LPVOID pManager);
private:
	HANDLE hThread;
	bool Exit;

	ThreadSync Sync;

	Module* Mods[16];
	int ModOverIndex;
	Module* RunMods[8];
	int RunModOverIndex;
	Module* TimeMods[8];
	int TimeModOverIndex;

	DWORD TimerSingleProc();
public:
	Manager();
	~Manager();
	void AddMod(Module* NewMod);
	int SeekMod(int ID);
	void AddTimeMod(int Index);
	bool FreeTimeMod(int Code);
	void AddRunMod(int Index);
	bool FreeRunMod(int Code);
	void gc();

	void Run();
};
