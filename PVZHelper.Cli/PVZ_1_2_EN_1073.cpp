#include "PVZ_1_2_EN_1073.h"

#pragma warning(push)
#pragma warning(disable : 4793)

#define SAFE_THREAD()                                                \
    process->WriteMemory(-2, 1, 0x005d6c6e),                         \
        Sleep(10),                                                   \
        asm_.CreateRemoteThread(process->GetHandle(), eCode.Thread), \
        process->WriteMemory(0xc8, 1, 0x005d6c6e)

bool PVZ_1_2_EN_1073::InGame()
{
    return process->ReadMemory(4, 0x729670, 1, 0x868) != 0;
}

int PVZ_1_2_EN_1073::GetMode()
{
    return process->ReadMemory(4, 0x729670, 1, 0x918);
}

void PVZ_1_2_EN_1073::SetSun(int sun)
{
    process->WriteMemory(sun, 4, 0x729670, 2, 0x868, 0x5578);
}

void PVZ_1_2_EN_1073::SetMoney(int money)
{
    process->WriteMemory(money, 4, 0x729670, 2, 0x94c, 0x50);
}

void PVZ_1_2_EN_1073::IgnoreSun(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x0041e842);
        process->WriteMemory(0x39, 1, 0x0041e846);
        process->WriteMemory(0x91, 1, 0x0041e890);
        process->WriteMemory(0x80, 1, 0x0042a502);
        process->WriteMemory(0x80, 1, 0x0042a854);
        process->WriteMemory(0xeb, 1, 0x0042e1a1);
    }
    else
    {
        process->WriteMemory(0x7f, 1, 0x0041e842);
        process->WriteMemory(0x89, 1, 0x0041e846);
        process->WriteMemory(0x9e, 1, 0x0041e890);
        process->WriteMemory(0x8f, 1, 0x0042a502);
        process->WriteMemory(0x8f, 1, 0x0042a854);
        process->WriteMemory(0x75, 1, 0x0042e1a1);
    }
}

void PVZ_1_2_EN_1073::AutoPickup(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x004342f2);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x004342f2);
    }
}

void PVZ_1_2_EN_1073::StartCar()
{
    if (InGame())
    {
        asm_.clear();
        GetCars();
        for (auto& rc : vc)
        {
            asm_.mov_exx(ESI, rc.addr);
            asm_.call(0x0045c660);
        }
        asm_.ret();
        SAFE_THREAD();
    }
}

void PVZ_1_2_EN_1073::ResetCar()
{
    if (InGame())
    {
        asm_.clear();
        GetCars();
        for (auto& rc : vc)
        {
            asm_.mov_exx(EAX, rc.addr);
            asm_.call(0x0045c5d0);
        }
        process->WriteMemory(0x60eb, 2, 0x0040e52f);
        process->WriteMemory(1, 1, 0x0040e5b7);
        process->WriteMemory(0xe1e4, 2, 0x0045b8d2);
        asm_.mov_exx_dword_ptr(EAX, 0x729670);
        asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
        asm_.push_exx(EAX);
        asm_.call(0x0040e510);
        asm_.ret();
        SAFE_THREAD();
        process->WriteMemory(0x0975, 2, 0x0040e52f);
        process->WriteMemory(0, 1, 0x0040e5b7);
        process->WriteMemory(0xe3c8, 2, 0x0045b8d2);
    }
}

void PVZ_1_2_EN_1073::asm_plant(int row, int col, int type, bool copy)
{
    if (copy)
    {
        asm_.push(type);
        asm_.push(48);
        asm_.mov_exx(EAX, row);
        asm_.push(col);
        asm_.mov_exx_dword_ptr(EBP, 0x729670);
        asm_.mov_exx_dword_ptr_exx_add(EBP, 0x868);
        asm_.push_exx(EBP);
        asm_.call(0x0040fa10);
        asm_.add_word(0xf08b); //mov esi,eax
        asm_.push_exx(EAX);
        asm_.call(0x0046a630);
        asm_.add_byte(0x58); //pop eax
    }
    else
    {
        asm_.push(-1);
        asm_.push(type);
        asm_.mov_exx(EAX, row);
        asm_.push(col);
        asm_.mov_exx_dword_ptr(EBP, 0x729670);
        asm_.mov_exx_dword_ptr_exx_add(EBP, 0x868);
        asm_.push_exx(EBP);
        asm_.call(0x0040fa10);
    }
}

void PVZ_1_2_EN_1073::asm_plant_paper(int row, int col, int type)
{
    asm_plant(row, col, type, false);
    asm_.push_exx(EAX);
    asm_.mov_exx_dword_ptr(EAX, 0x729670);
    asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
    asm_.mov_exx_dword_ptr_exx_add(EAX, 0x178);
    asm_.call(0x0042d0d0);
}

void PVZ_1_2_EN_1073::Planting(int row, int col, int type, bool copy)
{
    if (InGame())
    {
        Map_1_2_EN_1073 map(process.get());
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

void PVZ_1_2_EN_1073::NoPause(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x0045272a);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0045272a);
    }
}

void PVZ_1_2_EN_1073::asm_put_zombie(int row, int col, int type)
{
    asm_.push(col);
    asm_.push(type);
    asm_.mov_exx(EAX, row);
    asm_.mov_exx_dword_ptr(ECX, 0x729670);
    asm_.mov_exx_dword_ptr_exx_add(ECX, 0x868);
    asm_.mov_exx_dword_ptr_exx_add(ECX, 0x178);
    asm_.call(0x0042cc90);
}

