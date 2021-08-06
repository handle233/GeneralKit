#pragma once
/**
* project : 物理和渲染模型
* file    : Main.cpp
* Copyright 2021 handle All rights reserved.
* 缅怀袁隆平院士
**/
#include "CommonHead.h"
#include "MathSupport.h"
#include "Container.h"
#include "Window.h"

class Object;

void ColliCallFunc(void* pColliStr);
void SpeedCallFunc(void* pColliStr);

class Physics {
	friend void ColliCallFunc(void* pColliStr);
	friend void SpeedCallFunc(void* pColliStr);
private:
	GraphObject *Obj;
	int Tick;
	List<Object*> *Objs;
	bool ColliTendCheck(Object& a, Object& b);

	ThreadPool *Pool;
	struct ColliStr {
		Physics* It;
		Object* a;
		Object* b;
	};
	struct SpeedStr {
		Physics* It;
		Object* a;
		Object* b;
	};

public:
	void MultiThreadSupport(int ThreadNum);
	void SetDebugScreen(Screen* Scr);
	void Colli(Object& a, Object& b);
	void Gravi(Object& a, Object& b);
	const int& TickSpeed = Tick;
	Physics(int TickNum);
	void SetTickSpeed(int TickS) { Tick = TickS; }
	bool AddObject(Object* pObj);
	bool DelObject(Object* pObj);
	void PhysicsSpin();
};

class Object {
	friend class Physics;
public:
	Point& pt;
	Vector Speed;
	Vector Size;
	double Mass;
	Object(Point& Pos, Vector& Size_, double Mass_);
};