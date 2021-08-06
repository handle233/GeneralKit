#pragma once
/**
* project : �������Ⱦģ��
* file    : ReferenceFrame.h
* Copyright 2021 handle All rights reserved.
* �廳Ԭ¡ƽԺʿ
**/
#include "CommonHead.h"

class ReferenceConvertor {
public:
	enum AxisDirection {
		Negative = -1,
		Positive = 1
	};
	Point OriginOffset;
	double Magnif;
	AxisDirection SourceDirX, DestinDirX,SourceDirY, DestinDirY;
	ReferenceConvertor();
	Point Convert(Point Pt);
	Point Disconvert(Point Pt);
	Vector Resize(Vector Vec);
};