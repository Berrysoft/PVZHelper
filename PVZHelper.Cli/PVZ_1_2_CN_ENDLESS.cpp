#include "PVZ_1_2_CN_ENDLESS.h"

#pragma warning(push)
#pragma warning(disable : 4793)

#define SAFE_THREAD()                                                \
    process->WriteMemory(-2, 1, 0x0062941e),                         \
        Sleep(10),                                                   \
        asm_.CreateRemoteThread(process->GetHandle(), eCode.Thread), \
        process->WriteMemory(0xc8, 1, 0x0062941e)

bool PVZ_1_2_CN_ENDLESS::InGame()
{
    return process->ReadMemory(4, 0x755e0c, 1, 0x868) != 0;
}

int PVZ_1_2_CN_ENDLESS::GetMode()
{
    return process->ReadMemory(4, 0x755e0c, 1, 0x91c);
}

void PVZ_1_2_CN_ENDLESS::SetSun(int sun)
{
    process->WriteMemory(sun, 4, 0x755e0c, 2, 0x868, 0x5578);
}

void PVZ_1_2_CN_ENDLESS::SetMoney(int money)
{
    process->WriteMemory(money, 4, 0x755e0c, 2, 0x950, 0x50);
}

void PVZ_1_2_CN_ENDLESS::IgnoreSun(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00427692);
        process->WriteMemory(0xdb, 1, 0x00427695);
        process->WriteMemory(0x91, 1, 0x004276e0);
        process->WriteMemory(0x80, 1, 0x00432cd2);
        process->WriteMemory(0x80, 1, 0x0043302d);
        process->WriteMemory(0xeb, 1, 0x0042fb6d);
    }
    else
    {
        process->WriteMemory(0x7f, 1, 0x00427692);
        process->WriteMemory(0xf3, 1, 0x00427695);
        process->WriteMemory(0x9e, 1, 0x004276e0);
        process->WriteMemory(0x8f, 1, 0x00432cd2);
        process->WriteMemory(0x8f, 1, 0x0043302d);
        process->WriteMemory(0x74, 1, 0x0042fb6d);
    }
}

void PVZ_1_2_CN_ENDLESS::AutoPickup(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x0043cc72);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0043cc72);
    }
}

void PVZ_1_2_CN_ENDLESS::StartCar()
{
    if (InGame())
    {
        asm_.clear();
        GetCars();
        for (auto& rc : vc)
        {
            asm_.mov_exx(ESI, rc.addr);
            asm_.call(0x00466cf0);
        }
        asm_.ret();
        SAFE_THREAD();
    }
}

void PVZ_1_2_CN_ENDLESS::ResetCar()
{
    if (InGame())
    {
        asm_.clear();
        GetCars();
        for (auto& rc : vc)
        {
            asm_.mov_exx(EAX, rc.addr);
            asm_.call(0x00466c60);
        }
        process->WriteMemory(0x60eb, 2, 0x004179d8);
        process->WriteMemory(1, 1, 0x00417a57);
        process->WriteMemory(0x7894, 2, 0x00465f62);
        asm_.mov_exx_dword_ptr(EAX, 0x755e0c);
        asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
        asm_.push_exx(EAX);
        asm_.call(0x004179b0);
        asm_.ret();
        SAFE_THREAD();
        process->WriteMemory(0x0975, 2, 0x004179d8);
        process->WriteMemory(0, 1, 0x00417a57);
        process->WriteMemory(0x7a78, 2, 0x00465f62);
    }
}

void PVZ_1_2_CN_ENDLESS::asm_plant(int row, int col, int type, bool copy)
{
    if (copy)
    {
        asm_.push(type);
        asm_.push(48);
        asm_.mov_exx(EAX, row);
        asm_.push(col);
        asm_.mov_exx_dword_ptr(EBP, 0x755e0c);
        asm_.mov_exx_dword_ptr_exx_add(EBP, 0x868);
        asm_.push_exx(EBP);
        asm_.call(0x00418d70);
        asm_.add_word(0xf08b); //mov esi,eax
        asm_.push_exx(EAX);
        asm_.call(0x00475050);
        asm_.add_byte(0x58); //pop eax
    }
    else
    {
        asm_.push(-1);
        asm_.push(type);
        asm_.mov_exx(EAX, row);
        asm_.push(col);
        asm_.mov_exx_dword_ptr(EBP, 0x755e0c);
        asm_.mov_exx_dword_ptr_exx_add(EBP, 0x868);
        asm_.push_exx(EBP);
        asm_.call(0x00418d70);
    }
}

void PVZ_1_2_CN_ENDLESS::asm_plant_paper(int row, int col, int type)
{
    asm_plant(row, col, type, false);
    asm_.push_exx(EAX);
    asm_.mov_exx_dword_ptr(EAX, 0x755e0c);
    asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
    asm_.mov_exx_dword_ptr_exx_add(EAX, 0x178);
    asm_.call(0x004357d0);
}

void PVZ_1_2_CN_ENDLESS::Planting(int row, int col, int type, bool copy)
{
    if (InGame())
    {
        Map_1_2_CN_ENDLESS map(process);
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

void PVZ_1_2_CN_ENDLESS::NoPause(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc3, 1, 0x0045bfc0);
    }
    else
    {
        process->WriteMemory(0x56, 1, 0x0045bfc0);
    }
}

void PVZ_1_2_CN_ENDLESS::asm_put_zombie(int row, int col, int type)
{
    asm_.push(col);
    asm_.push(type);
    asm_.mov_exx(EAX, row);
    asm_.mov_exx_dword_ptr(ECX, 0x755e0c);
    asm_.mov_exx_dword_ptr_exx_add(ECX, 0x868);
    asm_.mov_exx_dword_ptr_exx_add(ECX, 0x178);
    asm_.call(0x00435390);
}

