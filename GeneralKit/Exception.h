#pragma once
/**
* project : 物理和渲染模型
* file    : Exception.h
* Copyright 2021 handle All rights reserved.
* 缅怀袁隆平院士
**/

#include <stdio.h>
class Exception;
class ExceptionHandler {
	friend class Exception;
protected:
	/*未知异常处理器*/
	virtual bool ErrorHandler(Exception &Exp);
};

#define MsgBox(q) MessageBox(nullptr,TEXT(q),nullptr,MB_ICONERROR);

class Exception {
private:
	static FILE* fLog;
	static ExceptionHandler* Handler;

	static char* MakeTimeWithRight();
	static char* MakeTimeStamp();

	const int ExptType;
	const char* ErrMsg;
public:
#define EXPT_INFO 0
#define EXPT_WARNING 1
#define EXPT_ERROR 2
#define EXPT_PANIC 3
	/*初始化整个程序的日志文件*/
	static bool InitLog(const char* sFile);
	/*选择异常处理器*/
	static void SelHandler(ExceptionHandler*);
	/*输出异常*/
	Exception(int ExceptionType, const char* sMsg);
	/*只用来输出一些辅助信息*/
	static void print(const char*);
	static void print(const int&);
	/*关闭日志*/
	static void CloseLog();
	/*获取相关信息*/
	int Type() { return ExptType; }
	const char* Msg() { return ErrMsg; }
	/*比较异常*/
	bool operator==(Exception Exp){
		if ((Exp.ExptType == ExptType) && (strcmp(Exp.ErrMsg, ErrMsg) == 0)) {
			return true;
		}
		return false;
	}
	/*调用异常处理程序*/
	static bool HandlerCallProc(Exception &Exp);
};

#define EXCEPTION_PRO {if(Exp.Type()==EXPT_PANIC){ExitProcess(-0);}}
#define FUNCTION_EXCEPTION_FILTER \
catch(...){ \
Exception::print("In ");\
Exception::print(__LINE__);\
Exception::print("\n");\
Exception::print(__FILE__);\
Exception::print("\n");\
Exception::print("Complite on ");\
Exception::print(__DATE__);\
Exception::print("\n");\
throw;\
}
