/**
* project : �������Ⱦģ��
* file    : Exception.cpp
* Copyright 2021 handle All rights reserved.
* �廳Ԭ¡ƽԺʿ
**/
#include <time.h>
#include <Windows.h>

#include "Exception.h"

FILE* Exception::fLog = nullptr;
ExceptionHandler* Exception::Handler = nullptr;
/*���������ַ�����������Ȩ������ע���ڴ���Ҫ�Լ����٣�*/
char* Exception::MakeTimeWithRight()
{
	time_t t = time(nullptr);
	tm tM = { 0 };
	localtime_s(&tM,&t);
	char* RetVal = new char[MAX_PATH];
	strftime(RetVal, MAX_PATH,
		"\
*Init Log \n\
/Time: y%y m%m d%d %H:%M:%S \n\
/CopyRight handle 2021 \n\
*Record \n\
"
, &tM);

	return RetVal;
}
/*����ʱ���ַ�����ע���ڴ���Ҫ�Լ����٣�*/
char* Exception::MakeTimeStamp()
{
	time_t t = time(nullptr);
	tm tM = { 0 };
	localtime_s(&tM,&t);
	char* RetVal = new char[MAX_PATH];
	strftime(RetVal, MAX_PATH, "At %H:%M:%S\n", &tM);

	return RetVal;
}

bool Exception::InitLog(const char* sFile)
{
	fopen_s(&fLog,sFile, "at+");
	if (fLog == nullptr) {
		return false;
	}
	char* str = MakeTimeWithRight();
	fputs(str, fLog);
	delete []str;
	str = nullptr;
	fflush(fLog);
	return true;
}

void Exception::SelHandler(ExceptionHandler* Handler_)
{
	Handler = Handler_;
}

Exception::Exception(int ExceptionType, const char* sMsg):ExptType(ExceptionType),ErrMsg(sMsg)
{

	if (fLog == nullptr) {
		throw "Log init failure";
	}
	char* str = new char[MAX_PATH];
	if (ExceptionType == EXPT_INFO) {
		sprintf_s(str, MAX_PATH, "##INFO: %s\n", sMsg);
	}
	else if (ExceptionType == EXPT_WARNING) {
		sprintf_s(str, MAX_PATH, "##WARNING: %s\n", sMsg);
	}
	else if (ExceptionType == EXPT_ERROR) {
		sprintf_s(str, MAX_PATH, "##ERROR: %s\n", sMsg);
	}
	else if (ExceptionType == EXPT_PANIC) {
		sprintf_s(str, MAX_PATH, "##PANIC!: %s\n", sMsg);
	}
	else {
		throw Exception(EXPT_ERROR,"Exception type undefined.");
	}
	fputs(str, fLog);
	delete[]str;
	str = MakeTimeStamp();
	fputs(str, fLog);
	delete[]str;
	fflush(fLog);
}

void Exception::print(const char* sStr)
{
	fputs(sStr, fLog);
	fflush(fLog);
}

void Exception::print(const int& num)
{
	char tmp[16] = "";
	sprintf_s(tmp, 16, "%d", num);
	print(tmp);
}

void Exception::CloseLog()
{
	fclose(fLog);
	fLog = nullptr;
}

bool Exception::HandlerCallProc(Exception& Exp)
{
	if (Handler == nullptr) {
		return true;
	}
	return Handler->ErrorHandler(Exp);
}

bool ExceptionHandler::ErrorHandler(Exception& Exp)
{
	char Str[MAX_PATH*4] = "";
	sprintf_s(Str, MAX_PATH*4, "һ��δ֪���쳣��������û���κδ��������������һ��Ϣ,��������Ѿ�\
��ȫ����������ȫ�޷��Ӵ����лָ�.\n������Ϣ�����������ж�����: %hs \n��\" ��ֹ \"��������ֹͣ�Ա�������\n\
�� \" ���� \" �����쳣����C++���п�ӹ�\n�� \" ���� \" ������ͼ����ִ��", Exp.Msg());
	int Val = MessageBoxA(NULL,Str,"���쳣����Ԫ", MB_ABORTRETRYIGNORE | MB_ICONSTOP);
	if (Val == IDABORT) {
		exit(0xf0f0f0f0);
	}
	if (Val == IDRETRY) {
		throw;
	}
	return false;
}