void PVZ_1_2_CN_ENDLESS::PutZombie(int row, int col, int type)
{
    if (InGame())
    {
        if (type == 25)
        {
            asm_.clear();
            asm_.mov_exx_dword_ptr(EAX, 0x755e0c);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
            asm_.push(0);
            asm_.push(25);
            asm_.call(0x00419a60);
            asm_.ret();
            SAFE_THREAD();
            return;
        }
        Map_1_2_CN_ENDLESS map(process);
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

void PVZ_1_2_CN_ENDLESS::asm_put_ladder(int row, int col)
{
    asm_.mov_exx(EDI, row);
    asm_.push(col);
    asm_.mov_exx_dword_ptr(EAX, 0x755e0c);
    asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
    asm_.call(0x00414d10);
}

void PVZ_1_2_CN_ENDLESS::PutLadder(int row, int col)
{
    if (InGame())
    {
        Map_1_2_CN_ENDLESS map(process);
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

void PVZ_1_2_CN_ENDLESS::asm_put_grave(int row, int col)
{
    asm_.mov_exx_dword_ptr(ECX, 0x755e0c);
    asm_.mov_exx_dword_ptr_exx_add(ECX, 0x868);
    asm_.mov_exx_dword_ptr_exx_add(ECX, 0x178);
    asm_.push_exx(ECX);
    asm_.mov_exx(EDI, row);
    asm_.mov_exx(EBX, col);
    asm_.call(0x00431900);
}

void PVZ_1_2_CN_ENDLESS::PutGrave(int row, int col)
{
    if (InGame())
    {
        Map_1_2_CN_ENDLESS map(process);
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

void PVZ_1_2_CN_ENDLESS::AutoLadder()
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
        Map_1_2_CN_ENDLESS map(process);
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
// 	asm_.mov_exx_dword_ptr(EAX, 0x755e0c);
// 	asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
// 	//asm_.push_exx(EAX);
// 	asm_.call(0x0040e270);
// 	asm_.add_esp(8);
// }
//
// void PVZ_1_2_CN_ENDLESS::PutRake(int row, int col)
// {
// 	if (InGame())
// 	{
// 		Map_1_2_CN_ENDLESS map(process);
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

void PVZ_1_2_CN_ENDLESS::SetBlock(int row, int col, int type)
{
    Map_1_2_CN_ENDLESS map(process);
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

void PVZ_1_2_CN_ENDLESS::SetRow(int row, int type)
{
    Map_1_2_CN_ENDLESS map(process);
    map.SetRow(row, type);
}

void PVZ_1_2_CN_ENDLESS::NoDelete(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00620143);
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x00620143);
    }
}

void PVZ_1_2_CN_ENDLESS::NoSave(bool b)
{
    if (b)
    {
        process->WriteMemory(0x3a, 1, 0x0061f8c1);
    }
    else
    {
        process->WriteMemory(0x3b, 1, 0x0061f8c1);
    }
}

void PVZ_1_2_CN_ENDLESS::Anywhere(bool b)
{
    if (b)
    {
        process->WriteMemory(0x81, 1, 0x0041bd2e);
        process->WriteMemory(0xeb, 1, 0x00444790);
        process->WriteMemory(0x81, 1, 0x00435579);
    }
    else
    {
        process->WriteMemory(0x84, 1, 0x0041bd2e);
        process->WriteMemory(0x74, 1, 0x00444790);
        process->WriteMemory(0x84, 1, 0x00435579);
    }
}

void PVZ_1_2_CN_ENDLESS::FastBelt(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x0042e58f);
        process->WriteMemory(0x33, 1, 0x0049f6fe);
    }
    else
    {
        process->WriteMemory(0x8f, 1, 0x0042e58f);
        process->WriteMemory(0x85, 1, 0x0049f6fe);
    }
}

void PVZ_1_2_CN_ENDLESS::ShowHide(bool b)
{
    if (b)
    {
        process->WriteMemory(0x38, 1, 0x0043935a);
    }
    else
    {
        process->WriteMemory(0x88, 1, 0x0043935a);
    }
}

void PVZ_1_2_CN_ENDLESS::FullFog(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc3c033, 3, 0x00427dd0);
        process->WriteMemory(0x3feb, 2, 0x00425f56);
        process->WriteMemory(0, 1, 0x00425f9a);
    }
    else
    {
        DWORD FogAddr = process->ReadMemory(4, 0x755e0c, 1, 0x868) + 0x4e0;
        for (int i = 0; i < 63; ++i)
        {
            process->WriteMemory(0, 4, FogAddr + i * 4);
        }
        process->WriteMemory(0xa4818b, 3, 0x00427dd0);
        process->WriteMemory(0x0575, 2, 0x00425f56);
        process->WriteMemory(4, 1, 0x00425f9a);
    }
}

void PVZ_1_2_CN_ENDLESS::SeeFog(bool b)
{
    if (b)
    {
        process->WriteMemory(0xd231, 2, 0x0042616d);
    }
    else
    {
        process->WriteMemory(0xf23b, 2, 0x0042616d);
    }
}

void PVZ_1_2_CN_ENDLESS::SeeVase(bool b)
{
    if (b)
    {
        static unsigned char code[] = { 0xb8, 0x32, 0, 0, 0 };
        process->WriteArrayMemory(code, sizeof(code), 0x00459c1a);
    }
    else
    {
        static unsigned char code[] = { 0x85, 0xc0, 0x7e, 4, 0x48 };
        process->WriteArrayMemory(code, sizeof(code), 0x00459c1a);
    }
}

void PVZ_1_2_CN_ENDLESS::JmpLevel(int level)
{
    process->WriteMemory(level, 4, 0x755e0c, 3, 0x868, 0x178, 0x6c);
}

void PVZ_1_2_CN_ENDLESS::MixMode(int mode, int level)
{
    if (mode == 0)
    {
        process->WriteMemory(level, 4, 0x755e0c, 2, 0x94c, 0x4c);
        process->WriteMemory(level, 4, 0x755e0c, 2, 0x868, 0x5568);
    }
    process->WriteMemory(mode, 4, 0x755e0c, 1, 0x91c);
}

void PVZ_1_2_CN_ENDLESS::Unlock(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x00454f4a);
        process->WriteMemory(0xeb, 1, 0x00460ec1);
        process->WriteMemory(0xc301b0, 0, 0x0040cb70);
        process->WriteMemory(0, 4, 0x00747d90);
        process->WriteMemory(0xc301b0, 3, 0x0040cc70);
        process->WriteMemory(0xeb, 1, 0x00454f6d);
        process->WriteMemory(0xeb, 1, 0x0049ffb4);
        process->WriteMemory(0xeb, 1, 0x004a2e3a);
        process->WriteMemory(0xc3c030, 3, 0x004a0520);
        process->WriteMemory(0xc301b0, 3, 0x004a1dd0);
        process->WriteMemory(0x70, 1, 0x004614f3);
        process->WriteMemory(0x70, 1, 0x00455630);
        process->WriteMemory(0xc3c031, 3, 0x004397f0);
    }
    else
    {
        process->WriteMemory(0x7f, 1, 0x00454f4a);
        process->WriteMemory(0x7f, 1, 0x00460ec1);
        process->WriteMemory(0x8b5351, 3, 0x0040cb70);
        process->WriteMemory(40, 4, 0x00747d90);
        process->WriteMemory(0x88808b, 3, 0x0040cc70);
        process->WriteMemory(0x7f, 1, 0x00454f6d);
        process->WriteMemory(0x7f, 1, 0x0049ffb4);
        process->WriteMemory(0x7d, 1, 0x004a2e3a);
        process->WriteMemory(0xf98b57, 3, 0x004a0520);
        process->WriteMemory(0xb78b56, 3, 0x004a1dd0);
        process->WriteMemory(0x7e, 1, 0x004614f3);
        process->WriteMemory(0x7e, 1, 0x00455630);
        process->WriteMemory(0x565351, 3, 0x004397f0);
    }
}

