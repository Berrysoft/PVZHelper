#include "PVZ_1_2_CN_REPAIR.h"

#pragma warning(push)
#pragma warning(disable : 4793)

#define SAFE_THREAD()                                                \
    process->WriteMemory(-2, 1, 0x627ade),                           \
        Sleep(10),                                                   \
        asm_.CreateRemoteThread(process->GetHandle(), eCode.Thread), \
        process->WriteMemory(0xc8, 1, 0x627ade)

bool PVZ_1_2_CN_REPAIR::InGame()
{
    return process->ReadMemory(4, 0x7578f8, 1, 0x868) != 0;
}

int PVZ_1_2_CN_REPAIR::GetMode()
{
    return process->ReadMemory(4, 0x7578f8, 1, 0x918);
}

void PVZ_1_2_CN_REPAIR::SetSun(int sun)
{
    process->WriteMemory(sun, 4, 0x7578f8, 2, 0x868, 0x5578);
}

void PVZ_1_2_CN_REPAIR::SetMoney(int money)
{
    process->WriteMemory(money, 4, 0x7578f8, 2, 0x94c, 0x50);
}

void PVZ_1_2_CN_REPAIR::IgnoreSun(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00422eb2);
        process->WriteMemory(0x39, 1, 0x00422eb6);
        process->WriteMemory(0x91, 1, 0x00422f00);
        process->WriteMemory(0x80, 1, 0x0042f2b2);
        process->WriteMemory(0x80, 1, 0x0042f630);
        process->WriteMemory(0xeb, 1, 0x00434818);
    }
    else
    {
        process->WriteMemory(0x7f, 1, 0x00422eb2);
        process->WriteMemory(0x89, 1, 0x00422eb6);
        process->WriteMemory(0x9e, 1, 0x00422f00);
        process->WriteMemory(0x8f, 1, 0x0042f2b2);
        process->WriteMemory(0x8f, 1, 0x0042f630);
        process->WriteMemory(0x7d, 1, 0x00434818);
    }
}

void PVZ_1_2_CN_REPAIR::AutoPickup(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x0043b3a2);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0043b3a2);
    }
}

void PVZ_1_2_CN_REPAIR::StartCar()
{
    if (InGame())
    {
        asm_.clear();
        GetCars();
        for (auto& rc : vc)
        {
            asm_.mov_exx(EBX, rc.addr);
            asm_.call(0x0046ade0);
        }
        asm_.ret();
        SAFE_THREAD();
    }
}

void PVZ_1_2_CN_REPAIR::ResetCar()
{
    if (InGame())
    {
        asm_.clear();
        GetCars();
        for (auto& rc : vc)
        {
            asm_.mov_exx(EAX, rc.addr);
            asm_.call(0x0046ad50);
        }
        process->WriteMemory(0x60eb, 2, 0x00411068);
        process->WriteMemory(1, 1, 0x004110e6);
        process->WriteMemory(0xa8dc, 2, 0x0046a119);
        asm_.mov_exx_dword_ptr(EAX, 0x7578f8);
        asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
        asm_.push_exx(EAX);
        asm_.call(0x00411040);
        asm_.ret();
        SAFE_THREAD();
        process->WriteMemory(0x0975, 2, 0x00411068);
        process->WriteMemory(0, 1, 0x004110e6);
        process->WriteMemory(0xad30, 2, 0x0046a119);
    }
}

void PVZ_1_2_CN_REPAIR::asm_plant(int row, int col, int type, bool copy)
{
    if (copy)
    {
        asm_.push(type);
        asm_.push(48);
        asm_.mov_exx(EAX, row);
        asm_.push(col);
        asm_.mov_exx_dword_ptr(EBP, 0x7578f8);
        asm_.mov_exx_dword_ptr_exx_add(EBP, 0x868);
        asm_.push_exx(EBP);
        asm_.call(0x00412370);
        asm_.add_word(0xf08b); //mov esi,eax
        asm_.push_exx(EAX);
        asm_.call(0x00479720);
        asm_.add_byte(0x58); //pop eax
    }
    else
    {
        asm_.push(-1);
        asm_.push(type);
        asm_.mov_exx(EAX, row);
        asm_.push(col);
        asm_.mov_exx_dword_ptr(EBP, 0x7578f8);
        asm_.mov_exx_dword_ptr_exx_add(EBP, 0x868);
        asm_.push_exx(EBP);
        asm_.call(0x00412370);
    }
}

void PVZ_1_2_CN_REPAIR::asm_plant_paper(int row, int col, int type)
{
    asm_plant(row, col, type, false);
    asm_.push_exx(EAX);
    asm_.mov_exx_dword_ptr(EAX, 0x7578f8);
    asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
    asm_.mov_exx_dword_ptr_exx_add(EAX, 0x178);
    asm_.call(0x00433700);
}

void PVZ_1_2_CN_REPAIR::Planting(int row, int col, int type, bool copy)
{
    if (InGame())
    {
        Map_1_2_CN_REPAIR map(process.get());
        int Rows = map.GetRowCount();
        int Cols = (type == 47 ? 4 : 9);
        asm_.clear();
        int mode = GetMode();
        if (row == -1 && col == -1)
        {
            for (int i = 0; i < Rows; ++i)
            {
                for (int j = 0; j < Cols; ++j)
                {
                    if (mode >= 61 && mode <= 70)
                        asm_plant_paper(i, j, type);
                    else
                        asm_plant(i, j, type, copy);
                    if (type == 47)
                        ++j;
                }
            }
        }
        else if (row != -1 && col == -1)
        {
            for (int i = 0; i < Cols; ++i)
            {
                if (mode >= 61 && mode <= 70)
                    asm_plant_paper(row, i, type);
                else
                    asm_plant(row, i, type, copy);
                if (type == 47)
                    ++i;
            }
        }
        else if (row == -1 && col != -1)
        {
            for (int i = 0; i < Rows; ++i)
            {
                if (mode >= 61 && mode <= 70)
                    asm_plant_paper(i, col, type);
                else
                    asm_plant(i, col, type, copy);
            }
        }
        else
        {
            if (mode >= 61 && mode <= 70)
                asm_plant_paper(row, col, type);
            else
                asm_plant(row, col, type, copy);
        }
        asm_.ret();
        SAFE_THREAD();
    }
}

void PVZ_1_2_CN_REPAIR::NoPause(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc3, 1, 0x0045ff30);
    }
    else
    {
        process->WriteMemory(0x56, 1, 0x0045ff30);
    }
}

void PVZ_1_2_CN_REPAIR::asm_put_zombie(int row, int col, int type)
{
    asm_.push(type);
    asm_.mov_exx_dword_ptr(ECX, 0x7578f8);
    asm_.mov_exx_dword_ptr_exx_add(ECX, 0x868);
    asm_.mov_exx_dword_ptr_exx_add(ECX, 0x178);
    asm_.push_exx(ECX);
    asm_.mov_exx(EAX, row);
    asm_.mov_exx(ECX, col);
    asm_.call(0x00433330);
}

void PVZ_1_2_CN_REPAIR::PutZombie(int row, int col, int type)
{
    if (InGame())
    {
        if (type == 25)
        {
            asm_.clear();
            asm_.mov_exx_dword_ptr(EAX, 0x7578f8);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
            asm_.push(0);
            asm_.push(25);
            asm_.call(0x00413430);
            asm_.ret();
            SAFE_THREAD();
            return;
        }
        Map_1_2_CN_REPAIR map(process.get());
        int Rows = map.GetRowCount();
        int Cols = 9;
        asm_.clear();
        if (row == -1 && col == -1)
        {
            for (int i = 0; i < Rows; ++i)
            {
                for (int j = 0; j < Cols; ++j)
                {
                    asm_put_zombie(i, j, type);
                }
            }
        }
        else if (row != -1 && col == -1)
        {
            for (int i = 0; i < Cols; ++i)
            {
                asm_put_zombie(row, i, type);
            }
        }
        else if (row == -1 && col != -1)
        {
            for (int i = 0; i < Rows; ++i)
            {
                asm_put_zombie(i, col, type);
            }
        }
        else
        {
            asm_put_zombie(row, col, type);
        }
        asm_.ret();
        SAFE_THREAD();
    }
}

void PVZ_1_2_CN_REPAIR::asm_put_ladder(int row, int col)
{
    asm_.mov_exx(EDI, row);
    asm_.push(col);
    asm_.mov_exx_dword_ptr(EAX, 0x7578f8);
    asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
    asm_.call(0x0040e290);
}