void PVZ_1_2_EN_1073::PutZombie(int row, int col, int type)
{
    if (InGame())
    {
        if (type == 25)
        {
            asm_.clear();
            asm_.mov_exx_dword_ptr(EAX, 0x729670);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
            asm_.push(0);
            asm_.push(25);
            asm_.call(0x00410700);
            asm_.ret();
            SAFE_THREAD();
            return;
        }
        Map_1_2_EN_1073 map(process.get());
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

void PVZ_1_2_EN_1073::asm_put_ladder(int row, int col)
{
    asm_.mov_exx(EDI, row);
    asm_.push(col);
    asm_.mov_exx_dword_ptr(EAX, 0x729670);
    asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
    asm_.call(0x0040b870);
}

void PVZ_1_2_EN_1073::PutLadder(int row, int col)
{
    if (InGame())
    {
        Map_1_2_EN_1073 map(process.get());
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

void PVZ_1_2_EN_1073::asm_put_grave(int row, int col)
{
    asm_.mov_exx_dword_ptr(ECX, 0x729670);
    asm_.mov_exx_dword_ptr_exx_add(ECX, 0x868);
    asm_.mov_exx_dword_ptr_exx_add(ECX, 0x178);
    asm_.push_exx(ECX);
    asm_.mov_exx(EDI, row);
    asm_.mov_exx(EBX, col);
    asm_.call(0x00429120);
}

void PVZ_1_2_EN_1073::PutGrave(int row, int col)
{
    if (InGame())
    {
        Map_1_2_EN_1073 map(process.get());
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

void PVZ_1_2_EN_1073::AutoLadder()
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
        Map_1_2_EN_1073 map(process.get());
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

// void static asm_put_rake(int row, int col)
// {
// 	asm_.push(row);
// 	asm_.push(col);
// 	asm_.mov_exx_dword_ptr(EAX, 0x729670);
// 	asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
// 	//asm_.push_exx(EAX);
// 	asm_.call(0x0040e270);
// 	asm_.add_esp(8);
// }
//
// void PVZ_1_2_EN_1073::PutRake(int row, int col)
// {
// 	if (InGame())
// 	{
// 		Map_1_2_EN_1073 map(process.get());
// 		int Rows = map.GetRowCount();
// 		int Cols = 9;
// 		asm_.clear();
// 		if (row == -1 && col == -1)
// 		{
// 			for (int i = 0; i < Rows; ++i)
// 			{
// 				for (int j = 0; j < Cols; ++j)
// 				{
// 					asm_put_rake(i, j);
// 				}
// 			}
// 		}
// 		else if (row != -1 && col == -1)
// 		{
// 			for (int i = 0; i < Cols; ++i)
// 			{
// 				asm_put_rake(row, i);
// 			}
// 		}
// 		else if (row == -1 && col != -1)
// 		{
// 			for (int i = 0; i < Rows; ++i)
// 			{
// 				asm_put_rake(i, col);
// 			}
// 		}
// 		else
// 		{
// 			asm_put_rake(row, col);
// 		}
// 		asm_.ret();
// 		process->WriteMemory(0x0020810f, 4, 0x0040e291);
// 		process->WriteMemory(0x39, 1, 0x0040e3ca);
// 		process->WriteMemory(0x900c7d8b, 4, 0x0040e3d9);
// 		process->WriteMemory(0x90084d8b, 4, 0x0040e3e2);
// 		SAFE_THREAD();
// 		process->WriteMemory(0x026c840f, 4, 0x0040e291);
// 		process->WriteMemory(-1, 1, 0x0040e3ca);
// 		process->WriteMemory(0x1c247c89, 4, 0x0040e3d9);
// 		process->WriteMemory(0x10244c8b, 4, 0x0040e3e2);
// 	}
// }

void PVZ_1_2_EN_1073::SetBlock(int row, int col, int type)
{
    Map_1_2_EN_1073 map(process.get());
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

void PVZ_1_2_EN_1073::SetRow(int row, int type)
{
    Map_1_2_EN_1073 map(process.get());
    map.SetRow(row, type);
}

void PVZ_1_2_EN_1073::NoDelete(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x005cd993);
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x005cd993);
    }
}

void PVZ_1_2_EN_1073::NoSave(bool b)
{
    if (b)
    {
        process->WriteMemory(0xb1, 1, 0x0048cd63);
    }
    else
    {
        process->WriteMemory(0x89, 1, 0x0048cd63);
    }
}

void PVZ_1_2_EN_1073::Anywhere(bool b)
{
    if (b)
    {
        process->WriteMemory(0x81, 1, 0x004127f0);
        process->WriteMemory(0xeb, 1, 0x0043c030);
        process->WriteMemory(0x81, 1, 0x0042ce79);
    }
    else
    {
        process->WriteMemory(0x84, 1, 0x004127f0);
        process->WriteMemory(0x74, 1, 0x0043c030);
        process->WriteMemory(0x84, 1, 0x0042ce79);
    }
}

void PVZ_1_2_EN_1073::FastBelt(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x004257ff);
        process->WriteMemory(0x33, 1, 0x004948ae);
    }
    else
    {
        process->WriteMemory(0x8f, 1, 0x004257ff);
        process->WriteMemory(0x85, 1, 0x004948ae);
    }
}

void PVZ_1_2_EN_1073::ShowHide(bool b)
{
    if (b)
    {
        process->WriteMemory(0x38, 1, 0x00430c80);
    }
    else
    {
        process->WriteMemory(0x88, 1, 0x00430c80);
    }
}

void PVZ_1_2_EN_1073::FullFog(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc3c033, 3, 0x0041ef80);
        process->WriteMemory(0x3feb, 2, 0x0041cf66);
        process->WriteMemory(0, 1, 0x0041cfaa);
    }
    else
    {
        DWORD FogAddr = process->ReadMemory(4, 0x729670, 1, 0x868) + 0x4e0;
        for (int i = 0; i < 63; ++i)
        {
            process->WriteMemory(0, 4, FogAddr + i * 4);
        }
        process->WriteMemory(0xa4818b, 3, 0x0041ef80);
        process->WriteMemory(0x0575, 2, 0x0041cf66);
        process->WriteMemory(4, 1, 0x0041cfaa);
    }
}

void PVZ_1_2_EN_1073::SeeFog(bool b)
{
    if (b)
    {
        process->WriteMemory(0xd231, 2, 0x0041d17d);
    }
    else
    {
        process->WriteMemory(0xf23b, 2, 0x0041d17d);
    }
}

void PVZ_1_2_EN_1073::SeeVase(bool b)
{
    if (b)
    {
        static unsigned char code[] = { 0xb8, 0x32, 0, 0, 0 };
        process->WriteArrayMemory(code, sizeof(code), 0x004518aa);
    }
    else
    {
        static unsigned char code[] = { 0x85, 0xc0, 0x7e, 4, 0x48 };
        process->WriteArrayMemory(code, sizeof(code), 0x004518aa);
    }
}

void PVZ_1_2_EN_1073::JmpLevel(int level)
{
    process->WriteMemory(level, 4, 0x729670, 3, 0x868, 0x178, 0x6c);
}

void PVZ_1_2_EN_1073::MixMode(int mode, int level)
{
    if (mode == 0)
    {
        process->WriteMemory(level, 4, 0x729670, 2, 0x94c, 0x4c);
        process->WriteMemory(level, 4, 0x729670, 2, 0x868, 0x5568);
    }
    process->WriteMemory(mode, 4, 0x729670, 1, 0x918);
}

