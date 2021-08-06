#pragma once
/**
* project : 物理和渲染模型
* file    : CommonHead.h
* Copyright 2021 handle All rights reserved.
* 缅怀袁隆平院士
**/
#include "CommonHead.h"
class Point;

class Figure {
private:
	double Num;
	static int CarelessMininum;
public:
	Figure Square() {
		return Num * Num;
	}
	Figure() :Num(0) {

	}
	Figure(double N) :Num(N) {

	}
	operator double() {
		return Num;
	}
	inline bool operator ==(const Figure &AnoNum) {
		if (Num - AnoNum.Num < CarelessMininum) {
			return true;
		}
		return false;
	}
	inline bool operator !=(const Figure& AnoNum) {
		if (*this == AnoNum) {
			return false;
		}
		return true;
	}
	inline bool operator >(const Figure& AnoNum) {
		if (Num > AnoNum.Num) {
			return true;
		}
		return false;
	}
	inline bool operator <(const Figure& AnoNum) {
		if (Num < AnoNum.Num) {
			return true;
		}
		return false;
	}
	inline bool operator >=(const Figure& AnoNum) {
		if (Num >= AnoNum.Num) {
			return true;
		}
		return false;
	}
	inline bool operator <=(const Figure& AnoNum) {
		if (Num <= AnoNum.Num) {
			return true;
		}
		return false;
	}
	inline void operator +=(const Figure AnoNum) {
		Num += AnoNum.Num;
	}
	inline void operator -=(const Figure& AnoNum) {
		Num -= AnoNum.Num;
	}
};

class Vector {
public:
	Figure x, y;
	Vector();
	Vector(Figure x_, Figure y_);
	Vector(const Vector& Vec);
	Figure length();
	Figure abs();
	double Angle();

	Vector operator +(Vector Vec);
	Vector operator -(Vector Vec);
	Vector operator *(Figure Right);
	Figure operator *(Vector Vec);
	Vector operator /(Figure Num);
};

class Line {
private:
	void FixRange();
public:
	bool Range;
	Figure RangeY1, RangeY2;
	Figure k, b;
	Line(Figure k_, Figure b_);
	Line(Vector& Vec);
	Line(Point& Pt, Vector& Vec);
	bool operator ==(Point& Pt);
	bool operator>(Point& Pt);
	bool operator<(Point& Pt);
	bool operator>=(Point& Pt);
	bool operator<=(Point& Pt);
};

class Point {
public:
	Figure x, y;
	Point();
	Point(Figure X, Figure Y);
	Point(const Point& Pt);

	Point operator +(Point& Pt);
	Point operator -(Point& Pt);
	Point operator *(Figure Num);

	//Windows support
	operator POINT();
	Point(const POINT& pt);
};

class Rect {
public:
	static int Segment;
	Point LT, RD;
	Rect(Figure x, Figure y,Figure Width,Figure Height);
	Rect(Vector& Vec);
	Rect(Point Pt, Vector Vec);
	bool operator ==(Point& Pt);
	bool operator ||(Line& L);
	bool operator &&(Rect& Rect);
	//windows SUPPORT
	Rect(RECT rect);
	operator RECT();
};