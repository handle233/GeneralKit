/**
* project : 物理和渲染模型
* file    : Main.cpp
* Copyright 2021 handle All rights reserved.
* 缅怀袁隆平院士
**/
#include "Manager.h"

DWORD WINAPI TimerThreadGlobalProc(LPVOID pManager) {
	Manager* pMan = (Manager*)pManager;
	return pMan->TimerSingleProc();
}

DWORD Manager::TimerSingleProc() {
	while (!Exit) {
		Sync.Lock();
		for (int a = 0; a < TimeModOverIndex; a++) {
			TimeMods[a]->Timer();
		}
		Sync.Free();
	}
	return 0;
}
Manager::Manager() {
	Exit = false;

	memset(Mods, 0, sizeof(Module*) * 16);
	memset(RunMods, 0, sizeof(Module*) * 8);
	memset(TimeMods, 0, sizeof(Module*) * 8);
	ModOverIndex = 0;
	RunModOverIndex = 0;
	TimeModOverIndex = 0;

	hThread = CreateThread(nullptr, 0, TimerThreadGlobalProc, this, THREAD_SUSPEND_RESUME, nullptr);
}
Manager::~Manager() {
	Exit = true;
	WaitForSingleObject(hThread, INFINITE);

	for (int a = 0; a < ModOverIndex; a++) {
		delete Mods[a];
	}
}
void Manager::AddMod(Module* NewMod) {
	if (ModOverIndex >= 16) {
		throw ModuleOverflow();
	}
	if (SeekMod(NewMod->IdentifyCode) != -1) {
		throw ModuleDefined();
	}
	Mods[ModOverIndex] = NewMod;
	ModOverIndex++;
}
int Manager::SeekMod(int ID) {
	for (int a = 0; a < ModOverIndex; a++) {
		if (Mods[a]->IdentifyCode == ID) {
			return a;
		}
	}
	return -1;
}
void Manager::AddTimeMod(int Index) {
	if (TimeModOverIndex >= 8) {
		throw ModuleOverflow();
	}
	TimeMods[TimeModOverIndex] = Mods[Index];
	Mods[Index]->UsingCount++;
	TimeModOverIndex++;

	if (TimeModOverIndex > 0) {
		ResumeThread(hThread);
	}
}
bool Manager::FreeTimeMod(int Code) {
	int Index = -1;
	for (int a = 0; a < TimeModOverIndex; a++) {
		if (TimeMods[a]->IdentifyCode == Code) {
			Index = a;
		}
	}
	if (Index == -1) {
		return false;
	}
	TimeMods[Index]->UsingCount--;
	memcpy(TimeMods + Index, TimeMods + Index + 1, sizeof(Module*) * (TimeModOverIndex - Index));
	TimeModOverIndex--;
	return true;
}
void Manager::AddRunMod(int Index) {
	if (RunModOverIndex >= 8) {
		throw ModuleOverflow();
	}
	RunMods[RunModOverIndex] = Mods[Index];
	Mods[Index]->UsingCount++;
	RunModOverIndex++;
}
bool Manager::FreeRunMod(int Code) {
	int Index = -1;
	for (int a = 0; a < RunModOverIndex; a++) {
		if (RunMods[a]->IdentifyCode == Code) {
			Index = a;
		}
	}
	if (Index == -1) {
		return false;
	}
	RunMods[Index]->UsingCount--;
	memcpy(RunMods + Index, RunMods + Index + 1, sizeof(Module*) * (RunModOverIndex - Index));
	RunModOverIndex--;
	return true;
}
void Manager::gc() {
	Sync.Lock();
	for (int a = 0; a < ModOverIndex; a++) {
		if (Mods[a]->UsingCount <= 0) {
			delete Mods[a];
			memcpy(Mods + a, Mods + a + 1, sizeof(Module*) * (ModOverIndex - a));
			ModOverIndex--;
		}
	}
	Sync.Free();
}
void Manager::Run() {
	Sync.Lock();
	for (int a = 0; a < RunModOverIndex; a++) {
		RunMods[a]->Main();
	}
	Sync.Free();
}