void PVZ_1_2_EN_1073::Unlock(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x0044cdf0);
        process->WriteMemory(0xeb, 1, 0x00456fa1);
        process->WriteMemory(0xc301b0, 3, 0x00404c50);
        process->WriteMemory(0, 4, 0x0071bdb0);
        process->WriteMemory(0xc301b0, 3, 0x00404d50);
        process->WriteMemory(0xeb, 1, 0x0044ce1c);
        process->WriteMemory(0xeb, 1, 0x00495163);
        process->WriteMemory(0xeb, 1, 0x004980a0);
        process->WriteMemory(0xc3c030, 3, 0x004956c0);
        process->WriteMemory(0xc301b0, 3, 0x00497050);
        process->WriteMemory(0x91, 1, 0x004575af);
        process->WriteMemory(0x70, 1, 0x0044d468);
        process->WriteMemory(0xc3c031, 3, 0x00431150);
    }
    else
    {
        process->WriteMemory(0x7f, 1, 0x0044cdf0);
        process->WriteMemory(0x7f, 1, 0x00456fa1);
        process->WriteMemory(0x8b5351, 3, 0x00404c50);
        process->WriteMemory(40, 4, 0x0071bdb0);
        process->WriteMemory(0x84808b, 3, 0x00404d50);
        process->WriteMemory(0x7f, 1, 0x0044ce1c);
        process->WriteMemory(0x7d, 1, 0x00495163);
        process->WriteMemory(0x7d, 1, 0x004980a0);
        process->WriteMemory(0xf98b57, 3, 0x004956c0);
        process->WriteMemory(0xb78b56, 3, 0x00497050);
        process->WriteMemory(0x9d, 1, 0x004575af);
        process->WriteMemory(0x7e, 1, 0x0044d468);
        process->WriteMemory(0x565351, 3, 0x00431150);
    }
}

void PVZ_1_2_EN_1073::Win()
{
    int mode = GetMode();
    if (!InGame() ||
        mode >= 1 && mode <= 15 && process->ReadMemory(4, 0x729670, 1, 0x91c != 3))
    {
        return;
    }
    asm_.clear();
    asm_.mov_exx_dword_ptr(ECX, 0x729670);
    asm_.mov_exx_dword_ptr_exx_add(ECX, 0x868);
    asm_.call(0x0040ecd0);
    asm_.ret();
    SAFE_THREAD();
}

void PVZ_1_2_EN_1073::Lock(bool b, int mode)
{
    if (b)
    {
        static const int val[] = { 24, -658, 73, 100, 122, 190, 249, 360 };
        process->WriteMemory(0x81, 1, 0x0042dd2c);
        process->WriteMemory(val[mode], 4, 0x0042dd2d);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x0042b047);
        process->WriteMemory(144, 4, 0x0042b048);
    }
}

void PVZ_1_2_EN_1073::Little(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x00533ed9);
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x00533ed9);
    }
}

void PVZ_1_2_EN_1073::Rain(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x004295fe);
        process->WriteMemory(0x70, 1, 0x00419813);
        process->WriteMemory(0xeb, 1, 0x0042721b);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x004295fe);
        process->WriteMemory(0x75, 1, 0x00419813);
        process->WriteMemory(0x74, 1, 0x0042721b);
    }
}

void PVZ_1_2_EN_1073::Storm(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x004197fb);
        process->WriteMemory(0xeb, 1, 0x0042961c);
        process->WriteMemory(0xeb, 1, 0x004295e5);
        process->WriteMemory(0xeb, 1, 0x004271cf);
        process->WriteMemory(0xeb, 1, 0x0042722f);
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x004197fb);
        process->WriteMemory(0x74, 1, 0x0042961c);
        process->WriteMemory(0x74, 1, 0x004295e5);
        process->WriteMemory(0x74, 1, 0x004271cf);
        process->WriteMemory(0x74, 1, 0x0042722f);
    }
}

void PVZ_1_2_EN_1073::Force(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00471bf6);
        process->WriteMemory(0x70, 1, 0x00472301);
        process->WriteMemory(0x70, 1, 0x0053628d);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00471bf6);
        process->WriteMemory(0x75, 1, 0x00472301);
        process->WriteMemory(0x75, 1, 0x0053628d);
    }
}

void PVZ_1_2_EN_1073::LikeCol(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x0043c24a);
        process->WriteMemory(0x80, 1, 0x004134a8);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x0043c24a);
        process->WriteMemory(0x85, 1, 0x004134a8);
    }
}

void PVZ_1_2_EN_1073::RunFast(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x004273b8);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x004273b8);
    }
}

void PVZ_1_2_EN_1073::SetCardNum(int num)
{
    if (InGame())
    {
        process->WriteMemory(0xc3, 1, 0x0041eca0);
        asm_.clear();
        asm_.mov_exx(EAX, num);
        asm_.mov_exx_dword_ptr(ESI, 0x729670);
        asm_.mov_exx_dword_ptr_exx_add(ESI, 0x868);
        asm_.mov_exx_dword_ptr_exx_add(ESI, 0x15c);
        asm_.call(0x004948d0);
        asm_.ret();
        SAFE_THREAD();
        process->WriteMemory(0x56, 1, 0x0041eca0);
    }
}

void PVZ_1_2_EN_1073::SetCard(int cid, int type, bool copy)
{
    if (copy)
    {
        process->WriteMemory(48, 4, 0x729670, 3, 0x868, 0x15c, 0x5c + cid * 0x50);
        process->WriteMemory(type, 4, 0x729670, 3, 0x868, 0x15c, 0x60 + cid * 0x50);
    }
    else
    {
        process->WriteMemory(type, 4, 0x729670, 3, 0x868, 0x15c, 0x5c + cid * 0x50);
    }
}

void PVZ_1_2_EN_1073::FSBoom(bool b)
{
    if (b)
    {
        process->WriteMemory(0x20eb, 2, 0x004206d3);
        process->WriteMemory(0x70, 1, 0x00469f62);
    }
    else
    {
        process->WriteMemory(0x5d7f, 2, 0x004206d3);
        process->WriteMemory(0x75, 1, 0x00469f62);
    }
}

void PVZ_1_2_EN_1073::FSSquash(bool b)
{
    if (b)
    {
        process->WriteMemory(0x5feb, 2, 0x004642cf);
        process->WriteMemory(0x80, 1, 0x004641f8);
        process->WriteMemory(0x5feb, 2, 0x00464107);
    }
    else
    {
        process->WriteMemory(0x047f, 2, 0x004642cf);
        process->WriteMemory(0x85, 1, 0x004641f8);
        process->WriteMemory(0x7475, 2, 0x00464107);
    }
}

void PVZ_1_2_EN_1073::FSMagnet(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x00465a83);
        process->WriteMemory(0x70, 1, 0x00465b04);
    }
    else
    {
        process->WriteMemory(0x87, 1, 0x00465a83);
        process->WriteMemory(0x74, 1, 0x00465b04);
    }
}

void PVZ_1_2_EN_1073::FSSputter(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00471b03);
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x00471b03);
    }
}

