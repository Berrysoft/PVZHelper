#include "PVZ.h"

class PVZ_1_2_EN_1073 : public PVZ
{
public:
    PVZ_1_2_EN_1073(std::unique_ptr<Process>&& p) : PVZ(std::move(p)) {}
    DECLARE_VIRTUAL(;)
    DECLARE_ASM
    void GetPlants()
    {
        vp.clear();
        Plant one;
        one.addr = process->ReadMemory(4, 0x729670, 2, 0x868, 0xc4);
        int pnum = process->ReadMemory(4, 0x729670, 2, 0x868, 0xd4);
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
        one.addr = process->ReadMemory(4, 0x729670, 2, 0x868, 0x134);
        int inum = process->ReadMemory(4, 0x729670, 2, 0x868, 0x144);
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
        one.addr = process->ReadMemory(4, 0x729670, 2, 0x868, 0x118);
        int cnum = process->ReadMemory(4, 0x729670, 2, 0x868, 0x128);
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
        one.addr = process->ReadMemory(4, 0x729670, 2, 0x868, 0xa8);
        int znum = process->ReadMemory(4, 0x729670, 2, 0x868, 0xb8);
        for (int i = 0; i < znum;)
        {
            if (process->ReadMemory(1, one.addr + 0xec) == 0)
            {
                one.Row = process->ReadMemory(4, one.addr + 0x1c);
                one.Type = process->ReadMemory(4, one.addr + 0x24);
                vz.push_back(one);
                ++i;
            }
            one.addr += 0x168;
        }
    }
};

struct Map_1_2_EN_1073 : public Map
{
    Map_1_2_EN_1073(Process* p) : Map(p)
    {
        Type = process->ReadMemory(4, 0x729670, 2, 0x868, 0x5564);
    }
    int GetBlock(int row, int col) /*1.草地2.裸地3.水池*/
    {
        return process->ReadMemory(4, 0x729670, 2, 0x868, 0x180 + row * 4 + col * 24);
    }
    void SetBlock(int row, int col, int type) /*1.草地2.裸地3.水池*/
    {
        process->WriteMemory(type, 4, 0x729670, 2, 0x868, 0x180 + row * 4 + col * 24);
    }
    int GetRow(int row) /*0.不出怪1.草地2.水路*/
    {
        return process->ReadMemory(4, 0x729670, 2, 0x868, 0x5f0 + row * 4);
    }
    void SetRow(int row, int type) /*0.不出怪1.草地2.水路*/
    {
        process->WriteMemory(type, 4, 0x729670, 2, 0x868, 0x5f0 + row * 4);
    }
};