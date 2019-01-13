#include "PVZ.h"

class PVZ_1_0 : public PVZ
{
public:
    PVZ_1_0(std::shared_ptr<Process> p) : PVZ(p) {}
    DECLARE_VIRTUAL(;)
    DECLARE_ASM
    void GetPlants()
    {
        vp.clear();
        Plant one;
        one.addr = process->ReadMemory(4, 0x6a9ec0, 2, 0x768, 0xac);
        int pnum = process->ReadMemory(4, 0x6a9ec0, 2, 0x768, 0xbc);
        for (int i = 0; i < pnum;)
        {
            if (process->ReadMemory(1, one.addr + 0x141) == 0)
            {
                one.Row = process->ReadMemory(4, one.addr + 0x1c);
                one.Col = process->ReadMemory(4, one.addr + 0x28);
                one.Type = process->ReadMemory(4, one.addr + 0x24);
                one.Sleeping = process->ReadMemory(1, one.addr + 0x143) == 1;
                vp.push_back(one);
                ++i;
            }
            one.addr += 0x14c;
        }
    }
    void GetItems()
    {
        vi.clear();
        Item one;
        one.addr = process->ReadMemory(4, 0x6a9ec0, 2, 0x768, 0x11c);
        int inum = process->ReadMemory(4, 0x6a9ec0, 2, 0x768, 0x12c);
        for (int i = 0; i < inum;)
        {
            if (process->ReadMemory(1, one.addr + 0x20) == 0)
            {
                one.Row = process->ReadMemory(4, one.addr + 0x14);
                one.Col = process->ReadMemory(4, one.addr + 0x10);
                one.Type = process->ReadMemory(4, one.addr + 8);
                vi.push_back(one);
                ++i;
            }
            one.addr += 0xec;
        }
    }
    void GetCars()
    {
        vc.clear();
        Car one;
        one.addr = process->ReadMemory(4, 0x6a9ec0, 2, 0x768, 0x100);
        int cnum = process->ReadMemory(4, 0x6a9ec0, 2, 0x768, 0x110);
        for (int i = 0; i < cnum;)
        {
            if (process->ReadMemory(1, one.addr + 0x30) == 0)
            {
                one.Row = process->ReadMemory(4, one.addr + 0x14);
                vc.push_back(one);
                ++i;
            }
            one.addr += 0x48;
        }
    }
    void GetZombies()
    {
        vz.clear();
        Zombie one;
        one.addr = process->ReadMemory(4, 0x6a9ec0, 2, 0x768, 0x90);
        int znum = process->ReadMemory(4, 0x6a9ec0, 2, 0x768, 0xa0);
        for (int i = 0; i < znum;)
        {
            if (process->ReadMemory(1, one.addr + 0xec) == 0)
            {
                one.Row = process->ReadMemory(4, one.addr + 0x1c);
                one.Type = process->ReadMemory(4, one.addr + 0x24);
                vz.push_back(one);
                ++i;
            }
            one.addr += 0x15c;
        }
    }
};

struct Map_1_0 : public Map
{
    Map_1_0(std::shared_ptr<Process> p) : Map(p)
    {
        Type = process->ReadMemory(4, 0x6a9ec0, 2, 0x768, 0x554c);
    }
    int GetBlock(int row, int col) /*1.草地2.裸地3.水池*/
    {
        return process->ReadMemory(4, 0x6a9ec0, 2, 0x768, 0x168 + row * 4 + col * 24);
    }
    void SetBlock(int row, int col, int type) /*1.草地2.裸地3.水池*/
    {
        process->WriteMemory(type, 4, 0x6a9ec0, 2, 0x768, 0x168 + row * 4 + col * 24);
    }
    int GetRow(int row) /*0.不出怪1.草地2.水路*/
    {
        return process->ReadMemory(4, 0x6a9ec0, 2, 0x768, 0x5d8 + row * 4);
    }
    void SetRow(int row, int type) /*0.不出怪1.草地2.水路*/
    {
        process->WriteMemory(type, 4, 0x6a9ec0, 2, 0x768, 0x5d8 + row * 4);
    }
};