void PVZ_1_2_CN_REPAIR::PutLadder(int row, int col)
{
    if (InGame())
    {
        Map_1_2_CN_REPAIR map(process.get());
        int Rows = map.GetRowCount();
        int Cols = 9;
        asm_.clear();
        if (row == -1 && col == -1)
        {
            for (int i = 0; i < Rows; ++i)
            {
                for (int j = 0; j < Cols; ++j)
                {
                    asm_put_ladder(i, j);
                }
            }
        }
        else if (row != -1 && col == -1)
        {
            for (int i = 0; i < Cols; ++i)
            {
                asm_put_ladder(row, i);
            }
        }
        else if (row == -1 && col != -1)
        {
            for (int i = 0; i < Rows; ++i)
            {
                asm_put_ladder(i, col);
            }
        }
        else
        {
            asm_put_ladder(row, col);
        }
        asm_.ret();
        SAFE_THREAD();
    }
}

void PVZ_1_2_CN_REPAIR::asm_put_grave(int row, int col)
{
    asm_.mov_exx_dword_ptr(ECX, 0x7578f8);
    asm_.mov_exx_dword_ptr_exx_add(ECX, 0x868);
    asm_.mov_exx_dword_ptr_exx_add(ECX, 0x178);
    asm_.push_exx(ECX);
    asm_.mov_exx(EDI, row);
    asm_.mov_exx(EBX, col);
    asm_.call(0x0042dda0);
}

void PVZ_1_2_CN_REPAIR::PutGrave(int row, int col)
{
    if (InGame())
    {
        Map_1_2_CN_REPAIR map(process.get());
        int Rows = map.GetRowCount();
        int Cols = 9;
        asm_.clear();
        if (row == -1 && col == -1)
        {
            for (int i = 0; i < Rows; ++i)
            {
                for (int j = 0; j < Cols; ++j)
                {
                    asm_put_grave(i, j);
                }
            }
        }
        else if (row != -1 && col == -1)
        {
            for (int i = 0; i < Cols; ++i)
            {
                asm_put_grave(row, i);
            }
        }
        else if (row == -1 && col != -1)
        {
            for (int i = 0; i < Rows; ++i)
            {
                asm_put_grave(i, col);
            }
        }
        else
        {
            asm_put_grave(row, col);
        }
        asm_.ret();
        SAFE_THREAD();
    }
}

void PVZ_1_2_CN_REPAIR::AutoLadder()
{
    if (InGame())
    {
        bool HasLadder[6][9] = { 0 };
        asm_.clear();
        GetItems();
        for (auto& ri : vi)
        {
            if (ri.Type == 3)
            {
                HasLadder[ri.Row][ri.Col] = true;
            }
        }
        Map_1_2_CN_REPAIR map(process.get());
        GetPlants();
        for (auto& rp : vp)
        {
            if (rp.Type == 30)
            {
                if (rp.Col != 0 && !HasLadder[rp.Row][rp.Col] && map.GetBlock(rp.Row, rp.Col) != 3)
                {
                    asm_put_ladder(rp.Row, rp.Col);
                }
            }
        }
        asm_.ret();
        SAFE_THREAD();
    }
}

//void static asm_put_rake(int row, int col)
//{
//	asm_.push(row);
//	asm_.push(col);
//	asm_.mov_exx_dword_ptr(ECX, 0x7578f8);
//	asm_.mov_exx_dword_ptr_exx_add(ECX, 0x868);
//	asm_.push_exx(ECX);
//	asm_.call(0x00410df0);
//	asm_.add_esp(8);
//}
//
//void PVZ_1_2_CN_REPAIR::PutRake(int row, int col)
//{
//	if (InGame())
//	{
//		Map_1_2_CN_REPAIR map(process.get());
//		int Rows = map.GetRowCount();
//		int Cols = 9;
//		asm_.clear();
//		if (row == -1 && col == -1)
//		{
//			for (int i = 0; i < Rows; ++i)
//			{
//				for (int j = 0; j < Cols; ++j)
//				{
//					asm_put_rake(i, j);
//				}
//			}
//		}
//		else if (row != -1 && col == -1)
//		{
//			for (int i = 0; i < Cols; ++i)
//			{
//				asm_put_rake(row, i);
//			}
//		}
//		else if (row == -1 && col != -1)
//		{
//			for (int i = 0; i < Rows; ++i)
//			{
//				asm_put_rake(i, col);
//			}
//		}
//		else
//		{
//			asm_put_rake(row, col);
//		}
//		asm_.ret();
//		process->WriteMemory(0x0020810f, 4, 0x00410e12);
//		process->WriteMemory(1, 1, 0x00410f55);
//		process->WriteMemory(0xeb107d8b, 4, 0x00410f51);
//		process->WriteMemory(0x900c758b, 4, 0x00410f65);
//		SAFE_THREAD();
//		process->WriteMemory(0x026c840f, 4, 0x00410e12);
//		process->WriteMemory(0, 1, 0x00410f55);
//		process->WriteMemory(0x024888ff, 4, 0x00410f51);
//		process->WriteMemory(0x1024748b, 4, 0x00410f65);
//	}
//}

void PVZ_1_2_CN_REPAIR::SetBlock(int row, int col, int type)
{
    Map_1_2_CN_REPAIR map(process.get());
    int Rows = map.GetRowCount();
    int Cols = 9;
    if (row == -1 && col == -1)
    {
        for (int i = 0; i < Rows; ++i)
        {
            for (int j = 0; j < Cols; ++j)
            {
                map.SetBlock(i, j, type);
            }
        }
    }
    else if (row != -1 && col == -1)
    {
        for (int i = 0; i < Cols; ++i)
        {
            map.SetBlock(row, i, type);
        }
    }
    else if (row == -1 && col != -1)
    {
        for (int i = 0; i < Rows; ++i)
        {
            map.SetBlock(i, col, type);
        }
    }
    else
    {
        map.SetBlock(row, col, type);
    }
}

void PVZ_1_2_CN_REPAIR::SetRow(int row, int type)
{
    Map_1_2_CN_REPAIR map(process.get());
    map.SetRow(row, type);
}

void PVZ_1_2_CN_REPAIR::NoDelete(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x0061e7f3);
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x0061e7f3);
    }
}

void PVZ_1_2_CN_REPAIR::NoSave(bool b)
{
    if (b)
    {
        process->WriteMemory(0x41, 1, 0x0049c733);
    }
    else
    {
        process->WriteMemory(0x19, 1, 0x0049c733);
    }
}

void PVZ_1_2_CN_REPAIR::Anywhere(bool b)
{
    if (b)
    {
        process->WriteMemory(0x810fc933, 4, 0x00413737);
        process->WriteMemory(0x81, 1, 0x004334d8);
    }
    else
    {
        process->WriteMemory(0x850fc985, 4, 0x00413737);
        process->WriteMemory(0x84, 1, 0x004334d8);
    }
}

void PVZ_1_2_CN_REPAIR::FastBelt(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x0042ac2f);
        process->WriteMemory(0x33, 1, 0x004a411e);
    }
    else
    {
        process->WriteMemory(0x8f, 1, 0x0042ac2f);
        process->WriteMemory(0x85, 1, 0x004a411e);
    }
}

void PVZ_1_2_CN_REPAIR::ShowHide(bool b)
{
    if (b)
    {
        process->WriteMemory(0x38, 1, 0x004374da);
    }
    else
    {
        process->WriteMemory(0x88, 1, 0x004374da);
    }
}

void PVZ_1_2_CN_REPAIR::FullFog(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc3c033, 3, 0x00423810);
    }
    else
    {
        DWORD FogAddr = process->ReadMemory(4, 0x7578f8, 1, 0x868) + 0x4e0;
        for (int i = 0; i < 63; ++i)
        {
            process->WriteMemory(0, 4, FogAddr + i * 4);
        }
        process->WriteMemory(0xa4818b, 3, 0x00423810);
    }
}

void PVZ_1_2_CN_REPAIR::SeeFog(bool b)
{
    if (b)
    {
        process->WriteMemory(0xd231, 2, 0x0041fcbd);
    }
    else
    {
        process->WriteMemory(0xf23b, 2, 0x0041fcbd);
    }
}

void PVZ_1_2_CN_REPAIR::SeeVase(bool b)
{
    if (b)
    {
        static unsigned char code[] = { 0xb8, 0x32, 0, 0, 0 };
        process->WriteArrayMemory(code, sizeof(code), 0x0045c66a);
    }
    else
    {
        static unsigned char code[] = { 0x85, 0xc0, 0x7e, 4, 0x48 };
        process->WriteArrayMemory(code, sizeof(code), 0x0045c66a);
    }
}

void PVZ_1_2_CN_REPAIR::JmpLevel(int level)
{
    process->WriteMemory(level, 4, 0x7578f8, 3, 0x868, 0x178, 0x6c);
}