void PVZ_1_2_EN_1073::FSMouth(bool b)
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

void PVZ_1_2_EN_1073::FSPrick(bool b)
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

void PVZ_1_2_EN_1073::FSSpout(bool b)
{
    if (b)
    {
        process->WriteMemory(0x74, 1, eCode.Row + 0x1d - 0x0d);
        process->WriteMemory(0x74, 1, eCode.Col + 0x42 - 0x32);
        process->WriteMemory(0x74, 1, eCode.Col + 0x47 - 0x32);
        process->WriteMemory(0x74, 1, eCode.RHurt + 0x64 - 0x5c);
        process->WriteMemory(0x74, 1, eCode.CHurt + 0x81 - 0x79);
        process->WriteMemory(0x74, 1, eCode.CHurt + 0x86 - 0x79);
        process->WriteMemory(0x39eb, 2, 0x0046b14d);
        process->WriteMemory(0x16eb, 2, 0x0046268c);
    }
    else
    {
        process->WriteMemory(0x70, 1, eCode.Row + 0x1d - 0x0d);
        process->WriteMemory(0x70, 1, eCode.Col + 0x42 - 0x32);
        process->WriteMemory(0x70, 1, eCode.Col + 0x47 - 0x32);
        process->WriteMemory(0x70, 1, eCode.RHurt + 0x64 - 0x5c);
        process->WriteMemory(0x70, 1, eCode.CHurt + 0x81 - 0x79);
        process->WriteMemory(0x70, 1, eCode.CHurt + 0x86 - 0x79);
        process->WriteMemory(0x8c0f, 2, 0x0046b14d);
        process->WriteMemory(0x8c0f, 2, 0x0046268c);
    }
}

void PVZ_1_2_EN_1073::FSCat(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x0046b2af);
        process->WriteMemory(0x75, 1, eCode.CatR + 0xc3 - 0x9b);
        process->WriteMemory(0x74, 1, eCode.CatC + 0xd1 - 0xc8);
        process->WriteMemory(0x1beb, 2, 0x004684a0);
        static unsigned char code1[] = { 0x33, 0xc0, 0xb0, 0x03, 0xeb, 0x0d };
        process->WriteArrayMemory(code1, sizeof(code1), 0x00462440);
        process->WriteMemory(0x3f, 1, 0x004684af);
        static unsigned char _3_code[] = { 0xE9, 0x8f, 0x00, 0x00, 0x00, 0x83, 0xF8, 0x12, 0x75, 0xE9, 0x8B, 0x87, 0x90, 0x00, 0x00, 0x00, 0x83, 0xF8, 0x09, 0x74, 0xBE, 0x83, 0xF8, 0x05, 0x74, 0xB9, 0x83, 0xF8, 0x01, 0x74, 0xB4, 0xE9, 0x74, 0xFE, 0xFF, 0xFF };
        process->WriteArrayMemory(_3_code, sizeof(_3_code), 0x004684ea);
        process->WriteMemory(0xeb, 1, 0x0046adb2);
        process->WriteMemory(0x70, 1, 0x0046ae27);
        process->WriteMemory(0x70, 1, eCode.Split);
        process->WriteMemory(0xe9, 1, 0x00468608);
        process->WriteMemory(0xfffffeb0, 4, 0x00468609);
        process->WriteMemory(0x70, 1, eCode.Throw);
        process->WriteMemory(0xffffffcd, 4, 0x0046ae73);
        process->WriteMemory(0x80, 1, 0x0046ad8f);
        process->WriteMemory(0x80, 1, 0x0046ad98);
        process->WriteMemory(0x80, 1, 0x0046ada1);
        process->WriteMemory(0x80, 1, 0x0046adaa);
        process->WriteMemory(0x80, 1, 0x0046ae11);
        process->WriteMemory(0x80, 1, 0x0046ae1a);
        process->WriteMemory(0xeb, 1, 0x0046ae2c);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0046b2af);
        process->WriteMemory(0x70, 1, eCode.CatR + 0xc3 - 0x9b);
        process->WriteMemory(0xeb, 1, eCode.CatC + 0xd1 - 0xc8);
        process->WriteMemory(0x4f8b, 2, 0x004684a0);
        static unsigned char code1[] = { 0x8B, 0x40, 0x24, 0x83, 0xF8, 0x1A };
        process->WriteArrayMemory(code1, sizeof(code1), 0x00462440);
        process->WriteMemory(0x2d, 1, 0x004684af);
        static unsigned char _3_code[] = { 0x83, 0xF8, 0x12, 0x0F, 0x85, 0x8B, 0x00, 0x00, 0x00, 0x8B, 0x5F, 0x1C, 0x8B, 0x37, 0x8D, 0x53, 0xFF, 0x89, 0x54, 0x24, 0x14, 0x8B, 0x97, 0x9C, 0x00, 0x00, 0x00, 0x8B, 0xC6, 0xE8, 0x54, 0xCF, 0xFE, 0xFF, 0x8B, 0x97 };
        process->WriteArrayMemory(_3_code, sizeof(_3_code), 0x004684ea);
        process->WriteMemory(0x75, 1, 0x0046adb2);
        process->WriteMemory(0x74, 1, 0x0046ae27);
        process->WriteMemory(0xeb, 1, eCode.Split);
        process->WriteMemory(0x8b, 1, 0x00468608);
        process->WriteMemory(0x006a1c4f, 4, 0x00468609);
        process->WriteMemory(0xeb, 1, eCode.Throw);
        process->WriteMemory(0x00000193, 4, 0x0046ae73);
        process->WriteMemory(0x84, 1, 0x0046ad8f);
        process->WriteMemory(0x84, 1, 0x0046ad98);
        process->WriteMemory(0x84, 1, 0x0046ada1);
        process->WriteMemory(0x84, 1, 0x0046adaa);
        process->WriteMemory(0x84, 1, 0x0046ae11);
        process->WriteMemory(0x84, 1, 0x0046ae1a);
        process->WriteMemory(0x75, 1, 0x0046ae2c);
    }
}

