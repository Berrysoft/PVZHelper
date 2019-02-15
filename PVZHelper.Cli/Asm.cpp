#include "Asm.h"
#include <Windows.h>

#pragma warning (push)
#pragma warning (disable: 4793)
void static __declspec(naked) volatile sample()
{
    //#define BLOCK_SIZE 20

    //#define EAX 0
    _asm
	{
#define MOV	0
        mov eax, 0x12345678
#define MOV_DWORD_PTR_ADD	MOV+5
        mov eax, [eax + 0x12345678]
#define PUSH				MOV_DWORD_PTR_ADD+6
        push eax
#define POP		PUSH+1
        pop eax
#define MOV_DWORD_PTR		POP+1
        mov eax, ds:[0x12345678]	/*eax6个字节，其他寄存器7个字节，VS2013下*/
    }
    //#define EBX EAX+BLOCK_SIZE-1
    _asm
	{
        mov ebx, 0x12345678
        mov ebx, [ebx + 0x12345678]
        push ebx
        pop ebx
        mov ebx, ds :[0x12345678]
    }
    //#define ECX EBX+BLOCK_SIZE
    _asm
	{
        mov ecx, 0x12345678
        mov ecx, [ecx + 0x12345678]
        push ecx
        pop ecx
        mov ecx, ds :[0x12345678]
    }
    //#define EDX ECX+BLOCK_SIZE
    _asm
	{
        mov edx, 0x12345678
        mov edx, [edx + 0x12345678]
        push edx
        pop edx
        mov edx, ds :[0x12345678]
    }
    //#define ESI EDX+BLOCK_SIZE
    _asm
	{
        mov esi, 0x12345678
        mov esi, [esi + 0x12345678]
        push esi
        pop esi
        mov esi, ds :[0x12345678]
    }
    //#define EDI ESI+BLOCK_SIZE
    _asm
	{
        mov edi, 0x12345678
        mov edi, [edi + 0x12345678]
        push edi
        pop edi
        mov edi, ds :[0x12345678]
    }
    //#define EBP EDI+BLOCK_SIZE
    _asm
	{
        mov ebp, 0x12345678
        mov ebp, [ebp + 0x12345678]
        push ebp
        pop ebp
        mov ebp, ds :[0x12345678]
    }
#define PUSH_VALUE	EBP+BLOCK_SIZE
    _asm push 0x12345678
#define CALL PUSH_VALUE+5
    _asm call $ + 7
#define JMP_SHORT CALL+5
    _asm jmp short $ + 8
#define RET	JMP_SHORT+2
    _asm ret
}
#pragma warning (pop)

static const unsigned char* const p = (unsigned char*)sample;

void Asm::CreateRemoteThread(HANDLE hpro, DWORD ThreadAddr)
{
    bool NeedFree;
    if (ThreadAddr == 0)
    {
        ThreadAddr = (DWORD)VirtualAllocEx(hpro, NULL, index, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        NeedFree = true;
    }
    else
    {
        NeedFree = false;
    }
    if (ThreadAddr)
    {
        WriteProcessMemory(hpro, (LPVOID)ThreadAddr, code.get(), index, 0);
        HANDLE hth = ::CreateRemoteThread(hpro, NULL, 0, LPTHREAD_START_ROUTINE(ThreadAddr), NULL, 0, NULL);
        WaitForSingleObject(hth, INFINITE);
        CloseHandle(hth);
        if (NeedFree)
            VirtualFreeEx(hpro, (LPVOID)ThreadAddr, 0, MEM_RELEASE);
    }
}

void Asm::push(int val)
{
    add_byte(p[PUSH_VALUE]);
    add_dword(val);
}
void Asm::mov_exx(RegType reg, int val)
{
    add_byte(p[reg + MOV]);
    add_dword(val);
}
void Asm::mov_exx_dword_ptr(RegType reg, int val)
{
    if (reg == EAX)
    {
        add_word((unsigned short&)p[EAX + MOV_DWORD_PTR]);
    }
    else
    {
        add_byte(p[reg + MOV_DWORD_PTR]);
        add_word((unsigned short&)p[reg + MOV_DWORD_PTR + 1]);
    }
    add_dword(val);
}
void Asm::mov_exx_dword_ptr_exx_add(RegType reg, int val)
{
    add_word((unsigned short&)p[reg + MOV_DWORD_PTR_ADD]);
    add_dword(val);
}
void Asm::push_exx(RegType reg)
{
    add_byte(p[reg + PUSH]);
}
void Asm::pop_exx(RegType reg)
{
    add_byte(p[reg + POP]);
}
void Asm::call(int addr)
{
    add_byte(p[CALL]);
    add_dword((unsigned int&)p[CALL + 1]);
    add_word((unsigned short&)p[JMP_SHORT]);
    push(addr);
    ret();
}
void Asm::ret()
{
    add_byte(p[RET]);
}