void PVZ_1_2_CN_REPAIR::MixMode(int mode, int level)
{
    if (mode == 0)
    {
        process->WriteMemory(level, 4, 0x7578f8, 2, 0x94c, 0x4c);
        process->WriteMemory(level, 4, 0x7578f8, 2, 0x868, 0x5568);
    }
    process->WriteMemory(mode, 4, 0x7578f8, 1, 0x918);
}

void PVZ_1_2_CN_REPAIR::Unlock(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x00457843);
        process->WriteMemory(0xeb, 1, 0x00464ca1);
        process->WriteMemory(0xc301b0, 3, 0x004058f0);
        process->WriteMemory(0, 4, 0x00749d08);
        process->WriteMemory(0xc301b0, 3, 0x00405a70);
        process->WriteMemory(0xeb, 1, 0x00457866);
        process->WriteMemory(0xeb, 1, 0x004a4f08);
        process->WriteMemory(0xeb, 1, 0x004a81ca);
        process->WriteMemory(0xc3c030, 4, 0x004a54f0);
        process->WriteMemory(0xc301b0, 3, 0x004a6f30);
        process->WriteMemory(0x70, 1, 0x00465303);
        process->WriteMemory(0x70, 1, 0x00457f67);
        process->WriteMemory(0x2febc031, 4, 0x00437a10);
    }
    else
    {
        process->WriteMemory(0x7f, 1, 0x00457843);
        process->WriteMemory(0x7f, 1, 0x00464ca1);
        process->WriteMemory(0x08ec83, 3, 0x004058f0);
        process->WriteMemory(40, 4, 0x00749d08);
        process->WriteMemory(0x575551, 3, 0x00405a70);
        process->WriteMemory(0x7f, 1, 0x00457866);
        process->WriteMemory(0x7d, 1, 0x004a4f08);
        process->WriteMemory(0x7d, 1, 0x004a81ca);
        process->WriteMemory(0xf98b57, 3, 0x004a54f0);
        process->WriteMemory(0xb78b56, 3, 0x004a6f30);
        process->WriteMemory(0x7e, 1, 0x00465303);
        process->WriteMemory(0x7e, 1, 0x00457f67);
        process->WriteMemory(0x6c8b5553, 4, 0x00437a10);
    }
}

void PVZ_1_2_CN_REPAIR::Win()
{
    int mode = GetMode();
    if (!InGame() ||
        mode >= 1 && mode <= 15 && process->ReadMemory(4, 0x7578f8, 1, 0x91c != 3))
    {
        return;
    }
    asm_.clear();
    asm_.mov_exx_dword_ptr(EAX, 0x7578f8);
    asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
    asm_.push_exx(EAX);
    asm_.call(0x00411880);
    asm_.ret();
    SAFE_THREAD();
}

void PVZ_1_2_CN_REPAIR::Lock(bool b, int mode)
{
    if (b)
    {
        static const int val[] = { 26, -660, 79, 106, 124, 194, 253, 364 };
        process->WriteMemory(0x81, 1, 0x0043439b);
        process->WriteMemory(val[mode], 4, 0x0043439c);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x0043439b);
        process->WriteMemory(146, 4, 0x0043439c);
    }
}

void PVZ_1_2_CN_REPAIR::Little(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x005480ca);
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x005480ca);
    }
}

void PVZ_1_2_CN_REPAIR::Rain(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x0041bfb3);
        process->WriteMemory(0x70, 1, 0x0042e290);
        process->WriteMemory(0xeb, 1, 0x0042c005);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0041bfb3);
        process->WriteMemory(0x75, 1, 0x0042e290);
        process->WriteMemory(0x74, 1, 0x0042c005);
    }
}

void PVZ_1_2_CN_REPAIR::Storm(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x0041bf9b);
        process->WriteMemory(0xeb, 1, 0x0042e2ae);
        process->WriteMemory(0xeb, 1, 0x0042e277);
        process->WriteMemory(0xeb, 1, 0x0042bfbd);
        process->WriteMemory(0xeb, 1, 0x0042c019);
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x0041bf9b);
        process->WriteMemory(0x74, 1, 0x0042e2ae);
        process->WriteMemory(0x74, 1, 0x0042e277);
        process->WriteMemory(0x74, 1, 0x0042bfbd);
        process->WriteMemory(0x74, 1, 0x0042c019);
    }
}

void PVZ_1_2_CN_REPAIR::Force(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00480f67);
        process->WriteMemory(0x70, 1, 0x004815f0);
        process->WriteMemory(0x70, 1, 0x00549e9c);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00480f67);
        process->WriteMemory(0x75, 1, 0x004815f0);
        process->WriteMemory(0x75, 1, 0x00549e9c);
    }
}

void PVZ_1_2_CN_REPAIR::LikeCol(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x004439ad);
        process->WriteMemory(0x80, 1, 0x004163ad);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x004439ad);
        process->WriteMemory(0x85, 1, 0x004163ad);
    }
}

void PVZ_1_2_CN_REPAIR::RunFast(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x0042c1a2);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0042c1a2);
    }
}

void PVZ_1_2_CN_REPAIR::SetCardNum(int num)
{
    if (InGame())
    {
        process->WriteMemory(0xc3, 1, 0x00423530);
        asm_.clear();
        asm_.mov_exx(EAX, num);
        asm_.mov_exx_dword_ptr(ESI, 0x7578f8);
        asm_.mov_exx_dword_ptr_exx_add(ESI, 0x868);
        asm_.mov_exx_dword_ptr_exx_add(ESI, 0x15c);
        asm_.call(0x004a4140);
        asm_.ret();
        SAFE_THREAD();
        process->WriteMemory(0x56, 1, 0x00423530);
    }
}

void PVZ_1_2_CN_REPAIR::SetCard(int cid, int type, bool copy)
{
    if (copy)
    {
        process->WriteMemory(48, 4, 0x7578f8, 3, 0x868, 0x15c, 0x5c + cid * 0x50);
        process->WriteMemory(type, 4, 0x7578f8, 3, 0x868, 0x15c, 0x60 + cid * 0x50);
    }
    else
    {
        process->WriteMemory(type, 4, 0x7578f8, 3, 0x868, 0x15c, 0x5c + cid * 0x50);
    }
}

void PVZ_1_2_CN_REPAIR::FSBoom(bool b)
{
    if (b)
    {
        process->WriteMemory(0x26eb, 2, 0x0042511e);
        process->WriteMemory(0x70, 1, 0x00479042);
    }
    else
    {
        process->WriteMemory(0x5f7f, 2, 0x0042511e);
        process->WriteMemory(0x75, 1, 0x00479042);
    }
}

void PVZ_1_2_CN_REPAIR::FSSquash(bool b)
{
    if (b)
    {
        process->WriteMemory(0x42eb, 2, 0x00473358);
        process->WriteMemory(0x80, 1, 0x00473263);
        process->WriteMemory(0x5beb, 2, 0x00473166);
    }
    else
    {
        process->WriteMemory(0x6f7f, 2, 0x00473358);
        process->WriteMemory(0x85, 1, 0x00473263);
        process->WriteMemory(0x6c75, 2, 0x00473166);
    }
}

void PVZ_1_2_CN_REPAIR::FSMagnet(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x00474a40);
        process->WriteMemory(0x70, 1, 0x00474ac1);
    }
    else
    {
        process->WriteMemory(0x87, 1, 0x00474a40);
        process->WriteMemory(0x74, 1, 0x00474ac1);
    }
}

void PVZ_1_2_CN_REPAIR::FSSputter(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00480e73);
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x00480e73);
    }
}

void PVZ_1_2_CN_REPAIR::FSMouth(bool b)
{
    if (b)
    {
        process->WriteMemory(0x74, 1, eCode.Row + 0x18 - 0x0d);
        process->WriteMemory(0x74, 1, eCode.Col + 0x3d - 0x32);
    }
    else
    {
        process->WriteMemory(0x70, 1, eCode.Row + 0x18 - 0x0d);
        process->WriteMemory(0x70, 1, eCode.Col + 0x3d - 0x32);
    }
}