void PVZ_1_2_EN_1073::CatTouch(bool b)
{
    if (b)
    {
        process->WriteMemory(0x81, 1, 0x0047157c);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x0047157c);
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
        push 0x0046b344
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
        push 0x0046B341
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
        push 0x004627A6
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
        push 0x004627A6
        label:
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
            _jmp(0x0046B188)
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
        push 0x0046B2A0
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
        cmp dword ptr[edi + 0x24], 47
        je _2
        test eax, eax
        jne _2
        _jmp(0x00468382)
        _2:
        mov ecx, [edi + 0x1c]
            push esi
            _jmp(0x004684D6)
            _1 :
            mov ecx, [edi + 0x1c]
            push esi
            push ecx
            _jmp(0x00468679)
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
        fld dword ptr ds : [0x006FE1D0]
            ret
            _1 :
        cmp eax, 28
            jne short _2
            cmp edx, 1
            je short _3
            _2 :
        fld dword ptr ds : [0x006FDBF8]
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
        mov ecx, [edi + 0x1c]
        push ecx
        push edi
        xor ecx, ecx
        _call(0x0046B080)
        test eax, eax
        jne _2
        _jmp(0x00468382)
        _2:
        push eax
            mov ecx, [edi + 0x1c]
            push 0
            push ecx
            push eax
            push edi
            _call(0x0046A8C0)
            pop eax
            mov edx, [edi + 0x1c]
            push 1
            push edx
            push eax
            push edi
            _call(0x0046A8C0)
            _jmp(0x00468382)
            _1:
        mov esi, 3
            _jmp(0x004685A6)
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
        cmp[edi + 0x55b4], ecx
        ja short _1
        dec dword ptr ds : [edi + 0x55b4]
        ret
        _1 :
        mov[edi + 0x55b4], ecx
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
        mov eax, ds:[0x729670]
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
            mov dword ptr[edi + 0x28], 0x59
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
        mov eax, ds:[0x729670]
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
        mov eax, ds:[0x729670]
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
            _jmp(0x00429fb0)
            label2 :
            ret
            int 3
            int 3
            fend :
    }
}

void PVZ_1_2_EN_1073::InitExtraCode()
{
#define WRITE_CODE(f)                                                      \
    process->WriteArrayMemory(GET_ADDR(f) + HEAD_OF(f), SIZE_OF(f), temp), \
        process->WriteArrayMemory(CallCode, sizeof(CallCode), Caller)

    bool b = (process->ReadMemory(4, 0x00429a64) != 0xcccccccc);
    DWORD temp;
    if (b)
    {
        temp = process->ReadMemory(4, 0x00429a64);
    }
    else
    {
        temp = (DWORD)VirtualAllocEx(process->GetHandle(), NULL, 4096 * 2, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (temp)
            process->WriteMemory(temp, 4, 0x00429a64);
    }
    DWORD Caller;
    /*传送门*/
    if (!b)
    {
        Caller = 0x00429b1c;
        static unsigned char CallCode[] = { 0xb9, 4, 0, 0, 0, 0xe8, 0, 0, 0, 0 };
        (DWORD&)CallCode[6] = temp - Caller - 10;
        WRITE_CODE(DoorCode);
    }
    temp += SIZE_OF(DoorCode);
    /*行判定*/
    eCode.Row = temp;
    if (!b)
    {
        Caller = 0x0046b182;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(RowCode);
    }
    temp += SIZE_OF(RowCode);
    /*列判定*/
    eCode.Col = temp;
    if (!b)
    {
        Caller = 0x0046b29a;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(ColCode);
    }
    temp += SIZE_OF(ColCode);
    /*行伤害判定*/
    eCode.RHurt = temp;
    if (!b)
    {
        Caller = 0x0046269e;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(RHurtCode);
    }
    temp += SIZE_OF(RHurtCode);
    /*列伤害判定*/
    eCode.CHurt = temp;
    if (!b)
    {
        Caller = 0x00462712;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(CHurtCode);
    }
    temp += SIZE_OF(CHurtCode);
    /*香蒲免行判定*/
    eCode.CatR = temp;
    if (!b)
    {
        Caller = 0x0046b140;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(CatRCode);
    }
    temp += SIZE_OF(CatRCode);
    /*香蒲免列判定*/
    eCode.CatC = temp;
    if (!b)
    {
        Caller = 0x0046b280;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(CatCCode);
    }
    temp += SIZE_OF(CatCCode);
    /*投手特殊处理*/
    eCode.Throw = temp;
    if (!b)
    {
        Caller = 0x00468674;
        static unsigned char CallCode[] = { 0xe9, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(ThrowCode);
    }
    temp += SIZE_OF(ThrowCode);
    /*猫式全屏子弹方向处理*/
    eCode.CatDir = temp;
    if (!b)
    {
        Caller = 0x0046ae44;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(CatDirCode);
    }
    temp += SIZE_OF(CatDirCode);
    /*裂荚处理*/
    eCode.Split = temp;
    if (!b)
    {
        Caller = 0x004685a1;
        static unsigned char CallCode[] = { 0xe9, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(SplitCode);
    }
    temp += SIZE_OF(SplitCode);
    /*刷怪倒计时*/
    eCode.ZTimer = temp;
    if (!b)
    {
        Caller = 0x004167a5;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(ZTimerCode);
    }
    temp += SIZE_OF(ZTimerCode);
    /*僵博吐球行*/
    if (!b)
    {
        Caller = 0x00545ec8;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x50, 0x90, 0x90, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(GetHeadRowsCode);
    }
    temp += SIZE_OF(GetHeadRowsCode);
    /*僵博摔车行*/
    if (!b)
    {
        Caller = 0x0054554b;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x50, 0x90, 0x90, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(GetRVRowsCode);
    }
    temp += SIZE_OF(GetRVRowsCode);
    /*传送门行*/
    if (!b)
    {
        Caller = 0x00429fe4;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(DoorRowsCode);
    }
    temp += SIZE_OF(DoorRowsCode);

    eCode.Thread = temp;
    return;
}

void PVZ_1_2_EN_1073::StartDoor(bool b)
{
    if (b)
    {
        if (InGame() && process->ReadMemory(4, 0x6a9ec0, 1, 0x91c) > 2)
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
        process->WriteMemory(0x70, 1, 0x0046b125);
        process->WriteMemory(0x70, 1, 0x004273dd);
        process->WriteMemory(0x70, 1, 0x00422af9);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0046b125);
        process->WriteMemory(0x75, 1, 0x004273dd);
        process->WriteMemory(0x75, 1, 0x00422af9);
    }
}

void PVZ_1_2_EN_1073::StableDoor(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x0042a14a);
    }
    else
    {
        process->WriteMemory(-1, 1, 0x0042a14a);
    }
}

