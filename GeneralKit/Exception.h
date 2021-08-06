#pragma once
/**
* project : �������Ⱦģ��
* file    : Exception.h
* Copyright 2021 handle All rights reserved.
* �廳Ԭ¡ƽԺʿ
**/

#include <stdio.h>
class Exception;
class ExceptionHandler {
	friend class Exception;
protected:
	/*δ֪�쳣������*/
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
	/*��ʼ�������������־�ļ�*/
	static bool InitLog(const char* sFile);
	/*ѡ���쳣������*/
	static void SelHandler(ExceptionHandler*);
	/*����쳣*/
	Exception(int ExceptionType, const char* sMsg);
	/*ֻ�������һЩ������Ϣ*/
	static void print(const char*);
	static void print(const int&);
	/*�ر���־*/
	static void CloseLog();
	/*��ȡ�����Ϣ*/
	int Type() { return ExptType; }
	const char* Msg() { return ErrMsg; }
	/*�Ƚ��쳣*/
	bool operator==(Exception Exp){
		if ((Exp.ExptType == ExptType) && (strcmp(Exp.ErrMsg, ErrMsg) == 0)) {
			return true;
		}
		return false;
	}
	/*�����쳣�������*/
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