void PVZ_1_2_CN_ENDLESS::Win()
{
    int mode = GetMode();
    if (!InGame() ||
        mode >= 1 && mode <= 15 && process->ReadMemory(4, 0x755e0c, 1, 0x920 != 3))
    {
        return;
    }
    asm_.clear();
    asm_.mov_exx_dword_ptr(ECX, 0x755e0c);
    asm_.mov_exx_dword_ptr_exx_add(ECX, 0x868);
    asm_.call(0x00418140);
    asm_.ret();
    SAFE_THREAD();
}

void PVZ_1_2_CN_ENDLESS::Lock(bool b, int mode)
{
    if (b)
    {
        static const int val[] = { 24, -676, 73, 100, 122, 190, 249, 360 };
        process->WriteMemory(0x81, 1, 0x0043643e);
        process->WriteMemory(val[mode], 4, 0x0043643f);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x0043643e);
        process->WriteMemory(144, 4, 0x0043643f);
    }
}

void PVZ_1_2_CN_ENDLESS::Little(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x0053f2c9);
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x0053f2c9);
    }
}

void PVZ_1_2_CN_ENDLESS::Rain(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00431dde);
        process->WriteMemory(0x70, 1, 0x004223d9);
        process->WriteMemory(0xeb, 1, 0x0042f9fb);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00431dde);
        process->WriteMemory(0x75, 1, 0x004223d9);
        process->WriteMemory(0x74, 1, 0x0042f9fb);
    }
}

void PVZ_1_2_CN_ENDLESS::Storm(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x00431dfc);
        process->WriteMemory(0xeb, 1, 0x00431dc5);
        process->WriteMemory(0xeb, 1, 0x004223c1);
        process->WriteMemory(0xeb, 1, 0x0042f9af);
        process->WriteMemory(0xeb, 1, 0x0042fa0f);
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x00431dfc);
        process->WriteMemory(0x74, 1, 0x00431dc5);
        process->WriteMemory(0x74, 1, 0x004223c1);
        process->WriteMemory(0x74, 1, 0x0042f9af);
        process->WriteMemory(0x74, 1, 0x0042fa0f);
    }
}

void PVZ_1_2_CN_ENDLESS::Force(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x0047c936);
        process->WriteMemory(0x70, 1, 0x0047d041);
        process->WriteMemory(0x70, 1, 0x0054167d);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0047c936);
        process->WriteMemory(0x75, 1, 0x0047d041);
        process->WriteMemory(0x75, 1, 0x0054167d);
    }
}

void PVZ_1_2_CN_ENDLESS::LikeCol(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x004449aa);
        process->WriteMemory(0x80, 1, 0x0041ca10);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x004449aa);
        process->WriteMemory(0x85, 1, 0x0041ca10);
    }
}

void PVZ_1_2_CN_ENDLESS::RunFast(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x0042fb99);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0042fb99);
    }
}

void PVZ_1_2_CN_ENDLESS::SetCardNum(int num)
{
    if (InGame())
    {
        process->WriteMemory(0xc3, 1, 0x00427af0);
        asm_.clear();
        asm_.mov_exx(EAX, num);
        asm_.mov_exx_dword_ptr(ESI, 0x755e0c);
        asm_.mov_exx_dword_ptr_exx_add(ESI, 0x868);
        asm_.mov_exx_dword_ptr_exx_add(ESI, 0x15c);
        asm_.call(0x0049f720);
        asm_.ret();
        SAFE_THREAD();
        process->WriteMemory(0x56, 1, 0x00427af0);
    }
}

void PVZ_1_2_CN_ENDLESS::SetCard(int cid, int type, bool copy)
{
    if (copy)
    {
        process->WriteMemory(48, 4, 0x755e0c, 3, 0x868, 0x15c, 0x5c + cid * 0x50);
        process->WriteMemory(type, 4, 0x755e0c, 3, 0x868, 0x15c, 0x60 + cid * 0x50);
    }
    else
    {
        process->WriteMemory(type, 4, 0x755e0c, 3, 0x868, 0x15c, 0x5c + cid * 0x50);
    }
}

void PVZ_1_2_CN_ENDLESS::FSBoom(bool b)
{
    if (b)
    {
        process->WriteMemory(0x24eb, 2, 0x00429523);
        process->WriteMemory(0x70, 1, 0x00474962);
    }
    else
    {
        process->WriteMemory(0x5d7f, 2, 0x00429523);
        process->WriteMemory(0x75, 1, 0x00474962);
    }
}

void PVZ_1_2_CN_ENDLESS::FSSquash(bool b)
{
    if (b)
    {
        process->WriteMemory(0x45eb, 2, 0x0046ece9);
        process->WriteMemory(0x80, 1, 0x0046ebf8);
        process->WriteMemory(0x5feb, 2, 0x0046eb07);
    }
    else
    {
        process->WriteMemory(0x707f, 2, 0x0046ece9);
        process->WriteMemory(0x85, 1, 0x0046ebf8);
        process->WriteMemory(0x7475, 2, 0x0046eb07);
    }
}

void PVZ_1_2_CN_ENDLESS::FSMagnet(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x00470483);
        process->WriteMemory(0x70, 1, 0x00470504);
    }
    else
    {
        process->WriteMemory(0x87, 1, 0x00470483);
        process->WriteMemory(0x74, 1, 0x00470504);
    }
}

void PVZ_1_2_CN_ENDLESS::FSSputter(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x0047c843);
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x0047c843);
    }
}

void PVZ_1_2_CN_ENDLESS::FSMouth(bool b)
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

void PVZ_1_2_CN_ENDLESS::FSPrick(bool b)
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