void PVZ_1_2_EN_1073::SetDoor(int b1r, int b1c, int b2r, int b2c, int w1r, int w1c, int w2r, int w2c)
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
                asm_.call(0x004502b0);
            }
        }
        process->WriteMemory(b1c, 1, 0x00429a92);
        process->WriteMemory(b1r, 1, 0x00429a99);
        process->WriteMemory(b2c, 1, 0x00429abd);
        process->WriteMemory(b2r, 1, 0x00429ac4);
        process->WriteMemory(w1c, 1, 0x00429aed);
        process->WriteMemory(w1r, 1, 0x00429af4);
        process->WriteMemory(w2c, 1, 0x00429b18);
        process->WriteMemory(w2r, 1, 0x00429b1d);
        asm_.mov_exx_dword_ptr(EDI, 0x729670);
        asm_.mov_exx_dword_ptr_exx_add(EDI, 0x868);
        asm_.mov_exx_dword_ptr_exx_add(EDI, 0x178);
        asm_.call(0x00429a70);
        asm_.ret();
        SAFE_THREAD();
        process->WriteMemory(2, 1, 0x00429a92);
        process->WriteMemory(0, 1, 0x00429a99);
        process->WriteMemory(9, 1, 0x00429abd);
        process->WriteMemory(1, 1, 0x00429ac4);
        process->WriteMemory(9, 1, 0x00429aed);
        process->WriteMemory(3, 1, 0x00429af4);
        process->WriteMemory(2, 1, 0x00429b18);
        process->WriteMemory(4, 1, 0x00429b1d);
    }
}

void PVZ_1_2_EN_1073::CornNoWait(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x00464a0b);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x00464a0b);
    }
}

void PVZ_1_2_EN_1073::MagnetNoWait(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x00465817);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x00465817);
    }
}

void PVZ_1_2_EN_1073::PotatoNoWait(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x00463824);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x00463824);
    }
}

void PVZ_1_2_EN_1073::MouthNoWait(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00464f25);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00464f25);
    }
}

void PVZ_1_2_EN_1073::PStrong()
{
    process->WriteMemory(0x004046, 3, 0x00540681);
    process->WriteMemory(0x909090, 3, 0x0047169b);
    process->WriteMemory(0x909090, 3, 0x00471e59);
    process->WriteMemory(0xeb, 1, 0x0053f2bb);
    process->WriteMemory(0xeb, 1, 0x0041f9ef);
    process->WriteMemory(0xeb, 1, 0x00537d7a);
    process->WriteMemory(0x70, 1, 0x0046273a);
    process->WriteMemory(0, 1, 0x00462596);
}

void PVZ_1_2_EN_1073::PNormal()
{
    process->WriteMemory(0xfc4046, 3, 0x00540681);
    process->WriteMemory(0x405029, 3, 0x0047169b);
    process->WriteMemory(0x404e29, 3, 0x00471e59);
    process->WriteMemory(0x74, 1, 0x0053f2bb);
    process->WriteMemory(0x74, 1, 0x0041f9ef);
    process->WriteMemory(0x75, 1, 0x00537d7a);
    process->WriteMemory(0x75, 1, 0x0046273a);
    process->WriteMemory(-32, 1, 0x00462596);
}

void PVZ_1_2_EN_1073::PWeak()
{
    process->WriteMemory(0x004066, 3, 0x00540681);
    process->WriteMemory(0x404029, 3, 0x0047169b);
    process->WriteMemory(0x407629, 3, 0x00471e59);
    process->WriteMemory(0x74, 1, 0x0053f2bb);
    process->WriteMemory(0x74, 1, 0x0041f9ef);
    process->WriteMemory(0x75, 1, 0x00537d7a);
    process->WriteMemory(0x70, 1, 0x0046273a);
    process->WriteMemory(-32, 1, 0x00462596);
}

void PVZ_1_2_EN_1073::NoMark(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x0042057d);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0042057d);
    }
}

void PVZ_1_2_EN_1073::NoSleep(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x004617c2);
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
                    asm_.call(0x00462190);
                }
            }
            asm_.ret();
            SAFE_THREAD();
        }
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x004617c2);
    }
}

void PVZ_1_2_EN_1073::StrongWind(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x0046a071);
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x0046a071);
    }
}

void PVZ_1_2_EN_1073::StrongBean(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc3, 1, 0x00472af9);
    }
    else
    {
        process->WriteMemory(0xe9, 1, 0x00472af9);
    }
}

void PVZ_1_2_EN_1073::Umbrella(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x004201b8);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x004201b8);
    }
}

void PVZ_1_2_EN_1073::NoCD(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00491e55);
        process->WriteMemory(0xeb, 1, 0x00492e70);
    }
    else
    {
        process->WriteMemory(0x7e, 1, 0x00491e55);
        process->WriteMemory(0x75, 1, 0x00492e70);
    }
}

void PVZ_1_2_EN_1073::LockScoop(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x00414d53);
        process->WriteMemory(6, 4, 0x729670, 3, 0x868, 0x150, 0x30);
    }
    else
    {
        process->WriteMemory(0x89, 1, 0x00414d53);
    }
}

void PVZ_1_2_EN_1073::AnyMove(bool b)
{
    if (b)
    {
        process->WriteMemory(0x81, 1, 0x00423427);
        process->WriteMemory(0x81, 1, 0x00423071);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x00423427);
        process->WriteMemory(0x85, 1, 0x00423071);
    }
}

void PVZ_1_2_EN_1073::NoPurple(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc301b0, 3, 0x004205a0);
        process->WriteMemory(0xeb, 1, 0x00410dea);
    }
    else
    {
        process->WriteMemory(0x575351, 3, 0x004205a0);
        process->WriteMemory(0x74, 1, 0x00410dea);
    }
}

void PVZ_1_2_EN_1073::LockButter(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00468622);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00468622);
    }
}

void PVZ_1_2_EN_1073::LockCorn(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x00468622);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00468622);
    }
}

void PVZ_1_2_EN_1073::BoomIm()
{
    process->WriteMemory(0x80, 1, 0x0053718c);
    process->WriteMemory(0x80, 1, 0x00537c6d);
}

void PVZ_1_2_EN_1073::BoomNormal()
{
    process->WriteMemory(0x8f, 1, 0x0053718c);
    process->WriteMemory(0x85, 1, 0x00537c6d);
}

void PVZ_1_2_EN_1073::BoomNever()
{
    process->WriteMemory(0x81, 1, 0x0053718c);
    process->WriteMemory(0x81, 1, 0x00537c6d);
}

void PVZ_1_2_EN_1073::VisiableNo()
{
    process->WriteMemory(0x70, 1, 0x0053ecde);
    process->WriteMemory(0x70, 1, 0x00544a5b);
}

void PVZ_1_2_EN_1073::VisiableNormal()
{
    process->WriteMemory(0x75, 1, 0x0053ecde);
    process->WriteMemory(0x75, 1, 0x00544a5b);
}

void PVZ_1_2_EN_1073::VisiableYes()
{
    process->WriteMemory(0xeb, 1, 0x0053ecde);
    process->WriteMemory(0xeb, 1, 0x00544a5b);
}

