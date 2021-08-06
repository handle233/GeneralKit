#include "ReferenceFrame.h"
/**
* project : �������Ⱦģ��
* file    : ReferenceFrame.cpp
* Copyright 2021 handle All rights reserved.
* �廳Ԭ¡ƽԺʿ
**/

ReferenceConvertor::ReferenceConvertor()
{
	Magnif = 1.0;
	SourceDirX = Positive;
	SourceDirY = Positive;
	DestinDirX = Positive;
	DestinDirY = Positive;
	OriginOffset.x = 0;
	OriginOffset.y = 0;
}

Point ReferenceConvertor::Convert(Point Pt)
{
	if (Magnif <= 0) {
		throw Exception(EXPT_ERROR, "�Ŵ����Ϊ0�����Ǹ�ֵ�ĳ��淶Χ");
	}
	Pt.x = (Pt.x * Magnif + OriginOffset.x) * SourceDirX * DestinDirX;
	Pt.y = (Pt.y * Magnif + OriginOffset.y) * SourceDirY * DestinDirY;
	return Pt;
}

Point ReferenceConvertor::Disconvert(Point Pt)
{
	if (Magnif <= 0) {
		throw Exception(EXPT_ERROR, "�Ŵ����Ϊ0�����Ǹ�ֵ�ĳ��淶Χ");
	}
	Pt.x = (Pt.x * SourceDirX * DestinDirX - OriginOffset.x) / Magnif;
	Pt.y = (Pt.y * SourceDirY * DestinDirY - OriginOffset.y) / Magnif;
	return Pt;
}

Vector ReferenceConvertor::Resize(Vector Vec)
{
	if (Magnif <= 0) {
		throw Exception(EXPT_ERROR, "�Ŵ����Ϊ0�����Ǹ�ֵ�ĳ��淶Χ");
	}
	return Vec * Magnif;
}