void PVZ_1_2_CN_ENDLESS::FSSpout(bool b)
{
    if (b)
    {
        process->WriteMemory(0x74, 1, eCode.Row + 0x1d - 0x0d);
        process->WriteMemory(0x74, 1, eCode.Col + 0x42 - 0x32);
        process->WriteMemory(0x74, 1, eCode.Col + 0x47 - 0x32);
        process->WriteMemory(0x74, 1, eCode.RHurt + 0x64 - 0x5c);
        process->WriteMemory(0x74, 1, eCode.CHurt + 0x81 - 0x79);
        process->WriteMemory(0x74, 1, eCode.CHurt + 0x86 - 0x79);
        process->WriteMemory(0x39eb, 2, 0x00475b6d);
        process->WriteMemory(0x16eb, 2, 0x0046d08c);
    }
    else
    {
        process->WriteMemory(0x70, 1, eCode.Row + 0x1d - 0x0d);
        process->WriteMemory(0x70, 1, eCode.Col + 0x42 - 0x32);
        process->WriteMemory(0x70, 1, eCode.Col + 0x47 - 0x32);
        process->WriteMemory(0x70, 1, eCode.RHurt + 0x64 - 0x5c);
        process->WriteMemory(0x70, 1, eCode.CHurt + 0x81 - 0x79);
        process->WriteMemory(0x70, 1, eCode.CHurt + 0x86 - 0x79);
        process->WriteMemory(0x8c0f, 2, 0x00475b6d);
        process->WriteMemory(0x8c0f, 2, 0x0046d08c);
    }
}

void PVZ_1_2_CN_ENDLESS::FSCat(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00475ccf);
        process->WriteMemory(0x75, 1, eCode.CatR + 0xc3 - 0x9b);
        process->WriteMemory(0x74, 1, eCode.CatC + 0xd1 - 0xc8);
        process->WriteMemory(0x1beb, 2, 0x00472ea0);
        static unsigned char code1[] = { 0x33, 0xc0, 0xb0, 0x03, 0xeb, 0x0d };
        process->WriteArrayMemory(code1, sizeof(code1), 0x0046ce40);
        process->WriteMemory(0x3f, 1, 0x00472eaf);
        static unsigned char _3_code[] = { 0xE9, 0x8f, 0x00, 0x00, 0x00, 0x83, 0xF8, 0x12, 0x75, 0xE9, 0x8B, 0x87, 0x90, 0x00, 0x00, 0x00, 0x83, 0xF8, 0x09, 0x74, 0xBE, 0x83, 0xF8, 0x05, 0x74, 0xB9, 0x83, 0xF8, 0x01, 0x74, 0xB4, 0xE9, 0x74, 0xFE, 0xFF, 0xFF };
        process->WriteArrayMemory(_3_code, sizeof(_3_code), 0x00472eea);
        process->WriteMemory(0xeb, 1, 0x004757d2);
        process->WriteMemory(0x70, 1, 0x00475847);
        process->WriteMemory(0x70, 1, eCode.Split);
        process->WriteMemory(0xe9, 1, 0x00473008);
        process->WriteMemory(0xfffffeb0, 4, 0x00473009);
        process->WriteMemory(0x70, 1, eCode.Throw);
        process->WriteMemory(0xffffffcd, 4, 0x00475893);
        process->WriteMemory(0x80, 1, 0x004757af);
        process->WriteMemory(0x80, 1, 0x004757b8);
        process->WriteMemory(0x80, 1, 0x004757c1);
        process->WriteMemory(0x80, 1, 0x004757ca);
        process->WriteMemory(0x80, 1, 0x00475831);
        process->WriteMemory(0x80, 1, 0x0047583a);
        process->WriteMemory(0xeb, 1, 0x0047584c);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00475ccf);
        process->WriteMemory(0x70, 1, eCode.CatR + 0xc3 - 0x9b);
        process->WriteMemory(0xeb, 1, eCode.CatC + 0xd1 - 0xc8);
        process->WriteMemory(0x4f8b, 2, 0x00472ea0);
        static unsigned char code1[] = { 0x8B, 0x40, 0x24, 0x83, 0xF8, 0x1A };
        process->WriteArrayMemory(code1, sizeof(code1), 0x0046ce40);
        process->WriteMemory(0x2d, 1, 0x00472eaf);
        static unsigned char _3_code[] = { 0x83, 0xF8, 0x12, 0x0F, 0x85, 0x8B, 0x00, 0x00, 0x00, 0x8B, 0x5F, 0x1C, 0x8B, 0x37, 0x8D, 0x53, 0xFF, 0x89, 0x54, 0x24, 0x14, 0x8B, 0x97, 0x9C, 0x00, 0x00, 0x00, 0x8B, 0xC6, 0xE8, 0x74, 0xE1, 0xFE, 0xFF, 0x8B, 0x97 };
        process->WriteArrayMemory(_3_code, sizeof(_3_code), 0x00472eea);
        process->WriteMemory(0x75, 1, 0x004757d2);
        process->WriteMemory(0x74, 1, 0x00475847);
        process->WriteMemory(0xeb, 1, eCode.Split);
        process->WriteMemory(0x8b, 1, 0x00473008);
        process->WriteMemory(0x006a1c4f, 4, 0x00473009);
        process->WriteMemory(0xeb, 1, eCode.Throw);
        process->WriteMemory(0x00000193, 4, 0x00475893);
        process->WriteMemory(0x84, 1, 0x004757af);
        process->WriteMemory(0x84, 1, 0x004757b8);
        process->WriteMemory(0x84, 1, 0x004757c1);
        process->WriteMemory(0x84, 1, 0x004757ca);
        process->WriteMemory(0x84, 1, 0x00475831);
        process->WriteMemory(0x84, 1, 0x0047583a);
        process->WriteMemory(0x75, 1, 0x0047584c);
    }
}

void PVZ_1_2_CN_ENDLESS::CatTouch(bool b)
{
    if (b)
    {
        process->WriteMemory(0x81, 1, 0x0047c2bc);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x0047c2bc);
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
        push 0x00475D64
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
        push 0x00475D61
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
        push 0x0046D1A6
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
        push 0x0046D1A6
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
            _jmp(0x00475BA8)
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
        push 0x00475CC0
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
        _jmp(0x00472D82)
        _2:
        mov ecx, [ebx + 0x1c]
            push esi
            _jmp(0x00472ED6)
            _1 :
            mov ecx, [ebx + 0x1c]
            push esi
            push ecx
            _jmp(0x00473079)
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
        fld dword ptr ds : [0x00727880]
            ret
            _1 :
        cmp eax, 28
            jne short _2
            cmp edx, 1
            je short _3
            _2 :
        fld dword ptr ds : [0x00727298]
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
        _call(0x00475AA0)
        test eax, eax
        jne _2
        _jmp(0x00472D82)
        _2:
        push eax
            mov ecx, [edi + 0x1c]
            push 0
            push ecx
            push eax
            push edi
            _call(0x004752E0)
            pop eax
            mov edx, [edi + 0x1c]
            push 1
            push edx
            push eax
            push edi
            _call(0x004752E0)
            _jmp(0x00472D82)
            _1:
        mov esi, 3
            _jmp(0x00472FA6)
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
        mov eax, ds:[0x755e0c]
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
        mov eax, ds:[0x755e0c]
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
        mov eax, ds:[0x755e0c]
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
            _jmp(0x00432790)
            label2 :
            ret
            int 3
            int 3
            fend :
    }
}