void PVZ_1_2_EN_1073::ZStrong()
{
    process->WriteMemory(0xc0, 1, 0x005419f5);
    process->WriteMemory(0x90909090, 4, 0x00541cda);
    process->WriteMemory(0xd233c033, 4, 0x0054164b);
    process->WriteMemory(0x81, 1, 0x00543571);
    process->WriteMemory(0xeb, 1, 0x00464e08);
}

void PVZ_1_2_EN_1073::ZNormal()
{
    process->WriteMemory(0xc8, 1, 0x005419f5);
    process->WriteMemory(0x20246c2b, 4, 0x00541cda);
    process->WriteMemory(0xc28b027c, 4, 0x0054164b);
    process->WriteMemory(0x8d, 1, 0x00543571);
    process->WriteMemory(0x74, 1, 0x00464e08);
}

void PVZ_1_2_EN_1073::ZWeak()
{
    process->WriteMemory(0xc9, 1, 0x005419f5);
    process->WriteMemory(0x9090ed2b, 4, 0x00541cda);
    process->WriteMemory(0xc28bd08b, 4, 0x0054164b);
    process->WriteMemory(0x80, 1, 0x00543571);
    process->WriteMemory(0x74, 1, 0x00464e08);
}

void PVZ_1_2_EN_1073::NoSlow(bool b)
{
    if (b)
    {
        process->WriteMemory(0x90c033, 3, 0x0053bd2f);
    }
    else
    {
        process->WriteMemory(0xffc083, 3, 0x0053bd2f);
    }
}

void PVZ_1_2_EN_1073::NoButter(bool b)
{
    if (b)
    {
        process->WriteMemory(0x90c033, 3, 0x0053bd4b);
    }
    else
    {
        process->WriteMemory(0xffc083, 3, 0x0053bd4b);
    }
}

void PVZ_1_2_EN_1073::No3zGroup(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x0042940c);
        process->WriteMemory(0xeb, 1, 0x00415a02);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0042940c);
        process->WriteMemory(0x75, 1, 0x00415a02);
    }
}

void PVZ_1_2_EN_1073::NoIMP(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x00537895);
    }
    else
    {
        process->WriteMemory(0x7d, 1, 0x00537895);
    }
}

void PVZ_1_2_EN_1073::NoICE(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc3, 1, 0x0053b0b0);
        process->WriteMemory(0xeb, 1, 0x004222ea);
        for (int i = 0; i < 6; ++i)
        {
            process->WriteMemory(1, 4, 0x729670, 2, 0x868, 0x63c + i * 4);
        }
    }
    else
    {
        process->WriteMemory(0x51, 1, 0x0053b0b0);
        process->WriteMemory(0x75, 1, 0x004222ea);
    }
}

void PVZ_1_2_EN_1073::ZEatable(bool b)
{
    if (b)
    {
        process->WriteMemory(0, 1, 0x00464e15);
    }
    else
    {
        process->WriteMemory(1, 1, 0x00464e15);
    }
}

void PVZ_1_2_EN_1073::StopZTimer(bool b)
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

void PVZ_1_2_EN_1073::StopZombies(bool b)
{
    if (b)
    {
        process->WriteMemory(0x54, 1, 0x0053b433);
        process->WriteMemory(0x54, 1, 0x0053b43c);
    }
    else
    {
        process->WriteMemory(0x64, 1, 0x0053b433);
        process->WriteMemory(0x44, 1, 0x0053b43c);
    }
}

void PVZ_1_2_EN_1073::StopBoss(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc3, 1, 0x00546af0);
    }
    else
    {
        process->WriteMemory(0x51, 1, 0x00546af0);
    }
}

void PVZ_1_2_EN_1073::BalloonBoom(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x0053634b);
        process->WriteMemory(0x70, 1, 0x00536366);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0053634b);
        process->WriteMemory(0x74, 1, 0x00536366);
    }
}

void PVZ_1_2_EN_1073::ZTimerSpeed(int value)
{
    process->WriteMemory(value, 4, eCode.ZTimer + 1);
}

void PVZ_1_2_EN_1073::GatherZombie(bool b, float pos)
{
    if (b)
    {
        static unsigned char code[] = { 0xc7, 0x46, 0x2c, 0, 0, 0, 0, 0xeb, 0x23 };
        (DWORD&)code[3] = (DWORD&)pos;
        process->WriteMemory(0x32, 1, 0x0053b34a);
        process->WriteArrayMemory(code, sizeof(code), 0x0053b420);
    }
    else
    {
        static unsigned char code[] = { 0xe8, 0xab, 0x13, 0, 0, 0x84, 0xc0, 0x75, 0x0f };
        process->WriteMemory(0x84, 1, 0x0053b34a);
        process->WriteArrayMemory(code, sizeof(code), 0x0053b420);
    }
}

void PVZ_1_2_EN_1073::KillAllZombies()
{
    GetZombies();
    for (auto& rz : vz)
    {
        process->WriteMemory(3, 4, rz.addr + 0x28);
    }
}

void PVZ_1_2_EN_1073::ConfuseZombies()
{
    GetZombies();
    for (auto& rz : vz)
    {
        process->WriteMemory(1, 1, rz.addr + 0xb8);
    }
}

void PVZ_1_2_EN_1073::NaturalSeed(int* zombies, int znum)
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
        DWORD SeedAddr = process->ReadMemory(4, 0x729670, 1, 0x868) + 0x54ec;
        DWORD ListAddr = process->ReadMemory(4, 0x729670, 1, 0x868) + 0x6cc;
        for (int i = 0; i < 34; ++i)
        {
            process->WriteMemory(0, 1, SeedAddr + i);
        }
        for (int i = 0; i < znum; ++i)
        {
            process->WriteMemory(1, 1, SeedAddr + zombies[i]);
        }
        asm_.clear();
        asm_.mov_exx_dword_ptr(EDI, 0x729670);
        asm_.mov_exx_dword_ptr_exx_add(EDI, 0x868);
        asm_.call(0x0040bc20);
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
        if (process->ReadMemory(4, 0x729670, 1, 0x91c) == 2)
        {
            asm_.clear();
            asm_.mov_exx_dword_ptr(EBX, 0x729670);
            asm_.mov_exx_dword_ptr_exx_add(EBX, 0x868);
            asm_.call(0x004108e0);
            asm_.mov_exx_dword_ptr(EAX, 0x729670);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x174);
            asm_.push_exx(EAX);
            asm_.call(0x0043d350);
            asm_.ret();
            process->WriteMemory(0x80, 1, 0x0043d363);
            SAFE_THREAD();
            process->WriteMemory(0x85, 1, 0x0043d363);
        }
    }
}