void PVZ_1_2_CN_REPAIR::FSPrick(bool b)
{
    if (b)
    {
        process->WriteMemory(0x74, 1, eCode.Row + 0x22 - 0x0d);
        process->WriteMemory(0x74, 1, eCode.Row + 0x27 - 0x0d);
        process->WriteMemory(0x74, 1, eCode.Col + 0x4c - 0x32);
        process->WriteMemory(0x74, 1, eCode.Col + 0x51 - 0x32);
        process->WriteMemory(0x74, 1, eCode.RHurt + 0x69 - 0x5c);
        process->WriteMemory(0x74, 1, eCode.RHurt + 0x6e - 0x5c);
        process->WriteMemory(0x74, 1, eCode.CHurt + 0x8b - 0x79);
        process->WriteMemory(0x74, 1, eCode.CHurt + 0x90 - 0x79);
    }
    else
    {
        process->WriteMemory(0x70, 1, eCode.Row + 0x22 - 0x0d);
        process->WriteMemory(0x70, 1, eCode.Row + 0x27 - 0x0d);
        process->WriteMemory(0x70, 1, eCode.Col + 0x4c - 0x32);
        process->WriteMemory(0x70, 1, eCode.Col + 0x51 - 0x32);
        process->WriteMemory(0x70, 1, eCode.RHurt + 0x69 - 0x5c);
        process->WriteMemory(0x70, 1, eCode.RHurt + 0x6e - 0x5c);
        process->WriteMemory(0x70, 1, eCode.CHurt + 0x8b - 0x79);
        process->WriteMemory(0x70, 1, eCode.CHurt + 0x90 - 0x79);
    }
}

void PVZ_1_2_CN_REPAIR::FSSpout(bool b)
{
    if (b)
    {
        process->WriteMemory(0x74, 1, eCode.Row + 0x1d - 0x0d);
        process->WriteMemory(0x74, 1, eCode.Col + 0x42 - 0x32);
        process->WriteMemory(0x74, 1, eCode.Col + 0x47 - 0x32);
        process->WriteMemory(0x74, 1, eCode.RHurt + 0x64 - 0x5c);
        process->WriteMemory(0x74, 1, eCode.CHurt + 0x81 - 0x79);
        process->WriteMemory(0x74, 1, eCode.CHurt + 0x86 - 0x79);
        process->WriteMemory(0x39eb, 2, 0x0047a258);
        process->WriteMemory(0x16eb, 2, 0x00471640);
    }
    else
    {
        process->WriteMemory(0x70, 1, eCode.Row + 0x1d - 0x0d);
        process->WriteMemory(0x70, 1, eCode.Col + 0x42 - 0x32);
        process->WriteMemory(0x70, 1, eCode.Col + 0x47 - 0x32);
        process->WriteMemory(0x70, 1, eCode.RHurt + 0x64 - 0x5c);
        process->WriteMemory(0x70, 1, eCode.CHurt + 0x81 - 0x79);
        process->WriteMemory(0x70, 1, eCode.CHurt + 0x86 - 0x79);
        process->WriteMemory(0x8c0f, 2, 0x0047a258);
        process->WriteMemory(0x8c0f, 2, 0x00471640);
    }
}

void PVZ_1_2_CN_REPAIR::FSCat(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x0047a3bb);
        process->WriteMemory(0x75, 1, eCode.CatR + 0xc3 - 0x9b);
        process->WriteMemory(0x74, 1, eCode.CatC + 0xd1 - 0xc8);
        process->WriteMemory(0x1beb, 2, 0x00477545);
        static unsigned char code1[] = { 0x33, 0xc0, 0xb0, 0x03, 0xeb, 0x0d };
        process->WriteArrayMemory(code1, sizeof(code1), 0x00471460);
        process->WriteMemory(0x3f, 1, 0x00477554);
        static unsigned char _3_code[] = { 0xE9, 0x91, 0x00, 0x00, 0x00, 0x83, 0xF8, 0x12, 0x75, 0xE9, 0x8B, 0x83, 0x90, 0x00, 0x00, 0x00, 0x83, 0xF8, 0x09, 0x74, 0xBE, 0x83, 0xF8, 0x05, 0x74, 0xB9, 0x83, 0xF8, 0x01, 0x74, 0xB4, 0xE9, 0x62, 0xFE, 0xFF, 0xFF };
        process->WriteArrayMemory(_3_code, sizeof(_3_code), 0x0047758f);
        process->WriteMemory(0xeb, 1, 0x00479ea0);
        process->WriteMemory(0x70, 1, 0x00479f11);
        process->WriteMemory(0x70, 1, eCode.Split);
        process->WriteMemory(0xe9, 1, 0x004776b3);
        process->WriteMemory(0xfffffeaa, 4, 0x004776b4);
        process->WriteMemory(0x70, 1, eCode.Throw);
        process->WriteMemory(0xcb, 1, 0x00479f5f);
        process->WriteMemory(0x80, 1, 0x00479e7d);
        process->WriteMemory(0x80, 1, 0x00479e86);
        process->WriteMemory(0x80, 1, 0x00479e8f);
        process->WriteMemory(0x80, 1, 0x00479e98);
        process->WriteMemory(0x80, 1, 0x00479efb);
        process->WriteMemory(0x80, 1, 0x00479f04);
        process->WriteMemory(0xeb, 1, 0x00479f16);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0047a3bb);
        process->WriteMemory(0x70, 1, eCode.CatR + 0xc3 - 0x9b);
        process->WriteMemory(0xeb, 1, eCode.CatC + 0xd1 - 0xc8);
        process->WriteMemory(0x4b8b, 2, 0x00477545);
        static unsigned char code1[] = { 0x8B, 0x40, 0x24, 0x83, 0xF8, 0x1A };
        process->WriteArrayMemory(code1, sizeof(code1), 0x00471460);
        process->WriteMemory(0x2d, 1, 0x00477554);
        static unsigned char _3_code[] = { 0x83, 0xF8, 0x12, 0x0F, 0x85, 0x8B, 0x00, 0x00, 0x00, 0x8B, 0x5F, 0x1C, 0x8B, 0x37, 0x8D, 0x53, 0xFF, 0x89, 0x54, 0x24, 0x14, 0x8B, 0x97, 0x9C, 0x00, 0x00, 0x00, 0x8B, 0xC6, 0xE8, 0x54, 0xCF, 0xFE, 0xFF, 0x8B, 0x97 };
        process->WriteArrayMemory(_3_code, sizeof(_3_code), 0x0047758f);
        process->WriteMemory(0x75, 1, 0x00479ea0);
        process->WriteMemory(0x74, 1, 0x00479f11);
        process->WriteMemory(0xeb, 1, eCode.Split);
        process->WriteMemory(0x8b, 1, 0x004776b3);
        process->WriteMemory(0x006a1c4b, 4, 0x004776b4);
        process->WriteMemory(0xeb, 1, eCode.Throw);
        process->WriteMemory(0x62, 1, 0x00479f5f);
        process->WriteMemory(0x84, 1, 0x00479e7d);
        process->WriteMemory(0x84, 1, 0x00479e86);
        process->WriteMemory(0x84, 1, 0x00479e8f);
        process->WriteMemory(0x84, 1, 0x00479e98);
        process->WriteMemory(0x84, 1, 0x00479efb);
        process->WriteMemory(0x84, 1, 0x00479f04);
        process->WriteMemory(0x75, 1, 0x00479f16);
    }
}

void PVZ_1_2_CN_REPAIR::CatTouch(bool b)
{
    if (b)
    {
        process->WriteMemory(0x81, 1, 0x00480887);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x00480887);
    }
}

void static __declspec(naked) DoorCode()
{
    _asm {
        jmp fend
        mov[eax + 0x14], ecx
        mov dword ptr[eax + 0x1c], 0x00054b78
        ret
        int 3
        int 3
        fend :
    }
}

void static __declspec(naked) RowCode()
{
    _asm {
        jmp fend
        je short label
        mov edx, [ebp + 8]
        mov edx, [edx + 0x24]
        cmp edx, 6
        jo short label
        cmp edx, 10
        jo short label
        cmp edx, 21
        jo short label
        cmp edx, 46
        jo short label
        pop edx
        push 0x0047A450
        label:
        ret
            int 3
            int 3
            fend :
    }
}

void static __declspec(naked) ColCode()
{
    _asm {
        jmp fend
        jge short label
        mov edi, [ebp + 8]
        mov edi, [edi + 0x24]
        cmp edi, 6
        jo short label
        cmp edi, 10
        jo short label
        cmp edi, 42
        jo short label
        cmp edi, 21
        jo short label
        cmp edi, 46
        jo short label
        pop edi
        push 0x0047A44D
        label :
        ret
            int 3
            int 3
            fend :
    }
}

void static __declspec(naked) RHurtCode()
{
    _asm {
        jmp fend
        je short label
        mov edx, [ebp + 0x24]
        cmp edx, 10
        jo short label
        cmp edx, 21
        jo short label
        cmp edx, 46
        jo short label
        pop edx
        push 0x0047171A
        label :
        ret
            int 3
            int 3
            fend :
    }
}

