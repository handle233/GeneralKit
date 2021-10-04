#pragma once
/**
* project : 物理和渲染模型
* file    : CommonHead.h
* Copyright 2021 handle All rights reserved.
* 缅怀袁隆平院士
**/
#include "CommonHead.h"


class Figure {
private:
	double Number;
public:
	Figure() {
		Number = 0;
	}
	Figure(double N) {
		Number = N;
	}
	operator double(){
		return Number;
	}
	Figure operator +(Figure A) {
		return Number + A.Number;
	}
	Figure operator -(Figure A) {
		return Number - A.Number;
	}
	Figure operator *(Figure A) {
		return Number * A.Number;
	}
	Figure operator /(Figure A) {
		return Number / A.Number;
	}
	Figure operator +=(Figure A) {
		return Number += A.Number;
	}
	Figure operator -=(Figure A) {
		return Number -= A.Number;
	}
	Figure operator >(Figure A) {
		return Number > A.Number;
	}
	Figure operator >=(Figure A) {
		return Number >= A.Number;
	}
	Figure operator <(Figure A) {
		return Number > A.Number;
	}
	Figure operator <=(Figure A) {
		return Number <= A.Number;
	}
	Figure operator ==(Figure A) {
		return Number == A.Number;
	}
};

class Vector {
public:
	Figure x, y;
	Vector(Figure xCoord, Figure yCoord) {
		x = xCoord, y = yCoord;
	}
	Figure abs() {
		return sqrt(x * x + y * y);
	}
	Vector Unit() {
		Vector tmp = Vector(x / abs(), y / abs());
		return tmp;
	}
	Vector operator +(const Vector &Vec) {
		Vector nVec(x + Vec.x, y + Vec.y);
		return nVec;
	}
	Vector operator -(const Vector& Vec) {
		Vector nVec(x - Vec.x, y - Vec.y);
		return nVec;
	}
	Figure operator *(const Vector& Vec) {
		return x * Vec.x + y * Vec.y;
	}
	Vector operator *(const Figure& Fig) {
		return Vector(x * Fig, y * Fig);
	}
};

class Point {
private:
	Vector p;
public:
	Figure x() { return p.x; }
	Figure y() { return p.y; }
	Point(Figure x, Figure y) :p(x, y) {
		
	}
	Point(POINT& pt) :p(pt.x, pt.y) {
		
	}

	operator POINT() {
		POINT pt;
		pt.x = p.x;
		pt.y = p.y;
		return pt;
	}
};

class Range {
public:
	Figure low, high;
	bool DisableLow, DisableHigh;

	void KeepLowHigh() {
		if (low > high) {
			Figure tmp = high;
			high = low;
			low = tmp;
		}
	}
	Range(Figure A, Figure B) {
		low = A, high = B;
		KeepLowHigh();//以防万一
		DisableHigh = DisableLow = false;
	}
	Range operator +(Figure TobeAdd) {
		if (TobeAdd > high) {
			high = TobeAdd;
		}
		else if (TobeAdd < low) {
			low = TobeAdd;
		}
		return *this;
	}
	bool operator &&(Figure N) {
		KeepLowHigh();//以防万一
		if ((N > low || DisableLow) && (N < high || DisableHigh)) {
			return true;
		}
		return false;
	}
};

class Line {
private:
	Figure k, b;
public:
	Range XRange;
	Line(Point pt, Vector Line) :XRange(pt.x(), pt.x() + Line.x) {
		k = Line.Unit().y / Line.Unit().x;
		b = pt.y() - k * pt.x();
	}
	bool operator &&(Line& Ano) {
		if ((k - Ano.k) == Figure(0)) {
			return false;
		}
		Figure X = (Ano.b - b) / (k - Ano.k);
		return XRange && X;
	}
	Figure operator &(Line& Ano) {
		Figure X = (Ano.b - b) / (k - Ano.k);
		return X;
	}
};