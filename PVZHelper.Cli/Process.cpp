#include "Process.h"
#include <TlHelp32.h>

static BOOL EnableDebugPrivilege()
{
    HANDLE hToken;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount = 1;
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
        CloseHandle(hToken);
    }
    return GetLastError() == 0;
}

#pragma warning(push)
#pragma warning(disable : 4793)
DWORD Process::ReadMemory(DWORD size, DWORD base, int argnum, ...)
{
    if (!hpro)
        return 0;
    if (size > 4)
        size = 4;
    DWORD ret = 0;
    if (argnum == 0)
    {
        ReadProcessMemory(hpro, LPCVOID(base), &ret, size, 0);
    }
    else
    {
        ReadProcessMemory(hpro, LPCVOID(base), &ret, 4, 0);
        int* arg = &argnum;
        for (int i = 1; i < argnum; ++i)
        {
            ReadProcessMemory(hpro, LPCVOID(ret + arg[i]), &ret, 4, 0);
        }
        ReadProcessMemory(hpro, LPCVOID(ret + arg[argnum]), &ret, size, 0);
    }
    return ret;
}

void Process::WriteMemory(DWORD val, DWORD size, DWORD base, int argnum, ...)
{
    if (!hpro)
        return;
    if (size > 4)
        size = 4;
    if (argnum == 0)
    {
        WriteProcessMemory(hpro, LPVOID(base), &val, size, 0);
    }
    else
    {
        DWORD temp;
        ReadProcessMemory(hpro, LPCVOID(base), &temp, 4, 0);
        int* arg = &argnum;
        for (int i = 1; i < argnum; ++i)
        {
            ReadProcessMemory(hpro, LPCVOID(temp + arg[i]), &temp, 4, 0);
        }
        WriteProcessMemory(hpro, LPVOID(temp + arg[argnum]), &val, size, 0);
    }
}

void Process::WriteArrayMemory(void* data, DWORD size, DWORD base, int argnum, ...)
{
    if (!hpro)
        return;
    if (argnum == 0)
    {
        WriteProcessMemory(hpro, LPVOID(base), data, size, 0);
        return;
    }
    DWORD temp;
    ReadProcessMemory(hpro, LPCVOID(base), &temp, 4, 0);
    int* arg = &argnum;
    for (int i = 1; i < argnum; ++i)
    {
        ReadProcessMemory(hpro, LPCVOID(temp + arg[i]), &temp, 4, 0);
    }
    WriteProcessMemory(hpro, LPVOID(temp + arg[argnum]), data, size, 0);
}
#pragma warning(pop)

int Process::OpenByPid(DWORD pid)
{
    CloseHandle(hpro);
    hpro = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hpro)
        return SUCCESS;
    else
    {
        EnableDebugPrivilege();
        hpro = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        if (hpro)
            return SUCCESS;
        else
            return OPEN_FAILED;
    }
}

int Process::OpenByWindow(const wchar_t* Title, const wchar_t* Class)
{
    pid = GetProcessId(Title, Class);
    if (pid)
    {
        return OpenByPid(pid);
    }
    else
    {
        return NOT_FOUND;
    }
}

int Process::OpenByName(const wchar_t* Name)
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return OPEN_FAILED;
    }
    PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
    for (BOOL fOk = Process32First(hProcessSnap, &pe); fOk; fOk = Process32Next(hProcessSnap, &pe))
    {
        if (_wcsicmp(pe.szExeFile, Name) == 0)
        {
            pid = pe.th32ProcessID;
            CloseHandle(hProcessSnap);
            return OpenByPid(pid);
        }
    }
    hpro = NULL;
    return NOT_FOUND;
}

DWORD Process::GetProcessId(const wchar_t* Title, const wchar_t* Class)
{
    HWND hWnd = FindWindow(Class, Title);
    DWORD pid = 0;
    if (hWnd)
    {
        GetWindowThreadProcessId(hWnd, &pid);
    }
    return pid;
}