void static __declspec(naked) CHurtCode()
{
    _asm {
        jmp fend
        jg short label
        mov edx, [ebp + 0x24]
        cmp edx, 10
        jo short label
        cmp edx, 42
        jo short label
        cmp edx, 21
        jo short label
        cmp edx, 46
        jo short label
        pop edx
        push 0x0047171A
        label:
        mov eax, [edi + 0x24]
            ret
            int 3
            int 3
            fend :
    }
}

void static __declspec(naked) CatRCode()
{
    _asm {
        jmp fend
        cmp eax, 43
        jne short _1
        _2 :
        pop eax
            _jmp(0x0047A293)
            _1 :
            cmp eax, 4
            je short label
            cmp eax, 6
            je short label
            cmp eax, 10
            je short label
            cmp eax, 42
            je short label
            cmp eax, 21
            je short label
            cmp eax, 46
            jo short _2
            label :
        ret
            int 3
            int 3
            fend :
    }
}

void static __declspec(naked) CatCCode()
{
    _asm {
        jmp fend
        mov ecx, [ebp + 8]
        mov ecx, [ecx + 0x24]
        cmp ecx, 4
        jmp short label
        cmp ecx, 6
        je short label
        cmp ecx, 10
        je short label
        cmp ecx, 42
        je short label
        cmp ecx, 21
        je short label
        cmp ecx, 46
        je short label
        pop ecx
        push 0x0047A3AC
        label:
        cmp byte ptr[esp + 0x17], 0
            ret
            int 3
            int 3
            fend :
    }
}

void static __declspec(naked) ThrowCode()
{
    _asm {
        jmp fend
        jmp short _1
        cmp dword ptr[ebx + 0x24], 47
        je _2
        test eax, eax
        jne _2
        _jmp(0x00477415)
        _2:
        mov ecx, [ebx + 0x1c]
            push esi
            _jmp(0x0047757B)
            _1 :
            mov ecx, [ebx + 0x1c]
            push esi
            push ecx
            _jmp(0x00477720)
            int 3
            int 3
            fend:
    }
}

void static __declspec(naked) CatDirCode()
{
    _asm {
        jmp fend
        cmp eax, 52
        jne short _1
        _3 :
        fld dword ptr ds : [0x0072A8C4]
            ret
            _1 :
        cmp eax, 28
            jne short _2
            cmp ecx, 1
            je short _3
            _2 :
        fld dword ptr ds : [0x0072A4D4]
            ret
            int 3
            int 3
            fend :
    }
}

void static __declspec(naked) SplitCode()
{
    _asm {
        jmp fend
        jmp short _1
        mov ecx, [ebx + 0x1c]
        push ecx
        push ebx
        xor ecx, ecx
        _call(0x0047A190)
        test eax, eax
        jne _2
        _jmp(0x00477415)
        _2:
        push eax
            mov ecx, [ebx + 0x1c]
            push 0
            push ecx
            push eax
            push ebx
            _call(0x00479A50)
            pop eax
            mov edx, [ebx + 0x1c]
            push 1
            push edx
            push eax
            push ebx
            _call(0x00479A50)
            _jmp(0x00477415)
            _1:
        mov esi, 3
            _jmp(0x0047764D)
            int 3
            int 3
            fend :
    }
}

void static __declspec(naked) ZTimerCode()
{
    _asm {
        jmp fend
        mov ecx, 0xffffffff
        cmp[ebp + 0x55b4], ecx
        ja short _1
        dec dword ptr ds : [ebp + 0x55b4]
        ret
        _1 :
        mov[ebp + 0x55b4], ecx
            ret
            int 3
            int 3
            fend :
    }
}

void static __declspec(naked) GetHeadRowsCode()
{
    _asm {
        jmp fend
        mov eax, ds:[0x7578f8]
        mov eax, [eax + 0x868]
        mov eax, [eax + 0x5564]
        cmp eax, 2
        je short _6
        cmp eax, 3
        jne short _5
        _6 :
        mov eax, 6
            jmp short label
            _5 :
        mov eax, 5
            label :
            mov dword ptr[ebp + 0x28], 0x59
            ret
            int 3
            int 3
            fend :
    }
}

void static __declspec(naked) GetRVRowsCode()
{
    _asm {
        jmp fend
        mov eax, ds:[0x7578f8]
        mov eax, [eax + 0x868]
        mov eax, [eax + 0x5564]
        cmp eax, 2
        je short _6
        cmp eax, 3
        jne short _5
        _6 :
        mov eax, 5
            jmp short label
            _5 :
        mov eax, 4
            label :
            mov dword ptr[esi + 0x28], 0x55
            ret
            int 3
            int 3
            fend :
    }
}

void static __declspec(naked) DoorRowsCode()
{
    _asm {
        jmp fend
        mov eax, ds:[0x7578f8]
        mov eax, [eax + 0x868]
        mov eax, [eax + 0x5564]
        cmp eax, 2
        je short _6
        cmp eax, 3
        jne short _5
        _6 :
        mov eax, 6
            jmp short label
            _5 :
        mov eax, 5
            label :
            cmp edi, eax
            jge label2
            pop eax
            _jmp(0x0042ed00)
            label2 :
            ret
            int 3
            int 3
            fend :
    }
}

void PVZ_1_2_CN_REPAIR::InitExtraCode()
{
#define WRITE_CODE(f)                                                      \
    process->WriteArrayMemory(GET_ADDR(f) + HEAD_OF(f), SIZE_OF(f), temp), \
        process->WriteArrayMemory(CallCode, sizeof(CallCode), Caller)

    bool b = (process->ReadMemory(4, 0x0042e6f7) != 0xcccccccc);
    DWORD temp;
    if (b)
    {
        temp = process->ReadMemory(4, 0x0042e6f7);
    }
    else
    {
        temp = (DWORD)VirtualAllocEx(process->GetHandle(), NULL, 4096 * 2, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (temp)
            process->WriteMemory(temp, 4, 0x0042e6f7);
    }
    DWORD Caller;
    /*传送门*/
    if (!b)
    {
        Caller = 0x0042e7b0;
        static unsigned char CallCode[] = { 0xb9, 4, 0, 0, 0, 0xe8, 0, 0, 0, 0 };
        (DWORD&)CallCode[6] = temp - Caller - 10;
        WRITE_CODE(DoorCode);
    }
    temp += SIZE_OF(DoorCode);
    /*行判定*/
    eCode.Row = temp;
    if (!b)
    {
        Caller = 0x0047a28d;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(RowCode);
    }
    temp += SIZE_OF(RowCode);
    /*列判定*/
    eCode.Col = temp;
    if (!b)
    {
        Caller = 0x0047a3a6;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(ColCode);
    }
    temp += SIZE_OF(ColCode);
    /*行伤害判定*/
    eCode.RHurt = temp;
    if (!b)
    {
        Caller = 0x00471652;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(RHurtCode);
    }
    temp += SIZE_OF(RHurtCode);
    /*列伤害判定*/
    eCode.CHurt = temp;
    if (!b)
    {
        Caller = 0x004716be;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(CHurtCode);
    }
    temp += SIZE_OF(CHurtCode);
    /*香蒲免行判定*/
    eCode.CatR = temp;
    if (!b)
    {
        Caller = 0x0047a24b;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(CatRCode);
    }
    temp += SIZE_OF(CatRCode);
    /*香蒲免列判定*/
    eCode.CatC = temp;
    if (!b)
    {
        Caller = 0x0047a38c;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(CatCCode);
    }
    temp += SIZE_OF(CatCCode);
    /*投手特殊处理*/
    eCode.Throw = temp;
    if (!b)
    {
        Caller = 0x0047771b;
        static unsigned char CallCode[] = { 0xe9, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(ThrowCode);
    }
    temp += SIZE_OF(ThrowCode);
    /*猫式全屏子弹方向处理*/
    eCode.CatDir = temp;
    if (!b)
    {
        Caller = 0x00479f2e;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(CatDirCode);
    }
    temp += SIZE_OF(CatDirCode);
    /*裂荚处理*/
    eCode.Split = temp;
    if (!b)
    {
        Caller = 0x00477648;
        static unsigned char CallCode[] = { 0xe9, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(SplitCode);
    }
    temp += SIZE_OF(SplitCode);
    /*刷怪倒计时*/
    eCode.ZTimer = temp;
    if (!b)
    {
        Caller = 0x004198ca;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(ZTimerCode);
    }
    temp += SIZE_OF(ZTimerCode);
    /*僵博吐球行*/
    if (!b)
    {
        Caller = 0x0055967a;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x50, 0x90, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(GetHeadRowsCode);
    }
    temp += SIZE_OF(GetHeadRowsCode);
    /*僵博摔车行*/
    if (!b)
    {
        Caller = 0x00558d3c;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x50, 0x90, 0x90, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(GetRVRowsCode);
    }
    temp += SIZE_OF(GetRVRowsCode);
    /*传送门行*/
    if (!b)
    {
        Caller = 0x0042ed39;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(DoorRowsCode);
    }
    temp += SIZE_OF(DoorRowsCode);

    eCode.Thread = temp;
    return;
}

void PVZ_1_2_CN_REPAIR::StartDoor(bool b)
{
    if (b)
    {
        if (InGame() && process->ReadMemory(4, 0x7578f8, 1, 0x91c) > 2)
        {
            bool HasDoor = false;
            GetItems();
            for (auto& ri : vi)
            {
                if (ri.Type == 4 || ri.Type == 5)
                {
                    HasDoor = true;
                    break;
                }
            }
            if (!HasDoor)
            {
                SetDoor(0, 2, 1, 9, 3, 9, 4, 2);
            }
        }
        process->WriteMemory(0x70, 1, 0x0047a230);
        process->WriteMemory(0x70, 1, 0x0042c1c8);
        process->WriteMemory(0x70, 1, 0x00428062);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0047a230);
        process->WriteMemory(0x75, 1, 0x0042c1c8);
        process->WriteMemory(0x75, 1, 0x00428062);
    }
}

void PVZ_1_2_CN_REPAIR::StableDoor(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x004198ca);
    }
    else
    {
        process->WriteMemory(-1, 1, 0x004198ca);
    }
}