void PVZ_1_2_CN_ENDLESS::InitExtraCode()
{
#define WRITE_CODE(f)                                                      \
    process->WriteArrayMemory(GET_ADDR(f) + HEAD_OF(f), SIZE_OF(f), temp), \
        process->WriteArrayMemory(CallCode, sizeof(CallCode), Caller)

    bool b = (process->ReadMemory(4, 0x00432244) != 0xcccccccc);
    DWORD temp;
    if (b)
    {
        temp = process->ReadMemory(4, 0x00432244);
    }
    else
    {
        temp = (DWORD)VirtualAllocEx(process->GetHandle(), NULL, 4096 * 2, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (temp)
            process->WriteMemory(temp, 4, 0x00432244);
    }
    DWORD Caller;
    /*传送门*/
    if (!b)
    {
        Caller = 0x004322fc;
        static unsigned char CallCode[] = { 0xb9, 4, 0, 0, 0, 0xe8, 0, 0, 0, 0 };
        (DWORD&)CallCode[6] = temp - Caller - 10;
        WRITE_CODE(DoorCode);
    }
    temp += SIZE_OF(DoorCode);
    /*行判定*/
    eCode.Row = temp;
    if (!b)
    {
        Caller = 0x00475ba2;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(RowCode);
    }
    temp += SIZE_OF(RowCode);
    /*列判定*/
    eCode.Col = temp;
    if (!b)
    {
        Caller = 0x00475cba;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(ColCode);
    }
    temp += SIZE_OF(ColCode);
    /*行伤害判定*/
    eCode.RHurt = temp;
    if (!b)
    {
        Caller = 0x0046d09e;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(RHurtCode);
    }
    temp += SIZE_OF(RHurtCode);
    /*列伤害判定*/
    eCode.CHurt = temp;
    if (!b)
    {
        Caller = 0x0046d112;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(CHurtCode);
    }
    temp += SIZE_OF(CHurtCode);
    /*香蒲免行判定*/
    eCode.CatR = temp;
    if (!b)
    {
        Caller = 0x00475b60;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(CatRCode);
    }
    temp += SIZE_OF(CatRCode);
    /*香蒲免列判定*/
    eCode.CatC = temp;
    if (!b)
    {
        Caller = 0x00475ca0;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(CatCCode);
    }
    temp += SIZE_OF(CatCCode);
    /*投手特殊处理*/
    eCode.Throw = temp;
    if (!b)
    {
        Caller = 0x00473074;
        static unsigned char CallCode[] = { 0xe9, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(ThrowCode);
    }
    temp += SIZE_OF(ThrowCode);
    /*猫式全屏子弹方向处理*/
    eCode.CatDir = temp;
    if (!b)
    {
        Caller = 0x00475864;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(CatDirCode);
    }
    temp += SIZE_OF(CatDirCode);
    /*裂荚处理*/
    eCode.Split = temp;
    if (!b)
    {
        Caller = 0x00472fa1;
        static unsigned char CallCode[] = { 0xe9, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(SplitCode);
    }
    temp += SIZE_OF(SplitCode);
    /*刷怪倒计时*/
    eCode.ZTimer = temp;
    if (!b)
    {
        Caller = 0x0041ff1c;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(ZTimerCode);
    }
    temp += SIZE_OF(ZTimerCode);
    /*僵博吐球行*/
    if (!b)
    {
        Caller = 0x005512a8;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x50, 0x90, 0x90, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(GetHeadRowsCode);
    }
    temp += SIZE_OF(GetHeadRowsCode);
    /*僵博摔车行*/
    if (!b)
    {
        Caller = 0x0055092b;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x50, 0x90, 0x90, 0x90 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(GetRVRowsCode);
    }
    temp += SIZE_OF(GetRVRowsCode);
    /*传送门行*/
    if (!b)
    {
        Caller = 0x004327c4;
        static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
        (DWORD&)CallCode[1] = temp - Caller - 5;
        WRITE_CODE(DoorRowsCode);
    }
    temp += SIZE_OF(DoorRowsCode);

    eCode.Thread = temp;
    return;
}

void PVZ_1_2_CN_ENDLESS::StartDoor(bool b)
{
    if (b)
    {
        if (InGame() && process->ReadMemory(4, 0x755e0c, 1, 0x920) > 2)
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
        process->WriteMemory(0x70, 1, 0x00475b45);
        process->WriteMemory(0x70, 1, 0x0042fbbe);
        process->WriteMemory(0x70, 1, 0x0042b8c1);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00475b45);
        process->WriteMemory(0x75, 1, 0x0042fbbe);
        process->WriteMemory(0x75, 1, 0x0042b8c1);
    }
}

void PVZ_1_2_CN_ENDLESS::StableDoor(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x0043292d);
    }
    else
    {
        process->WriteMemory(-1, 1, 0x0043292d);
    }
}

void PVZ_1_2_CN_ENDLESS::SetDoor(int b1r, int b1c, int b2r, int b2c, int w1r, int w1c, int w2r, int w2c)
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
                asm_.call(0x00458620);
            }
        }
        process->WriteMemory(b1c, 1, 0x00432272);
        process->WriteMemory(b1r, 1, 0x00432279);
        process->WriteMemory(b2c, 1, 0x0043229d);
        process->WriteMemory(b2r, 1, 0x004322a4);
        process->WriteMemory(w1c, 1, 0x004322cd);
        process->WriteMemory(w1r, 1, 0x004322d4);
        process->WriteMemory(w2c, 1, 0x004322f8);
        process->WriteMemory(w2r, 1, 0x004322fd);
        asm_.mov_exx_dword_ptr(EDI, 0x755e0c);
        asm_.mov_exx_dword_ptr_exx_add(EDI, 0x868);
        asm_.mov_exx_dword_ptr_exx_add(EDI, 0x178);
        asm_.call(0x00432250);
        asm_.ret();
        SAFE_THREAD();
        process->WriteMemory(2, 1, 0x00432272);
        process->WriteMemory(0, 1, 0x00432279);
        process->WriteMemory(9, 1, 0x0043229d);
        process->WriteMemory(1, 1, 0x004322a4);
        process->WriteMemory(9, 1, 0x004322cd);
        process->WriteMemory(3, 1, 0x004322d4);
        process->WriteMemory(2, 1, 0x004322f8);
        process->WriteMemory(4, 1, 0x004322fd);
    }
}

void PVZ_1_2_CN_ENDLESS::CornNoWait(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x0046f40b);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x0046f40b);
    }
}

void PVZ_1_2_CN_ENDLESS::MagnetNoWait(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x00470217);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x00470217);
    }
}

void PVZ_1_2_CN_ENDLESS::PotatoNoWait(bool b)
{
    if (b)
    {
        process->WriteMemory(0x80, 1, 0x0046e224);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x0046e224);
    }
}

