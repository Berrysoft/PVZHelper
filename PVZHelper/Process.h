#pragma once

#include "windows.h"

enum { NOT_FOUND, OPEN_FAILED, SUCCESS };

class Process
{
public:
	Process(){ hpro = NULL; }
	Process(DWORD pid){ OpenByPid(pid); }
	Process(Process& p){ DuplicateHandle(GetCurrentProcess(), p.GetHandle(), GetCurrentProcess(), &hpro, 0, TRUE, DUPLICATE_SAME_ACCESS); }
	int OpenByPid(DWORD pid);
	int OpenByWindow(char* Title, char* Class = NULL);
	int OpenByName(char* Name);
	~Process(){ CloseHandle(hpro); hpro = NULL; }
	bool IsValid(){ return hpro ? true : false; }
	DWORD ReadMemory(DWORD size, DWORD base, int argnum = 0, ...);
	void WriteMemory(DWORD data, DWORD size, DWORD base, int argnum = 0, ...);
	void WriteArrayMemory(void* data, DWORD size, DWORD base, int argnum = 0, ...);
	int pid;
	HANDLE GetHandle(){ return hpro; }
	static DWORD GetProcessId(char*Title, char* Class = NULL);
private:
	HANDLE hpro;
};