void PVZ_1_2_EN_1073::LimitSeed(int* zombies, int znum, bool thief, bool red)
{
    if (InGame())
    {
        DWORD SeedAddr = process->ReadMemory(4, 0x729670, 1, 0x868) + 0x54ec;
        DWORD ListAddr = process->ReadMemory(4, 0x729670, 1, 0x868) + 0x6cc;
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
        if (process->ReadMemory(4, 0x729670, 1, 0x91c) == 2)
        {
            asm_.clear();
            asm_.mov_exx_dword_ptr(EBX, 0x729670);
            asm_.mov_exx_dword_ptr_exx_add(EBX, 0x868);
            asm_.call(0x004108e0);
            asm_.mov_exx_dword_ptr(EAX, 0x729670);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x174);
            asm_.push_exx(EAX);
            asm_.call(0x0043d350);
            asm_.ret();
            process->WriteMemory(0x80, 1, 0x0043d363);
            SAFE_THREAD();
            process->WriteMemory(0x85, 1, 0x0043d363);
        }
    }
}

void PVZ_1_2_EN_1073::ResetSeed()
{
    if (InGame())
    {
        DWORD addr = process->ReadMemory(4, 0x729670, 1, 0x868) + 0x54ec;
        for (int i = 0; i < 34; ++i)
        {
            process->WriteMemory(0, 1, addr + i);
        }
        asm_.clear();
        asm_.mov_exx_dword_ptr(ESI, 0x729670);
        asm_.mov_exx_dword_ptr_exx_add(ESI, 0x868);
        asm_.mov_exx_dword_ptr_exx_add(ESI, 0x178);
        asm_.call(0x00428370);
        asm_.mov_exx_dword_ptr(EDI, 0x729670);
        asm_.mov_exx_dword_ptr_exx_add(EDI, 0x868);
        asm_.call(0x0040bc20);
        if (process->ReadMemory(4, 0x729670, 1, 0x91c) == 2)
        {
            asm_.mov_exx_dword_ptr(EBX, 0x729670);
            asm_.mov_exx_dword_ptr_exx_add(EBX, 0x868);
            asm_.call(0x004108e0);
            asm_.mov_exx_dword_ptr(EAX, 0x729670);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x174);
            asm_.push_exx(EAX);
            asm_.call(0x0043d350);
            process->WriteMemory(0x80, 1, 0x0043d363);
        }
        asm_.ret();
        SAFE_THREAD();
        process->WriteMemory(0x85, 1, 0x0043d363);
    }
}

void PVZ_1_2_EN_1073::SeeLeftZombies(bool (&zombies)[33])
{
    memset(zombies, 0, sizeof(zombies));
    if (InGame())
    {
        DWORD Base = process->ReadMemory(4, 0x729670, 1, 0x868);
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

void PVZ_1_2_EN_1073::FlowerNPK(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x0052992b);
        process->WriteMemory(1020, 4, 0x729670, 2, 0x94c, 0x220);
    }
    else
    {
        process->WriteMemory(-1, 1, 0x0052992b);
    }
}

void PVZ_1_2_EN_1073::Medicine(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x00529a23);
        process->WriteMemory(1020, 4, 0x729670, 2, 0x94c, 0x224);
    }
    else
    {
        process->WriteMemory(-1, 1, 0x00529a23);
    }
}

void PVZ_1_2_EN_1073::Chocolate(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x00529545);
        process->WriteMemory(0x39, 1, 0x005295c7);
        process->WriteMemory(1020, 4, 0x729670, 2, 0x94c, 0x250);
    }
    else
    {
        process->WriteMemory(-1, 1, 0x00529545);
        process->WriteMemory(-1, 1, 0x005295c7);
    }
}

void PVZ_1_2_EN_1073::TreeNPK(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x0043018d);
        process->WriteMemory(1020, 4, 0x729670, 2, 0x94c, 0x258);
    }
    else
    {
        process->WriteMemory(-1, 1, 0x0043018d);
    }
}

void PVZ_1_2_EN_1073::TreeHeight(int h)
{
    if (process->ReadMemory(4, 0x729670, 1, 0x918) == 50)
    {
        process->WriteMemory(h - 1, 4, 0x729670, 2, 0x94c, 0x11c);
        asm_.clear();
        asm_.mov_exx_dword_ptr(ESI, 0x729670);
        asm_.mov_exx_dword_ptr_exx_add(ESI, 0x868);
        asm_.mov_exx_dword_ptr_exx_add(ESI, 0x178);
        asm_.call(0x0042ff00);
        asm_.ret();
        SAFE_THREAD();
    }
    else
        process->WriteMemory(h, 4, 0x729670, 2, 0x94c, 0x11c);
}

void PVZ_1_2_EN_1073::FlowerNum(int n)
{
    process->WriteMemory(n, 4, 0x729670, 2, 0x94c, 0x378);
}

void PVZ_1_2_EN_1073::ChangeFlower(int fid, int type)
{
    process->WriteMemory(type, 4, 0x729670, 2, 0x94c, 0x380 + fid * 0x58);
}

void PVZ_1_2_EN_1073::ChangeFSize(int fid, int size)
{
    process->WriteMemory(size, 4, 0x729670, 2, 0x94c, 0x3a4 + fid * 0x58);
}

void PVZ_1_2_EN_1073::ChangeFDir(int fid, int dir)
{
    process->WriteMemory(dir, 4, 0x729670, 2, 0x94c, 0x390 + fid * 0x58);
}

void PVZ_1_2_EN_1073::ChangeFPlace(int fid, int place)
{
    process->WriteMemory(place, 4, 0x729670, 2, 0x94c, 0x384 + fid * 0x58);
}

int PVZ_1_2_EN_1073::GetFlowerNum()
{
    return process->ReadMemory(4, 0x729670, 2, 0x94c, 0x378);
}

int PVZ_1_2_EN_1073::GetFlower(int fid)
{
    return process->ReadMemory(4, 0x729670, 2, 0x94c, 0x380 + fid * 0x58);
}

int PVZ_1_2_EN_1073::GetFSize(int fid)
{
    return process->ReadMemory(4, 0x729670, 2, 0x94c, 0x3a4 + fid * 0x58);
}

int PVZ_1_2_EN_1073::GetFDir(int fid)
{
    return process->ReadMemory(4, 0x729670, 2, 0x94c, 0x390 + fid * 0x58);
}

int PVZ_1_2_EN_1073::GetFPlace(int fid)
{
    return process->ReadMemory(4, 0x729670, 2, 0x94c, 0x384 + fid * 0x58);
}

void PVZ_1_2_EN_1073::Patch()
{
    process->WriteMemory(0x23, 1, 0x00545751);
    process->WriteMemory(0x23, 1, 0x00545efe);
}

#pragma warning(pop)