void PVZ_1_2_CN_ENDLESS::MouthNoWait(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x0046f925);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0046f925);
    }
}

void PVZ_1_2_CN_ENDLESS::PStrong()
{
    process->WriteMemory(0x004046, 3, 0x0054ba6b);
    process->WriteMemory(0x909090, 3, 0x0047c3db);
    process->WriteMemory(0x909090, 3, 0x0047cb99);
    process->WriteMemory(0xeb, 1, 0x0054a6ab);
    process->WriteMemory(0xeb, 1, 0x0042883f);
    process->WriteMemory(0xeb, 1, 0x0054316a);
    process->WriteMemory(0x70, 1, 0x0046d13a);
    process->WriteMemory(0, 1, 0x0046cf96);
}

void PVZ_1_2_CN_ENDLESS::PNormal()
{
    process->WriteMemory(0xfc4046, 3, 0x0054ba6b);
    process->WriteMemory(0x405029, 3, 0x0047c3db);
    process->WriteMemory(0x404e29, 3, 0x0047cb99);
    process->WriteMemory(0x74, 1, 0x0054a6ab);
    process->WriteMemory(0x74, 1, 0x0042883f);
    process->WriteMemory(0x75, 1, 0x0054316a);
    process->WriteMemory(0x75, 1, 0x0046d13a);
    process->WriteMemory(-32, 1, 0x0046cf96);
}

void PVZ_1_2_CN_ENDLESS::PWeak()
{
    process->WriteMemory(0x004066, 3, 0x0054ba6b);
    process->WriteMemory(0x404029, 3, 0x0047c3db);
    process->WriteMemory(0x407629, 3, 0x0047cb99);
    process->WriteMemory(0x74, 1, 0x0054a6ab);
    process->WriteMemory(0x74, 1, 0x0042883f);
    process->WriteMemory(0x75, 1, 0x0054316a);
    process->WriteMemory(0x70, 1, 0x0046d13a);
    process->WriteMemory(-32, 1, 0x0046cf96);
}

void PVZ_1_2_CN_ENDLESS::NoMark(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x004293cd);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x004293cd);
    }
}

void PVZ_1_2_CN_ENDLESS::NoSleep(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x0046c1c2);
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
                    asm_.call(0x0046cb90);
                }
            }
            asm_.ret();
            SAFE_THREAD();
        }
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x0046c1c2);
    }
}

void PVZ_1_2_CN_ENDLESS::StrongWind(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x00474a71);
    }
    else
    {
        process->WriteMemory(0x74, 1, 0x00474a71);
    }
}

void PVZ_1_2_CN_ENDLESS::StrongBean(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc3, 1, 0x0047d839);
    }
    else
    {
        process->WriteMemory(0xe9, 1, 0x0047d839);
    }
}

void PVZ_1_2_CN_ENDLESS::Umbrella(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00429008);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00429008);
    }
}

void PVZ_1_2_CN_ENDLESS::NoCD(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x0049ce02);
        process->WriteMemory(0xeb, 1, 0x0049dde0);
    }
    else
    {
        process->WriteMemory(0x7e, 1, 0x0049ce02);
        process->WriteMemory(0x75, 1, 0x0049dde0);
    }
}

void PVZ_1_2_CN_ENDLESS::LockScoop(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x0041e36e);
        process->WriteMemory(6, 4, 0x755e0c, 3, 0x868, 0x150, 0x30);
    }
    else
    {
        process->WriteMemory(0x89, 1, 0x0041e36e);
    }
}

void PVZ_1_2_CN_ENDLESS::AnyMove(bool b)
{
    if (b)
    {
        process->WriteMemory(0x81, 1, 0x0042c1e7);
        process->WriteMemory(0x81, 1, 0x0042be31);
    }
    else
    {
        process->WriteMemory(0x85, 1, 0x0042c1e7);
        process->WriteMemory(0x85, 1, 0x0042be31);
    }
}

void PVZ_1_2_CN_ENDLESS::NoPurple(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc301b0, 3, 0x004293f0);
        process->WriteMemory(0xeb, 1, 0x0041a15a);
    }
    else
    {
        process->WriteMemory(0x575351, 3, 0x004293f0);
        process->WriteMemory(0x74, 1, 0x0041a15a);
    }
}

void PVZ_1_2_CN_ENDLESS::LockButter(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x00473002);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00473002);
    }
}

void PVZ_1_2_CN_ENDLESS::LockCorn(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x00473002);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x00473002);
    }
}

void PVZ_1_2_CN_ENDLESS::BoomIm()
{
    process->WriteMemory(0x80, 1, 0x0054257c);
    process->WriteMemory(0x80, 1, 0x005275dd);
}

void PVZ_1_2_CN_ENDLESS::BoomNormal()
{
    process->WriteMemory(0x8f, 1, 0x0054257c);
    process->WriteMemory(0x85, 1, 0x005275dd);
}

void PVZ_1_2_CN_ENDLESS::BoomNever()
{
    process->WriteMemory(0x81, 1, 0x0054257c);
    process->WriteMemory(0x81, 1, 0x005275dd);
}

void PVZ_1_2_CN_ENDLESS::VisiableNo()
{
    process->WriteMemory(0x70, 1, 0x0054a0ce);
    process->WriteMemory(0x70, 1, 0x0053402b);
}

void PVZ_1_2_CN_ENDLESS::VisiableNormal()
{
    process->WriteMemory(0x75, 1, 0x0054a0ce);
    process->WriteMemory(0x75, 1, 0x0053402b);
}

void PVZ_1_2_CN_ENDLESS::VisiableYes()
{
    process->WriteMemory(0xeb, 1, 0x0054a0ce);
    process->WriteMemory(0xeb, 1, 0x0053402b);
}

void PVZ_1_2_CN_ENDLESS::ZStrong()
{
    process->WriteMemory(0xc0, 1, 0x0054cdd5);
    process->WriteMemory(0x90909090, 4, 0x0054d0ba);
    process->WriteMemory(0xd233c033, 4, 0x0054ca2b);
    process->WriteMemory(0x81, 1, 0x0054e951);
    process->WriteMemory(0xeb, 1, 0x0046f808);
}

void PVZ_1_2_CN_ENDLESS::ZNormal()
{
    process->WriteMemory(0xc8, 1, 0x0054cdd5);
    process->WriteMemory(0x20246c2b, 4, 0x0054d0ba);
    process->WriteMemory(0xc28b027c, 4, 0x0054ca2b);
    process->WriteMemory(0x8d, 1, 0x0054e951);
    process->WriteMemory(0x74, 1, 0x0046f808);
}

