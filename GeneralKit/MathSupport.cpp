#pragma once
/**
* project : 物理和渲染模型
* file    : CommonHead.h
* Copyright 2021 handle All rights reserved.
* 缅怀袁隆平院士
**/
#include "MathSupport.h"

int Figure::CarelessMininum = 0.001;

Point::Point() { x = 0.0, y = 0.0; }
Point::Point(Figure X, Figure Y) :x(X), y(Y) { }
Point::Point(const Point& Pt) {
	x = Pt.x, y = Pt.y;
}
Point Point::operator +(Point& Pt) {
	Point a(x + Pt.x, y + Pt.y);
	return a;
}
Point Point::operator -(Point& Pt) {
	Point a(x - Pt.x, y - Pt.y);
	return a;
}
Point Point::operator*(Figure Num)
{
	Point a(x * Num, y * Num);
	return a;
}
Point::operator POINT() {
	POINT pt;
	pt.x = x, pt.y = y;
	return pt;
}
Point::Point(const POINT& pt) {
	x = pt.x;
	y = pt.y;
}

Vector::Vector() { x = 0.0, y = 0.0; }
Vector::Vector(Figure x_, Figure y_) :x(x_), y(y_) { }
Vector::Vector(const Vector& Vec) {
	x = Vec.x, y = Vec.y;
}
Figure Vector::length() {
	return sqrt(::abs(x) * ::abs(x) + ::abs(y) * ::abs(y));
}
Figure Vector::abs() {
	if ((x * x + y * y) == 0) {
		return 0;
	}
	return sqrt(x * x + y * y);
}
double Vector::Angle()
{
	return atan2(y,x);
}
Vector Vector::operator +(Vector Vec) {
	Vector Ret(x + Vec.x, y + Vec.y);
	return Ret;
}
Vector Vector::operator -(Vector Vec) {
	Vector Ret(x - Vec.x, y - Vec.y);
	return Ret;
}
Vector Vector::operator *(Figure Right) {
	Vector a(x * Right, y * Right);
	return a;
}
Figure Vector::operator *(Vector Vec) {
	return x * Vec.x + y * Vec.y;
}
Vector Vector::operator /(Figure Num) {
	Vector A(*this);
	A.x = A.x / Num;
	A.y = A.y / Num;
	return A;
}

Line::Line(Figure k_, Figure b_) : k(k_), b(b_) {
	RangeY1 = 0; RangeY2 = 0;
	Range = false;
}
Line::Line(Vector& Vec) {
	b = 0;
	k = Vec.y / Vec.x;
	RangeY1 = 0; RangeY2 = Vec.y;
	Range = true;
	FixRange();
}
Line::Line(Point& Pt, Vector& Vec) :Line(Vec) {
	b = Pt.y-Pt.x * k;

	RangeY1 = Pt.y; RangeY2 = Pt.y+Vec.y;
	Range = true;
	FixRange();
}
void Line::FixRange()
{
	if (!Range) {
		return;
	}
	if (RangeY1 > RangeY2) {
		Figure tmp = RangeY1;
		RangeY1 = RangeY2;
		RangeY2 = tmp;
	}
}
bool Line::operator ==(Point& Pt) {
	if (k * Pt.x + b == Pt.y && (!Range || (Pt.y >= RangeY1 && Pt.y <= RangeY2))) {
		return true;
	}
	return false;
}
bool Line::operator >(Point& Pt) {
	if (((Pt.y - b) / k > Pt.x) && (!Range || (Pt.y >= RangeY1 && Pt.y <= RangeY2))) {
		return true;
	}
	return false;
}
bool Line::operator <(Point& Pt) {
	if (((Pt.y - b) / k < Pt.x) && (!Range || (Pt.y >= RangeY1 && Pt.y <= RangeY2))) {
		return true;
	}
	return false;
}
bool Line::operator >=(Point& Pt) {
	if (((Pt.y - b) / k >= Pt.x) && (!Range || (Pt.y >= RangeY1 && Pt.y <= RangeY2))) {
		return true;
	}
	return false;
}
bool Line::operator <=(Point& Pt) {
	if (((Pt.y - b) / k <= Pt.x) && (!Range || (Pt.y >= RangeY1 && Pt.y <= RangeY2))) {
		return true;
	}
	return false;
}

int Rect::Segment = 10;
Rect::Rect(Figure x, Figure y,Figure Width,Figure Height):Rect(Point(x,y),Vector(Width,Height))
{
	
}
Rect::Rect(Vector& Vec) : Rect(Point(), Vec) {}
Rect::Rect(Point Pt, Vector Vec) : LT(Pt.x, Pt.y), RD(Vec.x, Vec.y) {
	if (Vec.x < Figure(0.0)) {
		LT.x = Pt.x + Vec.x;
		RD.x = Pt.x;
	}
	else {
		LT.x = Pt.x;
		RD.x = Pt.x + Vec.x;
	}
	if (Vec.y < Figure(0.0)) {
		LT.y = Pt.y;
		RD.y = Pt.y + Vec.y;
	}
	else {
		LT.y = Pt.y + Vec.y;
		RD.y = Pt.y;
	}
}
bool Rect::operator==(Point& Pt)
{
	if (Pt.x > LT.x && Pt.x < RD.x) {
		if (Pt.y<LT.y && Pt.y>RD.y) {
			return true;
		}
	}
	return false;
}
bool Rect::operator||(Line& L)
{
	for (Figure i = LT.x; i < RD.x; i += (RD.x - LT.x) / Segment) {
		Figure y = L.k * i + L.b;
		if (y > RD.y && y < LT.y) {
			return true;
		}
	}
	
	return false;
}
inline bool CheckRanging(Point& pt1, Point& pt2, Point& CheckPt) {
	if ((pt1.y >= CheckPt.y && pt2.y <= CheckPt.y) &&
		(pt1.x <= CheckPt.x && pt2.x >= CheckPt.x)) {
		return true;
	}
	return false;
}
bool Rect::operator&&(Rect& Rect)
{
	Point RT(RD.x, LT.y);
	Point LD(LT.x, RD.y);
	if (CheckRanging(Rect.LT, Rect.RD, LT)) {
		return true;
	}
	if (CheckRanging(Rect.LT, Rect.RD, RT)) {
		return true;
	}
	if (CheckRanging(Rect.LT, Rect.RD, LD)) {
		return true;
	}
	if (CheckRanging(Rect.LT, Rect.RD, RD)) {
		return true;
	}
	return false;
}
Rect::Rect(RECT rect):Rect(Point(rect.left,rect.top),Vector(rect.right,rect.bottom))
{

}
Rect::operator RECT()
{
	RECT rect;
	rect.left = LT.x;
	rect.top = LT.y;
	rect.right = RD.x;
	rect.bottom = RD.y;
	return rect;
}