void PVZ_1_2_CN_REPAIR::SetDoor(int b1r, int b1c, int b2r, int b2c, int w1r, int w1c, int w2r, int w2c)
{
    if (InGame())
    {
        asm_.clear();
        GetItems();
        for (auto& ri : vi)
        {
            if (ri.Type == 4 || ri.Type == 5)
            {
                asm_.mov_exx(ESI, ri.addr);
                asm_.call(0x0045b160);
            }
        }
        process->WriteMemory(b1c, 1, 0x0042e723);
        process->WriteMemory(b1r, 1, 0x0042e72a);
        process->WriteMemory(b2c, 1, 0x0042e74f);
        process->WriteMemory(b2r, 1, 0x0042e756);
        process->WriteMemory(w1c, 1, 0x0042e780);
        process->WriteMemory(w1r, 1, 0x0042e787);
        process->WriteMemory(w2c, 1, 0x0042e7ac);
        process->WriteMemory(w2r, 1, 0x0042e7b1);
        asm_.mov_exx_dword_ptr(EDI, 0x7578f8);
        asm_.mov_exx_dword_ptr_exx_add(EDI, 0x868);
        asm_.mov_exx_dword_ptr_exx_add(EDI, 0x178);
        asm_.call(0x0042e700);
        asm_.ret();
        SAFE_THREAD();
        process->WriteMemory(2, 1, 0x0042e723);
        process->WriteMemory(0, 1, 0x0042e72a);
        process->WriteMemory(9, 1, 0x0042e74f);
        process->WriteMemory(1, 1, 0x0042e756);
        process->WriteMemory(9, 1, 0x0042e780);
        process->WriteMemory(3, 1, 0x0042e787);
        process->WriteMemory(2, 1, 0x0042e7ac);
        process->WriteMemory(4, 1, 0x0042e7b1);
    }
}

void PVZ_1_2_CN_REPAIR::CornNoWait(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x00473a2c);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x00473a2c);
    }
}

void PVZ_1_2_CN_REPAIR::MagnetNoWait(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x00474777);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x00474777);
    }
}

void PVZ_1_2_CN_REPAIR::PotatoNoWait(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x00472783);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x00472783);
    }
}

void PVZ_1_2_CN_REPAIR::MouthNoWait(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00473ea5);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00473ea5);
    }
}

void PVZ_1_2_CN_REPAIR::PStrong()
{
    process->WriteMemory(0x004046, 3, 0x00553c3b);
    process->WriteMemory(0x909090, 3, 0x004809a5);
    process->WriteMemory(0x909090, 3, 0x00481200);
    process->WriteMemory(0xeb, 1, 0x005527cb);
    process->WriteMemory(0xeb, 1, 0x004243df);
    process->WriteMemory(0xeb, 1, 0x0054b7bd);
    process->WriteMemory(0x70, 1, 0x004716e3);
    process->WriteMemory(0, 1, 0x0047157b);
}

void PVZ_1_2_CN_REPAIR::PNormal()
{
    process->WriteMemory(0xfc4046, 3, 0x00553c3b);
    process->WriteMemory(0x404629, 3, 0x004809a5);
    process->WriteMemory(0x405629, 3, 0x00481200);
    process->WriteMemory(0x74, 1, 0x005527cb);
    process->WriteMemory(0x74, 1, 0x004243df);
    process->WriteMemory(0x75, 1, 0x0054b7bd);
    process->WriteMemory(0x75, 1, 0x004716e3);
    process->WriteMemory(-32, 1, 0x0047157b);
}

void PVZ_1_2_CN_REPAIR::PWeak()
{
    process->WriteMemory(0x004066, 3, 0x00553c3b);
    process->WriteMemory(0x407629, 3, 0x004809a5);
    process->WriteMemory(0x407629, 3, 0x00481200);
    process->WriteMemory(0x74, 1, 0x005527cb);
    process->WriteMemory(0x74, 1, 0x004243df);
    process->WriteMemory(0x75, 1, 0x0054b7bd);
    process->WriteMemory(0x70, 1, 0x004716e3);
    process->WriteMemory(-32, 1, 0x0047157b);
}

void PVZ_1_2_CN_REPAIR::NoMark(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00424fcd);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00424fcd);
    }
}

void PVZ_1_2_CN_REPAIR::NoSleep(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x004704f4);
        if (InGame())
        {
            asm_.clear();
            GetPlants();
            for (auto& rp : vp)
            {
                if (rp.Sleeping)
                {
                    asm_.mov_exx(EDI, rp.addr);
                    asm_.push(0);
                    asm_.call(0x004711b0);
                }
            }
            asm_.ret();
            SAFE_THREAD();
        }
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x004704f4);
    }
}

void PVZ_1_2_CN_REPAIR::StrongWind(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x00479160);
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x00479160);
    }
}

void PVZ_1_2_CN_REPAIR::StrongBean(bool b)
{
    if (b)
    {
        process->WriteMemory(0x3e, 1, 0x00481d52);
    }
    else
    {
        process->WriteMemory(0x25, 1, 0x00481d52);
    }
}

void PVZ_1_2_CN_REPAIR::Umbrella(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00424828);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00424828);
    }
}

void PVZ_1_2_CN_REPAIR::NoCD(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x004a1764);
        process->WriteMemory(0xeb, 1, 0x004a26f0);
    }
    else
    {
        process->WriteMemory(0x7e, 1, 0x004a1764);
        process->WriteMemory(0x75, 1, 0x004a26f0);
    }
}

void PVZ_1_2_CN_REPAIR::LockScoop(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x00417c9b);
        process->WriteMemory(6, 4, 0x7578f8, 3, 0x868, 0x150, 0x30);
    }
    else
    {
        process->WriteMemory(0x89, 1, 0x00417c9b);
    }
}

void PVZ_1_2_CN_REPAIR::AnyMove(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x00428985);
        process->WriteMemory(0x81, 1, 0x00428608);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00428985);
        process->WriteMemory(0x85, 1, 0x00428608);
    }
}

void PVZ_1_2_CN_REPAIR::NoPurple(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc301b0, 3, 0x00424ff0);
        process->WriteMemory(0xeb, 1, 0x00413837);
    }
    else
    {
        process->WriteMemory(0x575351, 3, 0x00424ff0);
        process->WriteMemory(0x74, 1, 0x00413837);
    }
}

void PVZ_1_2_CN_REPAIR::LockButter(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x004776cf);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x004776cf);
    }
}

void PVZ_1_2_CN_REPAIR::LockCorn(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x004776cf);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x004776cf);
    }
}

void PVZ_1_2_CN_REPAIR::BoomIm()
{
    process->WriteMemory(0x80, 1, 0x0054ad4c);
    process->WriteMemory(0x80, 1, 0x0054b6fa);
}

void PVZ_1_2_CN_REPAIR::BoomNormal()
{
    process->WriteMemory(0x8f, 1, 0x0054ad4c);
    process->WriteMemory(0x85, 1, 0x0054b6fa);
}

void PVZ_1_2_CN_REPAIR::BoomNever()
{
    process->WriteMemory(0x81, 1, 0x0054ad4c);
    process->WriteMemory(0x81, 1, 0x0054b6fa);
}

void PVZ_1_2_CN_REPAIR::VisiableNo()
{
    process->WriteMemory(0x70, 1, 0x005521cd);
    process->WriteMemory(0x70, 1, 0x00558230);
}