void PVZ_1_2_CN_ENDLESS::ZWeak()
{
    process->WriteMemory(0xc9, 1, 0x0054cdd5);
    process->WriteMemory(0x9090ed2b, 4, 0x0054d0ba);
    process->WriteMemory(0xc28bd08b, 4, 0x0054ca2b);
    process->WriteMemory(0x80, 1, 0x0054e951);
    process->WriteMemory(0x74, 1, 0x0046f808);
}

void PVZ_1_2_CN_ENDLESS::NoSlow(bool b)
{
    if (b)
    {
        process->WriteMemory(0x90c033, 3, 0x0054711f);
    }
    else
    {
        process->WriteMemory(0xffc083, 3, 0x0054711f);
    }
}

void PVZ_1_2_CN_ENDLESS::NoButter(bool b)
{
    if (b)
    {
        process->WriteMemory(0x90c033, 3, 0x0054713b);
    }
    else
    {
        process->WriteMemory(0xffc083, 3, 0x0054713b);
    }
}

void PVZ_1_2_CN_ENDLESS::No3zGroup(bool b)
{
    if (b)
    {
        process->WriteMemory(0xeb, 1, 0x0041fee0);
        process->WriteMemory(0xeb, 1, 0x0041f1a2);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0041fee0);
        process->WriteMemory(0x75, 1, 0x0041f1a2);
    }
}

void PVZ_1_2_CN_ENDLESS::NoIMP(bool b)
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

void PVZ_1_2_CN_ENDLESS::NoICE(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc3, 1, 0x005464a0);
        process->WriteMemory(0xeb, 1, 0x0042b0cc);
        for (int i = 0; i < 6; ++i)
        {
            process->WriteMemory(1, 4, 0x755e0c, 2, 0x868, 0x63c + i * 4);
        }
    }
    else
    {
        process->WriteMemory(0x51, 1, 0x005464a0);
        process->WriteMemory(0x75, 1, 0x0042b0cc);
    }
}

void PVZ_1_2_CN_ENDLESS::ZEatable(bool b)
{
    if (b)
    {
        process->WriteMemory(0, 1, 0x0046f815);
    }
    else
    {
        process->WriteMemory(1, 1, 0x0046f815);
    }
}

void PVZ_1_2_CN_ENDLESS::StopZTimer(bool b)
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

void PVZ_1_2_CN_ENDLESS::StopZombies(bool b)
{
    if (b)
    {
        process->WriteMemory(0x54, 1, 0x00546823);
        process->WriteMemory(0x54, 1, 0x0054682c);
    }
    else
    {
        process->WriteMemory(0x64, 1, 0x00546823);
        process->WriteMemory(0x44, 1, 0x0054682c);
    }
}

void PVZ_1_2_CN_ENDLESS::StopBoss(bool b)
{
    if (b)
    {
        process->WriteMemory(0xc3, 1, 0x00546b40);
    }
    else
    {
        process->WriteMemory(0x55, 1, 0x00546b40);
    }
}

void PVZ_1_2_CN_ENDLESS::BalloonBoom(bool b)
{
    if (b)
    {
        process->WriteMemory(0x70, 1, 0x0054173b);
        process->WriteMemory(0x70, 1, 0x00541756);
    }
    else
    {
        process->WriteMemory(0x75, 1, 0x0054173b);
        process->WriteMemory(0x74, 1, 0x00541756);
    }
}

void PVZ_1_2_CN_ENDLESS::ZTimerSpeed(int value)
{
    process->WriteMemory(value, 4, eCode.ZTimer + 1);
}

void PVZ_1_2_CN_ENDLESS::GatherZombie(bool b, float pos)
{
    if (b)
    {
        static unsigned char code[] = { 0xc7, 0x46, 0x2c, 0, 0, 0, 0, 0xeb, 0x23 };
        (DWORD&)code[3] = (DWORD&)pos;
        process->WriteMemory(0x32, 1, 0x0054673a);
        process->WriteArrayMemory(code, sizeof(code), 0x00546810);
    }
    else
    {
        static unsigned char code[] = { 0xe8, 0xab, 0x13, 0, 0, 0x84, 0xc0, 0x75, 0x0f };
        process->WriteMemory(0x84, 1, 0x0054673a);
        process->WriteArrayMemory(code, sizeof(code), 0x00546810);
    }
}

void PVZ_1_2_CN_ENDLESS::KillAllZombies()
{
    GetZombies();
    for (auto& rz : vz)
    {
        process->WriteMemory(3, 4, rz.addr + 0x28);
    }
}

void PVZ_1_2_CN_ENDLESS::ConfuseZombies()
{
    GetZombies();
    for (auto& rz : vz)
    {
        process->WriteMemory(1, 1, rz.addr + 0xb8);
    }
}

void PVZ_1_2_CN_ENDLESS::NaturalSeed(int* zombies, int znum)
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
        DWORD SeedAddr = process->ReadMemory(4, 0x755e0c, 1, 0x868) + 0x54ec;
        DWORD ListAddr = process->ReadMemory(4, 0x755e0c, 1, 0x868) + 0x6cc;
        for (int i = 0; i < 34; ++i)
        {
            process->WriteMemory(0, 1, SeedAddr + i);
        }
        for (int i = 0; i < znum; ++i)
        {
            process->WriteMemory(1, 1, SeedAddr + zombies[i]);
        }
        asm_.clear();
        asm_.mov_exx_dword_ptr(EDI, 0x755e0c);
        asm_.mov_exx_dword_ptr_exx_add(EDI, 0x868);
        asm_.call(0x004150c0);
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
        if (process->ReadMemory(4, 0x755e0c, 1, 0x920) == 2)
        {
            asm_.clear();
            asm_.mov_exx_dword_ptr(EBX, 0x755e0c);
            asm_.mov_exx_dword_ptr_exx_add(EBX, 0x868);
            asm_.call(0x00419c50);
            asm_.mov_exx_dword_ptr(EAX, 0x755e0c);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x174);
            asm_.push_exx(EAX);
            asm_.call(0x00445d20);
            asm_.ret();
            process->WriteMemory(0x80, 1, 0x00445d33);
            SAFE_THREAD();
            process->WriteMemory(0x85, 1, 0x00445d33);
        }
    }
}

