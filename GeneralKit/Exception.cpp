/**
* project : 物理和渲染模型
* file    : Exception.cpp
* Copyright 2021 handle All rights reserved.
* 缅怀袁隆平院士
**/
#include <time.h>
#include <Windows.h>

#include "Exception.h"

FILE* Exception::fLog = nullptr;
ExceptionHandler* Exception::Handler = nullptr;
/*制作日期字符串，包含版权声明，注意内存需要自己销毁！*/
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
/*制作时间字符串，注意内存需要自己销毁！*/
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
	sprintf_s(Str, MAX_PATH*4, "一个未知的异常被触发，没有任何处理程序拦截了这一消息,程序可能已经\
完全崩溃并且完全无法从错误中恢复.\n下列信息可能有助于判断问题: %hs \n按\" 中止 \"程序将立即停止以保护数据\n\
按 \" 重试 \" 程序将异常交由C++运行库接管\n按 \" 忽略 \" 程序将试图继续执行", Exp.Msg());
	int Val = MessageBoxA(NULL,Str,"根异常处理单元", MB_ABORTRETRYIGNORE | MB_ICONSTOP);
	if (Val == IDABORT) {
		exit(0xf0f0f0f0);
	}
	if (Val == IDRETRY) {
		throw;
	}
	return false;
}