void PVZ_1_2_CN_REPAIR::VisiableNormal()
{
    process->WriteMemory(0x75, 1, 0x005521cd);
    process->WriteMemory(0x75, 1, 0x00558230);
}

void PVZ_1_2_CN_REPAIR::VisiableYes()
{
    process->WriteMemory(0xeb, 1, 0x005521cd);
    process->WriteMemory(0xeb, 1, 0x00558230);
}

void PVZ_1_2_CN_REPAIR::ZStrong()
{
    process->WriteMemory(0xc0, 1, 0x00555001);
    process->WriteMemory(0x90909090, 4, 0x00555476);
    process->WriteMemory(0xf633c033, 4, 0x00554b68);
    process->WriteMemory(0xc3, 1, 0x00556d60);
    process->WriteMemory(0xeb, 1, 0x00473dba);
}

void PVZ_1_2_CN_REPAIR::ZNormal()
{
    process->WriteMemory(0xc8, 1, 0x00555001);
    process->WriteMemory(0x2024742b, 4, 0x00555476);
    process->WriteMemory(0xf38b027c, 4, 0x00554b68);
    process->WriteMemory(0x55, 1, 0x00556d60);
    process->WriteMemory(0x74, 1, 0x00473dba);
}

void PVZ_1_2_CN_REPAIR::ZWeak()
{
    process->WriteMemory(0xc9, 1, 0x00555001);
    process->WriteMemory(0x9090f631, 4, 0x00555476);
    process->WriteMemory(0xf38bde8b, 4, 0x00554b68);
    process->WriteMemory(0x55, 1, 0x00556d60);
    process->WriteMemory(0x74, 1, 0x00473dba);
}

void PVZ_1_2_CN_REPAIR::NoSlow(bool b)
{
    if (b)
    {
        process->WriteMemory(0x90c033, 3, 0x0054f5c0);
    }
    else
    {
        process->WriteMemory(0xffc083, 3, 0x0054f5c0);
    }
}

void PVZ_1_2_CN_REPAIR::NoButter(bool b)
{
    if (b)
    {
        process->WriteMemory(0x90c033, 3, 0x0054f5dc);
    }
    else
    {
        process->WriteMemory(0xffc083, 3, 0x0054f5dc);
    }
}

void PVZ_1_2_CN_REPAIR::No3zGroup(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x0042e08c);
        process->WriteMemory(0xeb, 1, 0x00418b8c);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0042e08c);
        process->WriteMemory(0x75, 1, 0x00418b8c);
    }
}

void PVZ_1_2_CN_REPAIR::NoIMP(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x0054b364);
    }
    else
    {
        process->WriteMemory(0x7d, 1, 0x0054b364);
    }
}

void PVZ_1_2_CN_REPAIR::NoICE(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc3, 1, 0x0054e8d0);
        process->WriteMemory(0xeb, 1, 0x004278c6);
        for (int i = 0; i < 6; ++i)
        {
            process->WriteMemory(1, 4, 0x7578f8, 2, 0x868, 0x63c + i * 4);
        }
    }
    else
    {
        process->WriteMemory(0x51, 1, 0x0054e8d0);
        process->WriteMemory(0x75, 1, 0x004278c6);
    }
}

void PVZ_1_2_CN_REPAIR::ZEatable(bool b)
{
    if (b)
    {
        process->WriteMemory(0, 1, 0x00473dc7);
    }
    else
    {
        process->WriteMemory(1, 1, 0x00473dc7);
    }
}

void PVZ_1_2_CN_REPAIR::StopZTimer(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc3, 1, eCode.ZTimer);
    }
    else
    {
        process->WriteMemory(0xb9, 1, eCode.ZTimer);
    }
}

void PVZ_1_2_CN_REPAIR::StopZombies(bool b)
{
    if (b)
    {
        process->WriteMemory(0x54, 1, 0x0054ec57);
        process->WriteMemory(0x54, 1, 0x0054ec60);
    }
    else
    {
        process->WriteMemory(0x64, 1, 0x0054ec57);
        process->WriteMemory(0x44, 1, 0x0054ec60);
    }
}

void PVZ_1_2_CN_REPAIR::StopBoss(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc3, 1, 0x0055a320);
    }
    else
    {
        process->WriteMemory(0x51, 1, 0x0055a320);
    }
}

void PVZ_1_2_CN_REPAIR::BalloonBoom(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00545f91);
        process->WriteMemory(0x70, 1, 0x00549f6c);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00545f91);
        process->WriteMemory(0x74, 1, 0x00549f6c);
    }
}

void PVZ_1_2_CN_REPAIR::ZTimerSpeed(int value)
{
    process->WriteMemory(value, 4, eCode.ZTimer + 1);
}

void PVZ_1_2_CN_REPAIR::GatherZombie(bool b, float pos)
{
    if (b)
    {
        static unsigned char code[] = { 0xc7, 0x46, 0x2c, 0, 0, 0, 0, 0xeb, 0x23 };
        (DWORD&)code[3] = (DWORD&)pos;
        process->WriteMemory(0x32, 1, 0x0054eb6b);
        process->WriteArrayMemory(code, sizeof(code), 0x0054ec44);
    }
    else
    {
        static unsigned char code[] = { 0xe8, 0xb7, 0x13, 0, 0, 0x84, 0xc0, 0x75, 0x0f };
        process->WriteMemory(0x84, 1, 0x0054eb6b);
        process->WriteArrayMemory(code, sizeof(code), 0x0054ec44);
    }
}

void PVZ_1_2_CN_REPAIR::KillAllZombies()
{
    GetZombies();
    for (auto& rz : vz)
    {
        process->WriteMemory(3, 4, rz.addr + 0x28);
    }
}

void PVZ_1_2_CN_REPAIR::ConfuseZombies()
{
    GetZombies();
    for (auto& rz : vz)
    {
        process->WriteMemory(1, 1, rz.addr + 0xb8);
    }
}

void PVZ_1_2_CN_REPAIR::NaturalSeed(int* zombies, int znum)
{
    if (InGame())
    {
        int minzid = zombies[0];
        int minindex = 0;
        bool LowZ = (minzid == 0 || minzid == 26) ? true : false;
        for (int i = 1; i < znum; ++i)
        {
            if (zombies[i] < minzid)
            {
                minzid = zombies[i];
                minindex = i;
            }
            if (zombies[i] == 0 || zombies[i] == 26)
                LowZ = true;
        }
        if (!LowZ)
        {
            zombies[minindex] = 26;
        }
        DWORD SeedAddr = process->ReadMemory(4, 0x7578f8, 1, 0x868) + 0x54ec;
        DWORD ListAddr = process->ReadMemory(4, 0x7578f8, 1, 0x868) + 0x6cc;
        for (int i = 0; i < 34; ++i)
        {
            process->WriteMemory(0, 1, SeedAddr + i);
        }
        for (int i = 0; i < znum; ++i)
        {
            process->WriteMemory(1, 1, SeedAddr + zombies[i]);
        }
        asm_.clear();
        asm_.mov_exx_dword_ptr(EAX, 0x7578f8);
        asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
        asm_.call(0x0040e690);
        asm_.ret();
        SAFE_THREAD();
        if (!LowZ)
        {
            for (int i = 0; i < 2000; ++i)
            {
                if (process->ReadMemory(4, ListAddr + i * 4) == 26)
                    process->WriteMemory(minzid, 4, ListAddr + i * 4);
            }
        }
        if (process->ReadMemory(4, 0x7578f8, 1, 0x91c) == 2)
        {
            asm_.clear();
            asm_.mov_exx_dword_ptr(EBX, 0x7578f8);
            asm_.mov_exx_dword_ptr_exx_add(EBX, 0x868);
            asm_.call(0x00413630);
            asm_.mov_exx_dword_ptr(EAX, 0x7578f8);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x174);
            asm_.push_exx(EAX);
            asm_.call(0x00445470);
            asm_.ret();
            process->WriteMemory(0x80, 1, 0x00445488);
            SAFE_THREAD();
            process->WriteMemory(0x85, 1, 0x00445488);
        }
    }
}