void PVZ_1_2_CN_ENDLESS::LimitSeed(int* zombies, int znum, bool thief, bool red)
{
    if (InGame())
    {
        DWORD SeedAddr = process->ReadMemory(4, 0x755e0c, 1, 0x868) + 0x54ec;
        DWORD ListAddr = process->ReadMemory(4, 0x755e0c, 1, 0x868) + 0x6cc;
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
        if (process->ReadMemory(4, 0x755e0c, 1, 0x920) == 2)
        {
            asm_.clear();
            asm_.mov_exx_dword_ptr(EBX, 0x755e0c);
            asm_.mov_exx_dword_ptr_exx_add(EBX, 0x868);
            asm_.call(0x00419c50);
            asm_.mov_exx_dword_ptr(EAX, 0x755e0c);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x174);
            asm_.push_exx(EAX);
            asm_.call(0x00445d20);
            asm_.ret();
            process->WriteMemory(0x80, 1, 0x00445d33);
            SAFE_THREAD();
            process->WriteMemory(0x85, 1, 0x00445d33);
        }
    }
}

void PVZ_1_2_CN_ENDLESS::ResetSeed()
{
    if (InGame())
    {
        DWORD addr = process->ReadMemory(4, 0x755e0c, 1, 0x868) + 0x54ec;
        for (int i = 0; i < 34; ++i)
        {
            process->WriteMemory(0, 1, addr + i);
        }
        asm_.clear();
        asm_.mov_exx_dword_ptr(ESI, 0x755e0c);
        asm_.mov_exx_dword_ptr_exx_add(ESI, 0x868);
        asm_.mov_exx_dword_ptr_exx_add(ESI, 0x178);
        asm_.call(0x00430b50);
        asm_.mov_exx_dword_ptr(EDI, 0x755e0c);
        asm_.mov_exx_dword_ptr_exx_add(EDI, 0x868);
        asm_.call(0x004150c0);
        if (process->ReadMemory(4, 0x755e0c, 1, 0x920) == 2)
        {
            asm_.mov_exx_dword_ptr(EBX, 0x755e0c);
            asm_.mov_exx_dword_ptr_exx_add(EBX, 0x868);
            asm_.call(0x00419c50);
            asm_.mov_exx_dword_ptr(EAX, 0x755e0c);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x868);
            asm_.mov_exx_dword_ptr_exx_add(EAX, 0x174);
            asm_.push_exx(EAX);
            asm_.call(0x00445d20);
            process->WriteMemory(0x80, 1, 0x00445d33);
        }
        asm_.ret();
        SAFE_THREAD();
        process->WriteMemory(0x85, 1, 0x0043d363);
    }
}

void PVZ_1_2_CN_ENDLESS::SeeLeftZombies(bool (&zombies)[33])
{
    memset(zombies, 0, sizeof(zombies));
    if (InGame())
    {
        DWORD Base = process->ReadMemory(4, 0x755e0c, 1, 0x868);
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

void PVZ_1_2_CN_ENDLESS::FlowerNPK(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x00534d7b);
        process->WriteMemory(1020, 4, 0x755e0c, 2, 0x950, 0x220);
    }
    else
    {
        process->WriteMemory(-1, 1, 0x00534d7b);
    }
}

void PVZ_1_2_CN_ENDLESS::Medicine(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x00534e73);
        process->WriteMemory(1020, 4, 0x755e0c, 2, 0x950, 0x224);
    }
    else
    {
        process->WriteMemory(-1, 1, 0x00534e73);
    }
}

void PVZ_1_2_CN_ENDLESS::Chocolate(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x00534995);
        process->WriteMemory(0x39, 1, 0x00534a17);
        process->WriteMemory(1020, 4, 0x755e0c, 2, 0x950, 0x250);
    }
    else
    {
        process->WriteMemory(-1, 1, 0x00534995);
        process->WriteMemory(-1, 1, 0x00534a17);
    }
}

void PVZ_1_2_CN_ENDLESS::TreeNPK(bool b)
{
    if (b)
    {
        process->WriteMemory(0x39, 1, 0x0043885d);
        process->WriteMemory(1020, 4, 0x755e0c, 2, 0x950, 0x258);
    }
    else
    {
        process->WriteMemory(-1, 1, 0x0043885d);
    }
}

void PVZ_1_2_CN_ENDLESS::TreeHeight(int h)
{
    if (process->ReadMemory(4, 0x755e0c, 1, 0x91c) == 50)
    {
        process->WriteMemory(h - 1, 4, 0x755e0c, 2, 0x950, 0x11c);
        asm_.clear();
        asm_.mov_exx_dword_ptr(EDI, 0x755e0c);
        asm_.mov_exx_dword_ptr_exx_add(EDI, 0x868);
        asm_.mov_exx_dword_ptr_exx_add(EDI, 0x178);
        asm_.call(0x004385d0);
        asm_.ret();
        SAFE_THREAD();
    }
    else
        process->WriteMemory(h, 4, 0x755e0c, 2, 0x950, 0x11c);
}

void PVZ_1_2_CN_ENDLESS::FlowerNum(int n)
{
    process->WriteMemory(n, 4, 0x755e0c, 2, 0x950, 0x37c);
}

void PVZ_1_2_CN_ENDLESS::ChangeFlower(int fid, int type)
{
    process->WriteMemory(type, 4, 0x755e0c, 2, 0x950, 0x388 + fid * 0x58);
}

void PVZ_1_2_CN_ENDLESS::ChangeFSize(int fid, int size)
{
    process->WriteMemory(size, 4, 0x755e0c, 2, 0x950, 0x3ac + fid * 0x58);
}

void PVZ_1_2_CN_ENDLESS::ChangeFDir(int fid, int dir)
{
    process->WriteMemory(dir, 4, 0x755e0c, 2, 0x950, 0x398 + fid * 0x58);
}

void PVZ_1_2_CN_ENDLESS::ChangeFPlace(int fid, int place)
{
    process->WriteMemory(place, 4, 0x755e0c, 2, 0x950, 0x38c + fid * 0x58);
}

int PVZ_1_2_CN_ENDLESS::GetFlowerNum()
{
    return process->ReadMemory(4, 0x755e0c, 2, 0x950, 0x37c);
}

int PVZ_1_2_CN_ENDLESS::GetFlower(int fid)
{
    return process->ReadMemory(4, 0x755e0c, 2, 0x950, 0x388 + fid * 0x58);
}

int PVZ_1_2_CN_ENDLESS::GetFSize(int fid)
{
    return process->ReadMemory(4, 0x755e0c, 2, 0x950, 0x3ac + fid * 0x58);
}

int PVZ_1_2_CN_ENDLESS::GetFDir(int fid)
{
    return process->ReadMemory(4, 0x755e0c, 2, 0x950, 0x398 + fid * 0x58);
}

int PVZ_1_2_CN_ENDLESS::GetFPlace(int fid)
{
    return process->ReadMemory(4, 0x755e0c, 2, 0x950, 0x38c + fid * 0x58);
}

void PVZ_1_2_CN_ENDLESS::Patch()
{
    process->WriteMemory(0x23, 1, 0x00550b31);
    process->WriteMemory(0x23, 1, 0x005512de);
}

#pragma warning(pop)
