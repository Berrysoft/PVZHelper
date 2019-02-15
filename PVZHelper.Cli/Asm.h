#pragma once
#include <Windows.h>
#include <memory>

#define BLOCK_SIZE 20

enum RegType
{
    EAX = 0,
    EBX = EAX + BLOCK_SIZE - 1,
    ECX = EBX + BLOCK_SIZE,
    EDX = ECX + BLOCK_SIZE,
    ESI = EDX + BLOCK_SIZE,
    EDI = ESI + BLOCK_SIZE,
    EBP = EDI + BLOCK_SIZE
};

class Asm
{
public:
    std::unique_ptr<unsigned char[]> code;
    int index;
    Asm(int page = 1)
    {
        code = std::make_unique<unsigned char[]>(4096 * page);
        index = 0;
    }
    void clear() { index = 0; }
    void CreateRemoteThread(HANDLE hpro, DWORD ThreadAddr);
    void add_byte(unsigned char val)
    {
        code[index++] = val;
    }
    void add_word(unsigned short val)
    {
        (unsigned short&)code[index] = val, index += 2;
    }
    void add_dword(unsigned int val)
    {
        (unsigned int&)code[index] = val, index += 4;
    }
    void push(int val);
    void mov_exx(RegType reg, int val);
    void mov_exx_dword_ptr(RegType reg, int val);
    void mov_exx_dword_ptr_exx_add(RegType reg, int val);
    void push_exx(RegType reg);
    void pop_exx(RegType reg);
    void call(int addr);
    void ret();
};