void PVZ_1_2_CN_REPAIR::LimitSeed(int* zombies, int znum, bool thief, bool red)
{
    if (InGame())
    {
        DWORD SeedAddr = process->ReadMemory(4, 0x7578f8, 1, 0x868) + 0x54ec;
        DWORD ListAddr = process->ReadMemory(4, 0x7578f8, 1, 0x868) + 0x6cc;
        bool HasThief = false;
        bool HasRed = false;
        for (int i = 0; i < znum; ++i)
        {
            if (zombies[i] == 20)
                HasThief = true;
            if (zombies[i] == 32)
                HasRed = true;
        }
        int j = 0;
        for (int i = 0; i < 2000; ++i)
        {
            if (HasThief && thief && znum > 1)
            {
                if (i >= 450 && i <= 453 || i >= 950 && i <= 953 || i >= 1450 && i <= 1453 || i >= 1950 && i <= 1953)
                {
                    process->WriteMemory(20, 4, ListAddr + i * 4);
                    continue;
                }
                if (znum > 1 && zombies[j] == 20)
                    ++j;
                if (j >= znum)
                    j = 0;
            }
            if (HasRed && red && zombies[j] == 32 && znum > 1)
            {
                if (i >= 500 && i <= 950 || i >= 1500 && i <= 1950)
                    ++j;
                if (j >= znum)
                    j = 0;
                if (zombies[j] == 20 && thief)
                    ++j;
                if (j >= znum)
                    j = 0;
            }
            process->WriteMemory(zombies[j++], 4, ListAddr + i * 4);
            if (j >= znum)
                j = 0;
        }
        if (process->ReadMemory(4, 0x7578f8, 1, 0x91c) == 2)
        {
            asm_.clear();
            asm_.mov_exx_dword_ptr(EBX, 0x7578f8);
            asm_.mov_exx_dword_ptr_exx_add(EBX, 0x868);
            asm_.call(0x00413630);
            asm_.mov_exx_dword_ptr(EAX, 0x7578f8);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x174);
            asm_.push_exx(EAX);
            asm_.call(0x00445470);
            asm_.ret();
            process->WriteMemory(0x80, 1, 0x00445488);
            SAFE_THREAD();
            process->WriteMemory(0x85, 1, 0x00445488);
        }
    }
}

void PVZ_1_2_CN_REPAIR::ResetSeed()
{
    if (InGame())
    {
        DWORD addr = process->ReadMemory(4, 0x7578f8, 1, 0x868) + 0x54ec;
        for (int i = 0; i < 34; ++i)
        {
            process->WriteMemory(0, 1, addr + i);
        }
        asm_.clear();
        asm_.mov_exx_dword_ptr(ESI, 0x7578f8);
        asm_.mov_exx_dword_ptr_exx_add(ESI, 0x868);
        asm_.mov_exx_dword_ptr_exx_add(ESI, 0x178);
        asm_.call(0x0042cff0);
        asm_.mov_exx_dword_ptr(EAX, 0x7578f8);
        asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
        asm_.call(0x0040e690);
        if (process->ReadMemory(4, 0x7578f8, 1, 0x91c) == 2)
        {
            asm_.mov_exx_dword_ptr(EBX, 0x7578f8);
            asm_.mov_exx_dword_ptr_exx_add(EBX, 0x868);
            asm_.call(0x00413630);
            asm_.mov_exx_dword_ptr(EAX, 0x7578f8);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x174);
            asm_.push_exx(EAX);
            asm_.call(0x00445470);
            process->WriteMemory(0x80, 1, 0x00445488);
        }
        asm_.ret();
        SAFE_THREAD();
        process->WriteMemory(0x85, 1, 0x00445488);
    }
}

void PVZ_1_2_CN_REPAIR::SeeLeftZombies(bool (&zombies)[33])
{
    memset(zombies, 0, sizeof(zombies));
    if (InGame())
    {
        DWORD Base = process->ReadMemory(4, 0x7578f8, 1, 0x868);
        DWORD WaveCount = process->ReadMemory(4, Base + 0x557c);
        DWORD CurWave = process->ReadMemory(4, Base + 0x5594);
        if (WaveCount == 0)
            return;
        DWORD LastAddr = Base + 0x6cc + (WaveCount - 1) * 200;
        DWORD CurAddr = Base + 0x6cc + CurWave * 200;
        if (CurWave == 40)
        {
            CurAddr = LastAddr;
        }
        while (CurAddr <= LastAddr)
        {
            for (int i = 0; i < 50; ++i)
            {
                DWORD one = process->ReadMemory(4, CurAddr + i * 4);
                if (one < 0)
                    break;
                else if (one < 33)
                    zombies[one] = true;
            }
            CurAddr += 200;
        }
    }
}

void PVZ_1_2_CN_REPAIR::FlowerNPK(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x0053c864);
        process->WriteMemory(1020, 4, 0x7578f8, 2, 0x94c, 0x220);
    }
    else
    {
        process->WriteMemory(-1, 1, 0x0053c864);
    }
}

void PVZ_1_2_CN_REPAIR::Medicine(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x0053c931);
        process->WriteMemory(1020, 4, 0x7578f8, 2, 0x94c, 0x224);
    }
    else
    {
        process->WriteMemory(-1, 1, 0x0053c931);
    }
}

void PVZ_1_2_CN_REPAIR::Chocolate(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x0053c583);
        process->WriteMemory(0x39, 1, 0x0053c5ca);
        process->WriteMemory(1020, 4, 0x7578f8, 2, 0x94c, 0x250);
    }
    else
    {
        process->WriteMemory(-1, 1, 0x0053c583);
        process->WriteMemory(-1, 1, 0x0053c5ca);
    }
}

void PVZ_1_2_CN_REPAIR::TreeNPK(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x004368c9);
        process->WriteMemory(1020, 4, 0x7578f8, 2, 0x94c, 0x258);
    }
    else
    {
        process->WriteMemory(-1, 1, 0x004368c9);
    }
}

void PVZ_1_2_CN_REPAIR::TreeHeight(int h)
{
    if (process->ReadMemory(4, 0x7578f8, 1, 0x918) == 50)
    {
        process->WriteMemory(h - 1, 4, 0x7578f8, 2, 0x94c, 0x11c);
        asm_.clear();
        asm_.mov_exx_dword_ptr(EBX, 0x7578f8);
        asm_.mov_exx_dword_ptr_exx_add(EBX, 0x868);
        asm_.mov_exx_dword_ptr_exx_add(EBX, 0x178);
        asm_.push_exx(EBX);
        asm_.call(0x004366a0);
        asm_.ret();
        SAFE_THREAD();
    }
    else
        process->WriteMemory(h, 4, 0x7578f8, 2, 0x94c, 0x11c);
}

void PVZ_1_2_CN_REPAIR::FlowerNum(int n)
{
    process->WriteMemory(n, 4, 0x7578f8, 2, 0x94c, 0x378);
}

void PVZ_1_2_CN_REPAIR::ChangeFlower(int fid, int type)
{
    process->WriteMemory(type, 4, 0x7578f8, 2, 0x94c, 0x380 + fid * 0x58);
}

void PVZ_1_2_CN_REPAIR::ChangeFSize(int fid, int size)
{
    process->WriteMemory(size, 4, 0x7578f8, 2, 0x94c, 0x3a4 + fid * 0x58);
}

void PVZ_1_2_CN_REPAIR::ChangeFDir(int fid, int dir)
{
    process->WriteMemory(dir, 4, 0x7578f8, 2, 0x94c, 0x390 + fid * 0x58);
}

void PVZ_1_2_CN_REPAIR::ChangeFPlace(int fid, int place)
{
    process->WriteMemory(place, 4, 0x7578f8, 2, 0x94c, 0x384 + fid * 0x58);
}

int PVZ_1_2_CN_REPAIR::GetFlowerNum()
{
    return process->ReadMemory(4, 0x7578f8, 2, 0x94c, 0x378);
}

int PVZ_1_2_CN_REPAIR::GetFlower(int fid)
{
    return process->ReadMemory(4, 0x7578f8, 2, 0x94c, 0x380 + fid * 0x58);
}

int PVZ_1_2_CN_REPAIR::GetFSize(int fid)
{
    return process->ReadMemory(4, 0x7578f8, 2, 0x94c, 0x3a4 + fid * 0x58);
}

int PVZ_1_2_CN_REPAIR::GetFDir(int fid)
{
    return process->ReadMemory(4, 0x7578f8, 2, 0x94c, 0x390 + fid * 0x58);
}

int PVZ_1_2_CN_REPAIR::GetFPlace(int fid)
{
    return process->ReadMemory(4, 0x7578f8, 2, 0x94c, 0x384 + fid * 0x58);
}

void PVZ_1_2_CN_REPAIR::Patch()
{
    process->WriteMemory(0x0ceb, 2, 0x00423cf1);
    process->WriteMemory(0x10eb, 2, 0x00423e31);
    process->WriteMemory(0x80, 1, 0x00412750);
    process->WriteMemory(0xeb, 1, 0x00425ec8);
    process->WriteMemory(0xeb, 1, 0x00425ef9);
    process->WriteMemory(0x70, 1, 0x006b6353);

    process->WriteMemory(0x23, 1, 0x00558f12);
    process->WriteMemory(0, 2, 0x005596a0);
}

#pragma warning(pop)
