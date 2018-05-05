#include "stdafx.h"
#include "PVZ_1_0.h"

#define SAFE_THREAD() \
	process->WriteMemory(-2, 1, 0x00552014), \
	Sleep(10), \
	asm_.CreateRemoteThread(process->GetHandle(), eCode.Thread), \
	process->WriteMemory(0xdb, 1, 0x00552014)

bool PVZ_1_0::InGame()
{
	return process->ReadMemory(4, 0x6a9ec0, 1, 0x768) != 0;
}

int PVZ_1_0::GetMode()
{
	return process->ReadMemory(4, 0x6a9ec0, 1, 0x7f8);
}

void PVZ_1_0::SetSun(int sun)
{
	process->WriteMemory(sun, 4, 0x6a9ec0, 2, 0x768, 0x5560);
}

void PVZ_1_0::SetMoney(int money)
{
	process->WriteMemory(money, 4, 0x6a9ec0, 2, 0x82c, 0x28);
}

void PVZ_1_0::IgnoreSun(bool b)
{
	if (b)
	{
		process->WriteMemory(0x70, 1, 0x0041ba72);
		process->WriteMemory(0x3b, 1, 0x0041ba74);
		process->WriteMemory(0x91, 1, 0x0041bac0);
		process->WriteMemory(0x80, 1, 0x00427a92);
		process->WriteMemory(0x80, 1, 0x00427dfd);
		process->WriteMemory(0xeb, 1, 0x0042487f);
	}
	else
	{
		process->WriteMemory(0x7f, 1, 0x0041ba72);
		process->WriteMemory(0x2b, 1, 0x0041ba74);
		process->WriteMemory(0x9e, 1, 0x0041bac0);
		process->WriteMemory(0x8f, 1, 0x00427a92);
		process->WriteMemory(0x8f, 1, 0x00427dfd);
		process->WriteMemory(0x74, 1, 0x0042487f);
	}
}

void PVZ_1_0::AutoPickup(bool b)
{
	if (b)
	{
		process->WriteMemory(0xeb, 1, 0x0043158f);
	}
	else
	{
		process->WriteMemory(0x75, 1, 0x0043158f);
	}
}

void PVZ_1_0::StartCar()
{
	if (InGame())
	{
		asm_.clear();
		GetCars();
		for (auto& rc:vc)
		{
			asm_.mov_exx(ESI, rc.addr);
			asm_.call(0x00458da0);
		}
		asm_.ret();
		SAFE_THREAD();
	}
}

void PVZ_1_0::ResetCar()
{
	if (InGame())
	{
		asm_.clear();
		GetCars();
		for (auto& rc : vc)
		{
			asm_.mov_exx(EAX, rc.addr);
			asm_.call(0x00458d10);
		}
		process->WriteMemory(0x60eb, 2, 0x0040bc98);
		process->WriteMemory(1, 1, 0x0040bd17);
		process->WriteMemory(0x99fc, 2, 0x00458002);
		asm_.mov_exx_dword_ptr(EAX, 0x6a9ec0);
		asm_.mov_exx_dword_ptr_exx_add(EAX, 0x768);
		asm_.push_exx(EAX);
		asm_.call(0x0040bc70);
		asm_.ret();
		SAFE_THREAD();
		process->WriteMemory(0x0975, 2, 0x0040bc98);
		process->WriteMemory(0, 1, 0x0040bd17);
		process->WriteMemory(0x9bf8, 2, 0x00458002);
	}
}

void PVZ_1_0::asm_plant(int row, int col, int type, bool copy)
{
	if (copy)
	{
		asm_.push(type);
		asm_.push(48);
		asm_.mov_exx(EAX, row);
		asm_.push(col);
		asm_.mov_exx_dword_ptr(EBP, 0x6a9ec0);
		asm_.mov_exx_dword_ptr_exx_add(EBP, 0x768);
		asm_.push_exx(EBP);
		asm_.call(0x0040d120);
		asm_.add_word(0xf08b);//mov esi,eax
		asm_.push_exx(EAX);
		asm_.call(0x00466b80);
		asm_.add_byte(0x58);//pop eax
	}
	else
	{
		asm_.push(-1);
		asm_.push(type);
		asm_.mov_exx(EAX, row);
		asm_.push(col);
		asm_.mov_exx_dword_ptr(EBP, 0x6a9ec0);
		asm_.mov_exx_dword_ptr_exx_add(EBP, 0x768);
		asm_.push_exx(EBP);
		asm_.call(0x0040d120);
	}
}

void PVZ_1_0::asm_plant_paper(int row, int col, int type)
{
	asm_plant(row, col, type, false);
	asm_.push_exx(EAX);
	asm_.mov_exx_dword_ptr(EAX, 0x6a9ec0);
	asm_.mov_exx_dword_ptr_exx_add(EAX, 0x768);
	asm_.mov_exx_dword_ptr_exx_add(EAX, 0x160);
	asm_.call(0x0042a530);
}

void PVZ_1_0::Planting(int row, int col, int type, bool copy)
{
	if (InGame())
	{
		Map_1_0 map(process);
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

void PVZ_1_0::NoPause(bool b)
{
	if (b)
	{
		process->WriteMemory(0xc3, 1, 0x0054ebef);
	}
	else
	{
		process->WriteMemory(0x57, 1, 0x0054ebef);
	}
}

void PVZ_1_0::asm_put_zombie(int row, int col, int type)
{
	asm_.push(col);
	asm_.push(type);
	asm_.mov_exx(EAX, row);
	asm_.mov_exx_dword_ptr(ECX, 0x6a9ec0);
	asm_.mov_exx_dword_ptr_exx_add(ECX, 0x768);
	asm_.mov_exx_dword_ptr_exx_add(ECX, 0x160);
	asm_.call(0x0042a0f0);
}

void PVZ_1_0::PutZombie(int row, int col, int type)
{
	if (InGame())
	{
		if (type == 25)
		{
			asm_.clear();
			asm_.mov_exx_dword_ptr(EAX, 0x6a9ec0);
			asm_.mov_exx_dword_ptr_exx_add(EAX, 0x768);
			asm_.push(0);
			asm_.push(25);
			asm_.call(0x0040ddc0);
			asm_.ret();
			SAFE_THREAD();
			return;
		}
		Map_1_0 map(process);
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

void PVZ_1_0::asm_put_ladder(int row, int col)
{
	asm_.mov_exx(EDI, row);
	asm_.push(col);
	asm_.mov_exx_dword_ptr(EAX, 0x6a9ec0);
	asm_.mov_exx_dword_ptr_exx_add(EAX, 0x768);
	asm_.call(0x00408f40);
}

void PVZ_1_0::PutLadder(int row, int col)
{
	if (InGame())
	{
		Map_1_0 map(process);
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

void PVZ_1_0::asm_put_grave(int row, int col)
{
	asm_.mov_exx_dword_ptr(EDX, 0x6a9ec0);
	asm_.mov_exx_dword_ptr_exx_add(EDX, 0x768);
	asm_.mov_exx_dword_ptr_exx_add(EDX, 0x160);
	asm_.push_exx(EDX);
	asm_.mov_exx(EDI, row);
	asm_.mov_exx(EBX, col);
	asm_.call(0x00426620);
}

void PVZ_1_0::PutGrave(int row, int col)
{
	if (InGame())
	{
		Map_1_0 map(process);
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

void PVZ_1_0::AutoLadder()
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
		Map_1_0 map(process);
		GetPlants();
		for (auto& rp : vp)
		{
			if (rp.Type == 30)
			{
				if (rp.Col != 0 && !HasLadder[rp.Row][rp.Col]
					&& map.GetBlock(rp.Row, rp.Col) != 3)
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
// 	asm_.mov_exx_dword_ptr(ECX, 0x6a9ec0);
// 	asm_.mov_exx_dword_ptr_exx_add(ECX, 0x768);
// 	asm_.push_exx(ECX);
// 	asm_.call(0x0040b9c0);
// 	asm_.add_esp(8);
// }
// 
// void PVZ_1_0::PutRake(int row, int col)
// {
// 	if (InGame())
// 	{
// 		Map_1_0 map(process);
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
// 		process->WriteMemory(0x0000a681, 4, 0x0040b9e3);
// 		process->WriteMemory(0, 1, 0x0040bb2b);
// 		process->WriteMemory(0x900c4d8b, 4, 0x0040bb3b);
// 		process->WriteMemory(0x9010458b, 4, 0x0040bb41);
// 		SAFE_THREAD();
// 		process->WriteMemory(0x00027984, 4, 0x0040b9e3);
// 		process->WriteMemory(-1, 1, 0x0040bb2b);
// 		process->WriteMemory(0x10244c8b, 4, 0x0040bb3b);
// 		process->WriteMemory(0x1424448b, 4, 0x0040bb41);
// 	}
// }

void PVZ_1_0::SetBlock(int row, int col, int type)
{
	Map_1_0 map(process);
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

void PVZ_1_0::SetRow(int row, int type)
{
	Map_1_0 map(process);
	map.SetRow(row, type);
}

void PVZ_1_0::NoDelete(bool b)
{
	if (b)
	{
		process->WriteMemory(0x70, 1, 0x0054b267);
	}
	else
	{
		process->WriteMemory(0x74, 1, 0x0054b267);
	}
}

void PVZ_1_0::NoSave(bool b)
{
	if (b)
	{
		process->WriteMemory(0x2e, 1, 0x00482149);
	}
	else
	{
		process->WriteMemory(0x13, 1, 0x00482149);
	}
}

void PVZ_1_0::Anywhere(bool b)
{
	if (b)
	{
		process->WriteMemory(0x81, 1, 0x0040fe30);
		process->WriteMemory(0xeb, 1, 0x00438e40);
		process->WriteMemory(0x8d, 1, 0x0042a2d9);
	}
	else
	{
		process->WriteMemory(0x84, 1, 0x0040fe30);
		process->WriteMemory(0x74, 1, 0x00438e40);
		process->WriteMemory(0x84, 1, 0x0042a2d9);
	}
}

void PVZ_1_0::FastBelt(bool b)
{
	if (b)
	{
		process->WriteMemory(0x80, 1, 0x00422d20);
		process->WriteMemory(0x33, 1, 0x00489ca1);
	}
	else
	{
		process->WriteMemory(0x8f, 1, 0x00422d20);
		process->WriteMemory(0x85, 1, 0x00489ca1);
	}
}

void PVZ_1_0::ShowHide(bool b)
{
	if (b)
	{
		process->WriteMemory(0x38, 1, 0x0042df5d);
	}
	else
	{
		process->WriteMemory(0x88, 1, 0x0042df5d);
	}
}

void PVZ_1_0::FullFog(bool b)
{
	if (b)
	{
		process->WriteMemory(0xc3c033, 3, 0x0041c1c0);
		process->WriteMemory(0x3feb, 2, 0x0041a476);
		process->WriteMemory(0, 1, 0x0041a4ba);
	}
	else
	{
		DWORD FogAddr = process->ReadMemory(4, 0x6a9ec0, 1, 0x768) + 0x4c8;
		for (int i = 0; i < 63; ++i)
		{
			process->WriteMemory(0, 4, FogAddr + i * 4);
		}
		process->WriteMemory(0x8c818b, 3, 0x0041c1c0);
		process->WriteMemory(0x0575, 2, 0x0041a476);
		process->WriteMemory(4, 1, 0x0041a4ba);
	}
}

void PVZ_1_0::SeeFog(bool b)
{
	if (b)
	{
		process->WriteMemory(0xd231, 2, 0x0041a68d);
	}
	else
	{
		process->WriteMemory(0xf23b, 2, 0x0041a68d);
	}
}

void PVZ_1_0::SeeVase(bool b)
{
	if (b)
	{
		process->WriteMemory(0x0033b866, 4, 0x0044e5cc);
	}
	else
	{
		process->WriteMemory(0x067ec085, 4, 0x0044e5cc);
	}
}

void PVZ_1_0::JmpLevel(int level)
{
	process->WriteMemory(level, 4, 0x6a9ec0, 3, 0x768, 0x160, 0x6c);
}

void PVZ_1_0::MixMode(int mode, int level)
{
	if (mode == 0)
	{
		process->WriteMemory(level, 4, 0x6a9ec0, 2, 0x82c, 0x24);
		process->WriteMemory(level, 4, 0x6a9ec0, 2, 0x768, 0x5550);
	}
	process->WriteMemory(mode, 4, 0x6a9ec0, 1, 0x7f8);
}

void PVZ_1_0::Unlock(bool b)
{
	if (b)
	{
		process->WriteMemory(0x70, 1, 0x00454109);
		process->WriteMemory(0x70, 1, 0x0044a514);
		process->WriteMemory(0xc3c031, 3, 0x0042e440);
		process->WriteMemory(0xeb, 1, 0x00449e9d);
		process->WriteMemory(0xc3c030, 3, 0x0048aad0);
		process->WriteMemory(0xeb, 1, 0x0048a54c);
		process->WriteMemory(0xeb, 1, 0x0048d32b);
		process->WriteMemory(0xeb, 1, 0x0048c491);
		process->WriteMemory(0xeb, 1, 0x00449e7a);
		process->WriteMemory(0xeb, 1, 0x00453ad1);
		process->WriteMemory(0x5beb01b0, 4, 0x00403a10);
		process->WriteMemory(0, 4, 0x0069dca0);
		process->WriteMemory(0xc301b0, 3, 0x00403b30);
	}
	else
	{
		process->WriteMemory(0x7e, 1, 0x00454109);
		process->WriteMemory(0x7e, 1, 0x0044a514);
		process->WriteMemory(0x555351, 3, 0x0042e440);
		process->WriteMemory(0x7f, 1, 0x00449e9d);
		process->WriteMemory(0xd98b53, 3, 0x0048aad0);
		process->WriteMemory(0x7f, 1, 0x0048a54c);
		process->WriteMemory(0x7f, 1, 0x0048d32b);
		process->WriteMemory(0x7f, 1, 0x0048c491);
		process->WriteMemory(0x7f, 1, 0x00449e7a);
		process->WriteMemory(0x7f, 1, 0x00453ad1);
		process->WriteMemory(0x6c8b5551, 4, 0x00403a10);
		process->WriteMemory(40, 4, 0x0069dca0);
		process->WriteMemory(0x6c808b, 3, 0x00403b30);
	}
}

void PVZ_1_0::Win()
{
	int mode = GetMode();
	if (!InGame() ||
		mode >= 1 && mode <= 15 && process->ReadMemory(4, 0x6a9ec0, 1, 0x7fc != 3))
	{
		return;
	}
	asm_.clear();
	asm_.mov_exx_dword_ptr(ECX, 0x6a9ec0);
	asm_.mov_exx_dword_ptr_exx_add(ECX, 0x768);
	asm_.call(0x0040c3e0);
	asm_.ret();
	SAFE_THREAD();
}

void PVZ_1_0::Lock(bool b, int mode)
{
	if (b)
	{
		static const int val[] = { 24, -624, 73, 100, 122, 190, 249, 360 };
		process->WriteMemory(0x81, 1, 0x0042b047);
		process->WriteMemory(val[mode], 4, 0x0042b048);
	}
	else
	{
		process->WriteMemory(0x85, 1, 0x0042b047);
		process->WriteMemory(144, 4, 0x0042b048);
	}
}

void PVZ_1_0::Little(bool b)
{
	if (b)
	{
		process->WriteMemory(0xeb, 1, 0x00523ed5);
	}
	else
	{
		process->WriteMemory(0x74, 1, 0x00523ed5);
	}

}

void PVZ_1_0::Rain(bool b)
{
	if (b)
	{
		process->WriteMemory(0x70, 1, 0x00426b4e);
		process->WriteMemory(0x70, 1, 0x00416f07);
		process->WriteMemory(0xeb, 1, 0x00424715);
	}
	else
	{
		process->WriteMemory(0x75, 1, 0x00426b4e);
		process->WriteMemory(0x75, 1, 0x00416f07);
		process->WriteMemory(0x74, 1, 0x00424715);
	}
}

void PVZ_1_0::Storm(bool b)
{
	if (b)
	{
		process->WriteMemory(0xeb, 1, 0x00416eef);
		process->WriteMemory(0xeb, 1, 0x00426b6b);
		process->WriteMemory(0xeb, 1, 0x00426b35);
		process->WriteMemory(0xeb, 1, 0x004246c9);
		process->WriteMemory(0xeb, 1, 0x00424729);
	}
	else
	{
		process->WriteMemory(0x74, 1, 0x00416eef);
		process->WriteMemory(0x74, 1, 0x00426b6b);
		process->WriteMemory(0x74, 1, 0x00426b35);
		process->WriteMemory(0x74, 1, 0x004246c9);
		process->WriteMemory(0x74, 1, 0x00424729);
	}
}

void PVZ_1_0::Force(bool b)
{
	if (b)
	{
		process->WriteMemory(0x70, 1, 0x0046d546);
		process->WriteMemory(0x70, 1, 0x0046dc28);
		process->WriteMemory(0x70, 1, 0x00525bed);
	}
	else
	{
		process->WriteMemory(0x75, 1, 0x0046d546);
		process->WriteMemory(0x75, 1, 0x0046dc28);
		process->WriteMemory(0x75, 1, 0x00525bed);
	}
}

void PVZ_1_0::LikeCol(bool b)
{
	if (b)
	{
		process->WriteMemory(0x80, 1, 0x0043903d);
		process->WriteMemory(0x80, 1, 0x00410ae7);
	}
	else
	{
		process->WriteMemory(0x85, 1, 0x0043903d);
		process->WriteMemory(0x85, 1, 0x00410ae7);
	}
}

void PVZ_1_0::RunFast(bool b)
{
	if (b)
	{
		process->WriteMemory(0x70, 1, 0x004248aa);
	}
	else
	{
		process->WriteMemory(0x75, 1, 0x004248aa);
	}
}

void PVZ_1_0::SetCardNum(int num)
{
	if (InGame())
	{
		process->WriteMemory(0xc3, 1, 0x0041bee0);
		asm_.clear();
		asm_.mov_exx(EAX, num);
		asm_.mov_exx_dword_ptr(ESI, 0x6a9ec0);
		asm_.mov_exx_dword_ptr_exx_add(ESI, 0x768);
		asm_.mov_exx_dword_ptr_exx_add(ESI, 0x144);
		asm_.call(0x00489cd0);
		asm_.ret();
		SAFE_THREAD();
		process->WriteMemory(0x56, 1, 0x0041bee0);
	}
}

void PVZ_1_0::SetCard(int cid, int type, bool copy)
{
	if (copy)
	{
		process->WriteMemory(48, 4, 0x6a9ec0, 3, 0x768, 0x144, 0x5c + cid * 0x50);
		process->WriteMemory(type, 4, 0x6a9ec0, 3, 0x768, 0x144, 0x60 + cid * 0x50);
	}
	else
	{
		process->WriteMemory(type, 4, 0x6a9ec0, 3, 0x768, 0x144, 0x5c + cid * 0x50);
	}
}

void PVZ_1_0::FSBoom(bool b)
{
	if (b)
	{
		process->WriteMemory(0x20eb, 2, 0x0041d8ff);
		process->WriteMemory(0x70, 1, 0x004664f2);
	}
	else
	{
		process->WriteMemory(0x3e7f, 2, 0x0041d8ff);
		process->WriteMemory(0x75, 1, 0x004664f2);
	}
}

void PVZ_1_0::FSSquash(bool b)
{
	if (b)
	{
		process->WriteMemory(0x45eb, 2, 0x00460929);
		process->WriteMemory(0x80, 1, 0x00460838);
		process->WriteMemory(0x5feb, 2, 0x00460747);
	}
	else
	{
		process->WriteMemory(0x707f, 2, 0x00460929);
		process->WriteMemory(0x85, 1, 0x00460838);
		process->WriteMemory(0x7275, 2, 0x00460747);
	}
}

void PVZ_1_0::FSMagnet(bool b)
{
	if (b)
	{
		process->WriteMemory(0x80, 1, 0x004620a3);
		process->WriteMemory(0x70, 1, 0x00462124);
	}
	else
	{
		process->WriteMemory(0x87, 1, 0x004620a3);
		process->WriteMemory(0x74, 1, 0x00462124);
	}
}

void PVZ_1_0::FSSputter(bool b)
{
	if (b)
	{
		process->WriteMemory(0x70, 1, 0x0046d455);
	}
	else
	{
		process->WriteMemory(0x74, 1, 0x0046d455);
	}
}

void PVZ_1_0::FSMouth(bool b)
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

void PVZ_1_0::FSPrick(bool b)
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

void PVZ_1_0::FSSpout(bool b)
{
	if (b)
	{
		process->WriteMemory(0x74, 1, eCode.Row + 0x1d - 0x0d);
		process->WriteMemory(0x74, 1, eCode.Col + 0x42 - 0x32);
		process->WriteMemory(0x74, 1, eCode.Col + 0x47 - 0x32);
		process->WriteMemory(0x74, 1, eCode.RHurt + 0x64 - 0x5c);
		process->WriteMemory(0x74, 1, eCode.CHurt + 0x81 - 0x79);
		process->WriteMemory(0x74, 1, eCode.CHurt + 0x86 - 0x79);
		process->WriteMemory(0x39eb, 2, 0x0046768d);
		process->WriteMemory(0x16eb, 2, 0x0045ed5c);
	}
	else
	{
		process->WriteMemory(0x70, 1, eCode.Row + 0x1d - 0x0d);
		process->WriteMemory(0x70, 1, eCode.Col + 0x42 - 0x32);
		process->WriteMemory(0x70, 1, eCode.Col + 0x47 - 0x32);
		process->WriteMemory(0x70, 1, eCode.RHurt + 0x64 - 0x5c);
		process->WriteMemory(0x70, 1, eCode.CHurt + 0x81 - 0x79);
		process->WriteMemory(0x70, 1, eCode.CHurt + 0x86 - 0x79);
		process->WriteMemory(0x8c0f, 2, 0x0046768d);
		process->WriteMemory(0x8c0f, 2, 0x0045ed5c);
	}
}

void PVZ_1_0::FSCat(bool b)
{
	if (b)
	{
		process->WriteMemory(0x70, 1, 0x004677ef);
		process->WriteMemory(0x75, 1, eCode.CatR + 0xc3 - 0x9b);
		process->WriteMemory(0x74, 1, eCode.CatC + 0xd1 - 0xc8);
		process->WriteMemory(0x1beb, 2, 0x00464a52);
		static unsigned char code1[] = { 0x33, 0xc0, 0xb0, 0x03, 0xeb, 0x0f };
		process->WriteArrayMemory(code1, sizeof(code1), 0x0045eb10);
		process->WriteMemory(0x3f, 1, 0x00464a61);
		static unsigned char _3_code[] = { 0xE9, 0x91, 0x00, 0x00, 0x00, 0x83, 0xF8, 0x12, 0x75, 0xE9, 0x8B, 0x87, 0x90, 0x00, 0x00, 0x00, 0x83, 0xF8, 0x09, 0x74, 0xBE, 0x83, 0xF8, 0x05, 0x74, 0xB9, 0x83, 0xF8, 0x01, 0x74, 0xB4, 0xE9, 0x74, 0xFE, 0xFF, 0xFF };
		process->WriteArrayMemory(_3_code, sizeof(_3_code), 0x00464a9c);
		process->WriteMemory(0xeb, 1, 0x004672f4);
		process->WriteMemory(0x70, 1, 0x00467369);
		process->WriteMemory(0x70, 1, eCode.Split);
		process->WriteMemory(0xe9, 1, 0x00464bbc);
		process->WriteMemory(0xfffffeae, 4, 0x00464bbd);
		process->WriteMemory(0x70, 1, eCode.Throw);
		process->WriteMemory(0x02ebd175, 4, 0x004673b3);
		process->WriteMemory(0x80, 1, 0x004672d1);
		process->WriteMemory(0x80, 1, 0x004672da);
		process->WriteMemory(0x80, 1, 0x004672e3);
		process->WriteMemory(0x80, 1, 0x004672ec);
		process->WriteMemory(0x80, 1, 0x00467353);
		process->WriteMemory(0x80, 1, 0x0046735c);
		process->WriteMemory(0xeb, 1, 0x0046736e);
	}
	else
	{
		process->WriteMemory(0x75, 1, 0x004677ef);
		process->WriteMemory(0x70, 1, eCode.CatR + 0xc3 - 0x9b);
		process->WriteMemory(0xeb, 1, eCode.CatC + 0xd1 - 0xc8);
		process->WriteMemory(0x4f8b, 2, 0x00464a52);
		static unsigned char code1[] = { 0x8B, 0x40, 0x24, 0x83, 0xF8, 0x1A };
		process->WriteArrayMemory(code1, sizeof(code1), 0x0045eb10);
		process->WriteMemory(0x2d, 1, 0x00464a61);
		static unsigned char _3_code[] = { 0x83, 0xF8, 0x12, 0x0F, 0x85, 0x8D, 0x00, 0x00, 0x00, 0x8B, 0x5F, 0x1C, 0x8B, 0x37, 0x8B, 0x8F, 0x9C, 0x00, 0x00, 0x00, 0x8D, 0x53, 0xFF, 0x8B, 0xC6, 0x89, 0x54, 0x24, 0x14, 0xE8, 0xF2, 0xF1, 0xFE, 0xFF, 0x8B, 0x8F };
		process->WriteArrayMemory(_3_code, sizeof(_3_code), 0x00464a9c);
		process->WriteMemory(0x75, 1, 0x004672f4);
		process->WriteMemory(0x74, 1, 0x00467369);
		process->WriteMemory(0xeb, 1, eCode.Split);
		process->WriteMemory(0x8b, 1, 0x00464bbc);
		process->WriteMemory(0x006a1c4f, 4, 0x00464bbd);
		process->WriteMemory(0xeb, 1, eCode.Throw);
		process->WriteMemory(0x0193850f, 4, 0x004673b3);
		process->WriteMemory(0x84, 1, 0x004672d1);
		process->WriteMemory(0x84, 1, 0x004672da);
		process->WriteMemory(0x84, 1, 0x004672e3);
		process->WriteMemory(0x84, 1, 0x004672ec);
		process->WriteMemory(0x84, 1, 0x00467353);
		process->WriteMemory(0x84, 1, 0x0046735c);
		process->WriteMemory(0x75, 1, 0x0046736e);
	}
}

void PVZ_1_0::CatTouch(bool b)
{
	if (b)
	{
		process->WriteMemory(0x81, 1, 0x0046cecc);
	}
	else
	{
		process->WriteMemory(0x85, 1, 0x0046cecc);
	}
}

void static __declspec(naked) DoorCode()
{
	_asm{
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
	_asm{
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
			push 0x00467884
		label:
		ret
			int 3
			int 3
		fend :
	}
}

void static __declspec(naked) ColCode()
{
	_asm{
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
			push 0x00467881
		label :
		ret
			int 3
			int 3
		fend :
	}
}

void static __declspec(naked) RHurtCode()
{
	_asm{
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
			push 0x0045ee76
		label :
		ret
			int 3
			int 3
		fend :
	}
}

void static __declspec(naked) CHurtCode()
{
	_asm{
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
			push 0x0045ee76
		label:
		ret
			int 3
			int 3
		fend :
	}
}

void static __declspec(naked) CatRCode()
{
	_asm{
		jmp fend
			cmp eax, 43
			jne short _1
		_2 :
		pop eax
			_jmp(0x004676c8)
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
	_asm{
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
			push 0x004677e0
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
	_asm{
		jmp fend
			jmp short _1
			cmp dword ptr[edi + 0x24], 47
			je _2
			test eax, eax
			jne _2
			_jmp(0x00464934)
		_2:
		mov ecx, [edi + 0x1c]
			push esi
			_jmp(0x00464a88)
		_1 :
		   mov ecx, [edi + 0x1c]
		   push esi
		   push ecx
		   _jmp(0x00464c2d)
		   int 3
		   int 3
	   fend:
	}
}

void static __declspec(naked) CatDirCode()
{
	_asm{
		jmp fend
			cmp eax, 52
			jne short _1
		_3 :
		fld dword ptr ds : [0x006799e8]
			ret
		_1 :
		cmp eax, 28
			jne short _2
			cmp edx, 1
			je short _3
		_2 :
		fld dword ptr ds : [0x006793e0]
			ret
			int 3
			int 3
		fend :
	}
}

void static __declspec(naked) SplitCode()
{
	_asm{
		jmp fend
			jmp short _1
			mov ecx, [edi + 0x1c]
			push ecx
			push edi
			xor ecx, ecx
			_call(0x004675c0)
			test eax, eax
			jne _2
			_jmp(0x00464934)
		_2:
		push eax
			mov ecx, [edi + 0x1c]
			push 0
			push ecx
			push eax
			push edi
			_call(0x00466e00)
			pop eax
			mov edx, [edi + 0x1c]
			push 1
			push edx
			push eax
			push edi
			_call(0x00466e00)
			_jmp(0x00464934)
		_1:
		mov esi, 3
			_jmp(0x00464b5a)
			int 3
			int 3
		fend :
	}
}

void static __declspec(naked) ZTimerCode()
{
	_asm{
		jmp fend
			mov ecx, 0xffffffff
			cmp[edi + 0x559c], ecx
			ja short _1
			dec dword ptr ds : [edi + 0x559c]
			ret
		_1 :
		mov[edi + 0x559c], ecx
			ret
			int 3
			int 3
		fend :
	}
}

void static __declspec(naked) GetHeadRowsCode()
{
	_asm{
		jmp fend
			mov eax, ds:[0x6a9ec0]
			mov eax, [eax + 0x768]
			mov eax, [eax + 0x554c]
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
			  ret
			  int 3
			  int 3
		  fend :
	}
}

void static __declspec(naked) GetRVRowsCode()
{
	_asm{
		jmp fend
			mov eax, ds:[0x6a9ec0]
			mov eax, [eax + 0x768]
			mov eax, [eax + 0x554c]
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
			  ret
			  int 3
			  int 3
		  fend :
	}
}

void static __declspec(naked) DoorRowsCode()
{
	_asm{
		jmp fend
			mov eax, ds:[0x6a9ec0]
			mov eax, [eax + 0x768]
			mov eax, [eax + 0x554c]
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
			  inc edi
			  cmp edi, eax
			  ret
			  int 3
			  int 3
		  fend :
	}
}

void PVZ_1_0::InitExtraCode()
{
#define WRITE_CODE(f) \
	process->WriteArrayMemory(GET_ADDR(f) + HEAD_OF(f), SIZE_OF(f), temp), \
	process->WriteArrayMemory(CallCode, sizeof(CallCode), Caller)

	bool b = (process->ReadMemory(4, 0x00426fb7) != 0xcccccccc);
	DWORD temp;
	if (b)
	{
		temp = process->ReadMemory(4, 0x00426fb7);
	}
	else
	{
		temp = (DWORD)VirtualAllocEx(process->GetHandle(), NULL, 4096 * 2, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (temp)
			process->WriteMemory(temp, 4, 0x00426fb7);
	}
	DWORD Caller;
	/*传送门*/
	if (!b)
	{
		Caller = 0x0042706c;
		static unsigned char CallCode[] = { 0xb9, 4, 0, 0, 0, 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[6] = temp - Caller - 10;
		WRITE_CODE(DoorCode);
	}
	temp += SIZE_OF(DoorCode);
	/*行判定*/
	eCode.Row = temp;
	if (!b)
	{
		Caller = 0x004676c2;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(RowCode);
	}
	temp += SIZE_OF(RowCode);
	/*列判定*/
	eCode.Col = temp;
	if (!b)
	{
		Caller = 0x004677da;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(ColCode);
	}
	temp += SIZE_OF(ColCode);
	/*行伤害判定*/
	eCode.RHurt = temp;
	if (!b)
	{
		Caller = 0x0045ed6e;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(RHurtCode);
	}
	temp += SIZE_OF(RHurtCode);
	/*列伤害判定*/
	eCode.CHurt = temp;
	if (!b)
	{
		Caller = 0x0045ede2;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CHurtCode);
	}
	temp += SIZE_OF(CHurtCode);
	/*香蒲免行判定*/
	eCode.CatR = temp;
	if (!b)
	{
		Caller = 0x00467680;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CatRCode);
	}
	temp += SIZE_OF(CatRCode);
	/*香蒲免列判定*/
	eCode.CatC = temp;
	if (!b)
	{
		Caller = 0x004677c0;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CatCCode);
	}
	temp += SIZE_OF(CatCCode);
	/*投手特殊处理*/
	eCode.Throw = temp;
	if (!b)
	{
		Caller = 0x00464c28;
		static unsigned char CallCode[] = { 0xe9, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(ThrowCode);
	}
	temp += SIZE_OF(ThrowCode);
	/*猫式全屏子弹方向处理*/
	eCode.CatDir = temp;
	if (!b)
	{
		Caller = 0x00467386;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CatDirCode);
	}
	temp += SIZE_OF(CatDirCode);
	/*裂荚处理*/
	eCode.Split = temp;
	if (!b)
	{
		Caller = 0x00464b55;
		static unsigned char CallCode[] = { 0xe9, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(SplitCode);
	}
	temp += SIZE_OF(SplitCode);
	/*刷怪倒计时*/
	eCode.ZTimer = temp;
	if (!b)
	{
		Caller = 0x00413e45;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(ZTimerCode);
	}
	temp += SIZE_OF(ZTimerCode);
	/*僵博吐球行*/
	if (!b)
	{
		Caller = 0x00535484;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(GetHeadRowsCode);
	}
	temp += SIZE_OF(GetHeadRowsCode);
	if (!b)
	{
		Caller = 0x00534aeb;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(GetRVRowsCode);
	}
	temp += SIZE_OF(GetRVRowsCode);
	/*传送门行*/
	if (!b)
	{
		Caller = 0x00427535;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(DoorRowsCode);
	}
	temp += SIZE_OF(DoorRowsCode);


	eCode.Thread = temp;
	return;
}

void PVZ_1_0::StartDoor(bool b)
{
	if (b)
	{
		if (InGame() && process->ReadMemory(4, 0x6a9ec0, 1, 0x7fc) > 2)
		{
			bool HasDoor = false;
			GetItems();
			for (auto& ri:vi)
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
		process->WriteMemory(0x70, 1, 0x00467665);
		process->WriteMemory(0x70, 1, 0x004248ce);
		process->WriteMemory(0x70, 1, 0x0041ffb4);
	}
	else
	{
		process->WriteMemory(0x75, 1, 0x00467665);
		process->WriteMemory(0x75, 1, 0x004248ce);
		process->WriteMemory(0x75, 1, 0x0041ffb4);
	}
}

void PVZ_1_0::StableDoor(bool b)
{
	if (b)
	{
		process->WriteMemory(0, 1, 0x004276dd);
	}
	else
	{
		process->WriteMemory(-1, 1, 0x004276dd);
	}
}

void PVZ_1_0::SetDoor(int b1r, int b1c, int b2r, int b2c, int w1r, int w1c, int w2r, int w2c)
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
				asm_.call(0x0044d000);
			}
		}
		process->WriteMemory(b1c, 1, 0x00426fe2);
		process->WriteMemory(b1r, 1, 0x00426fe9);
		process->WriteMemory(b2c, 1, 0x0042700d);
		process->WriteMemory(b2r, 1, 0x00427014);
		process->WriteMemory(w1c, 1, 0x0042703d);
		process->WriteMemory(w1r, 1, 0x00427044);
		process->WriteMemory(w2c, 1, 0x00427068);
		process->WriteMemory(w2r, 1, 0x0042706d);
		asm_.mov_exx_dword_ptr(EDI, 0x6a9ec0);
		asm_.mov_exx_dword_ptr_exx_add(EDI, 0x768);
		asm_.mov_exx_dword_ptr_exx_add(EDI, 0x160);
		asm_.call(0x00426fc0);
		asm_.ret();
		SAFE_THREAD();
		process->WriteMemory(2, 1, 0x00426fe2);
		process->WriteMemory(0, 1, 0x00426fe9);
		process->WriteMemory(9, 1, 0x0042700d);
		process->WriteMemory(1, 1, 0x00427014);
		process->WriteMemory(9, 1, 0x0042703d);
		process->WriteMemory(3, 1, 0x00427044);
		process->WriteMemory(2, 1, 0x00427068);
		process->WriteMemory(4, 1, 0x0042706d);
	}
}

void PVZ_1_0::CornNoWait(bool b)
{
	if (b)
	{
		process->WriteMemory(0x80, 1, 0x0046103b);
	}
	else
	{
		process->WriteMemory(0x85, 1, 0x0046103b);
	}
}

void PVZ_1_0::MagnetNoWait(bool b)
{
	if (b)
	{
		process->WriteMemory(0x80, 1, 0x00461e37);
	}
	else
	{
		process->WriteMemory(0x85, 1, 0x00461e37);
	}
}

void PVZ_1_0::PotatoNoWait(bool b)
{
	if (b)
	{
		process->WriteMemory(0x80, 1, 0x0045fe54);
	}
	else
	{
		process->WriteMemory(0x85, 1, 0x0045fe54);
	}
}

void PVZ_1_0::MouthNoWait(bool b)
{
	if (b)
	{
		process->WriteMemory(0x70, 1, 0x00461565);
	}
	else
	{
		process->WriteMemory(0x75, 1, 0x00461565);
	}
}

void PVZ_1_0::PStrong()
{
	process->WriteMemory(0x004046, 3, 0x0052fcf1);
	process->WriteMemory(0x909090, 3, 0x0046cfeb);
	process->WriteMemory(0x909090, 3, 0x0046d7a6);
	process->WriteMemory(0xeb, 1, 0x0052e93b);
	process->WriteMemory(0xeb, 1, 0x0041cc2f);
	process->WriteMemory(0xeb, 1, 0x005276ea);
	process->WriteMemory(0x70, 1, 0x0045ee0a);
	process->WriteMemory(0, 1, 0x0045ec66);
}

void PVZ_1_0::PNormal()
{
	process->WriteMemory(0xfc4046, 3, 0x0052fcf1);
	process->WriteMemory(0x405029, 3, 0x0046cfeb);
	process->WriteMemory(0x404e29, 3, 0x0046d7a6);
	process->WriteMemory(0x74, 1, 0x0052e93b);
	process->WriteMemory(0x74, 1, 0x0041cc2f);
	process->WriteMemory(0x75, 1, 0x005276ea);
	process->WriteMemory(0x75, 1, 0x0045ee0a);
	process->WriteMemory(-32, 1, 0x0045ec66);
}

void PVZ_1_0::PWeak()
{
	process->WriteMemory(0x004066, 3, 0x0052fcf1);
	process->WriteMemory(0x404029, 3, 0x0046cfeb);
	process->WriteMemory(0x407629, 3, 0x0046d7a6);
	process->WriteMemory(0x74, 1, 0x0052e93b);
	process->WriteMemory(0x74, 1, 0x0041cc2f);
	process->WriteMemory(0x75, 1, 0x005276ea);
	process->WriteMemory(0x70, 1, 0x0045ee0a);
	process->WriteMemory(-32, 1, 0x0045ec66);
}

void PVZ_1_0::NoMark(bool b)
{
	if (b)
	{
		process->WriteMemory(0x70, 1, 0x0041d79e);
	}
	else
	{
		process->WriteMemory(0x75, 1, 0x0041d79e);
	}
}

void PVZ_1_0::NoSleep(bool b)
{
	if (b)
	{
		process->WriteMemory(0xeb, 1, 0x0045de8e);
		if (InGame())
		{
			asm_.clear();
			GetPlants();
			for (auto& rp:vp)
			{
				if (rp.Sleeping)
				{
					asm_.mov_exx(EAX, rp.addr);
					asm_.push(0);
					asm_.call(0x0045e860);
				}
			}
			asm_.ret();
			SAFE_THREAD();
		}
	}
	else
	{
		process->WriteMemory(0x74, 1, 0x0045de8e);
	}
}

void PVZ_1_0::StrongWind(bool b)
{
	if (b)
	{
		process->WriteMemory(0xeb, 1, 0x00466601);
	}
	else
	{
		process->WriteMemory(0x74, 1, 0x00466601);
	}
}

void PVZ_1_0::StrongBean(bool b)
{
	if (b)
	{
		process->WriteMemory(0xc3, 1, 0x0046e429);
	}
	else
	{
		process->WriteMemory(0xe9, 1, 0x0046e429);
	}
}

void PVZ_1_0::Umbrella(bool b)
{
	if (b)
	{
		process->WriteMemory(0x70, 1, 0x0041d3f8);
	}
	else
	{
		process->WriteMemory(0x75, 1, 0x0041d3f8);
	}
}

void PVZ_1_0::NoCD(bool b)
{
	if (b)
	{
		process->WriteMemory(0x70, 1, 0x00487296);
		process->WriteMemory(0xeb, 1, 0x00488250);
	}
	else
	{
		process->WriteMemory(0x7e, 1, 0x00487296);
		process->WriteMemory(0x75, 1, 0x00488250);
	}
}

void PVZ_1_0::LockScoop(bool b)
{
	if (b)
	{
		process->WriteMemory(0x39, 1, 0x004123a3);
		process->WriteMemory(6, 4, 0x6a9ec0, 3, 0x768, 0x138, 0x30);
	}
	else
	{
		process->WriteMemory(0x89, 1, 0x004123a3);
	}
}

void PVZ_1_0::AnyMove(bool b)
{
	if (b)
	{
		process->WriteMemory(0x81, 1, 0x004208fb);
		process->WriteMemory(0x81, 1, 0x00420533);
	}
	else
	{
		process->WriteMemory(0x85, 1, 0x004208fb);
		process->WriteMemory(0x85, 1, 0x00420533);
	}
}

void PVZ_1_0::NoPurple(bool b)
{
	if (b)
	{
		process->WriteMemory(0xc301b0, 3, 0x0041d7d0);
		process->WriteMemory(0xeb, 1, 0x0040e477);
	}
	else
	{
		process->WriteMemory(0xf88351, 3, 0x0041d7d0);
		process->WriteMemory(0x74, 1, 0x0040e477);
	}
}

void PVZ_1_0::LockButter(bool b)
{
	if (b)
	{
		process->WriteMemory(0x70, 1, 0x00464bd6);
	}
	else
	{
		process->WriteMemory(0x75, 1, 0x00464bd6);
	}
}

void PVZ_1_0::LockCorn(bool b)
{
	if (b)
	{
		process->WriteMemory(0xeb, 1, 0x00464bd6);
	}
	else
	{
		process->WriteMemory(0x75, 1, 0x00464bd6);
	}
}

void PVZ_1_0::BoomIm()
{
	process->WriteMemory(0x80, 1, 0x00526afc);
	process->WriteMemory(0x80, 1, 0x005275dd);
}

void PVZ_1_0::BoomNormal()
{
	process->WriteMemory(0x8f, 1, 0x00526afc);
	process->WriteMemory(0x85, 1, 0x005275dd);
}

void PVZ_1_0::BoomNever()
{
	process->WriteMemory(0x81, 1, 0x00526afc);
	process->WriteMemory(0x81, 1, 0x005275dd);
}

void PVZ_1_0::VisiableNo()
{
	process->WriteMemory(0x70, 1, 0x0052e357);
	process->WriteMemory(0x70, 1, 0x0053402b);
}

void PVZ_1_0::VisiableNormal()
{
	process->WriteMemory(0x75, 1, 0x0052e357);
	process->WriteMemory(0x75, 1, 0x0053402b);
}

void PVZ_1_0::VisiableYes()
{
	process->WriteMemory(0xeb, 1, 0x0052e357);
	process->WriteMemory(0xeb, 1, 0x0053402b);
}

void PVZ_1_0::ZStrong()
{
	process->WriteMemory(0xc0, 1, 0x00531045);
	process->WriteMemory(0x90909090, 4, 0x0053130f);
	process->WriteMemory(0xd233c033, 4, 0x00530c9b);
	process->WriteMemory(0x81, 1, 0x00532ba1);
	process->WriteMemory(0xeb, 1, 0x0046144a);
}

void PVZ_1_0::ZNormal()
{
	process->WriteMemory(0xc8, 1, 0x00531045);
	process->WriteMemory(0x20247c2b, 4, 0x0053130f);
	process->WriteMemory(0xc28b027c, 4, 0x00530c9b);
	process->WriteMemory(0x8d, 1, 0x00532ba1);
	process->WriteMemory(0x74, 1, 0x0046144a);
}

void PVZ_1_0::ZWeak()
{
	process->WriteMemory(0xc9, 1, 0x00531045);
	process->WriteMemory(0x9090ff2b, 4, 0x0053130f);
	process->WriteMemory(0xc28bd08b, 4, 0x00530c9b);
	process->WriteMemory(0x80, 1, 0x00532ba1);
	process->WriteMemory(0x74, 1, 0x0046144a);
}

void PVZ_1_0::NoSlow(bool b)
{
	if (b)
	{
		process->WriteMemory(0x90c033, 3, 0x0052b448);
	}
	else
	{
		process->WriteMemory(0xffc083, 3, 0x0052b448);
	}
}

void PVZ_1_0::NoButter(bool b)
{
	if (b)
	{
		process->WriteMemory(0x90c033, 3, 0x0052b464);
	}
	else
	{
		process->WriteMemory(0xffc083, 3, 0x0052b464);
	}
}

void PVZ_1_0::No3zGroup(bool b)
{
	if (b)
	{
		process->WriteMemory(0xeb, 1, 0x0042694a);
		process->WriteMemory(0xeb, 1, 0x00413083);
	}
	else
	{
		process->WriteMemory(0x75, 1, 0x0042694a);
		process->WriteMemory(0x75, 1, 0x00413083);
	}
}

void PVZ_1_0::NoIMP(bool b)
{
	if (b)
	{
		process->WriteMemory(0xeb, 1, 0x00527205);
	}
	else
	{
		process->WriteMemory(0x7d, 1, 0x00527205);
	}
}

void PVZ_1_0::NoICE(bool b)
{
	if (b)
	{
		process->WriteMemory(0xc3, 1, 0x0052a7b0);
		process->WriteMemory(0xeb, 1, 0x0041f79a);
		for (int i = 0; i < 6; ++i)
		{
			process->WriteMemory(1, 4, 0x6a9ec0, 2, 0x768, 0x624 + i * 4);
		}
	}
	else
	{
		process->WriteMemory(0x51, 1, 0x0052a7b0);
		process->WriteMemory(0x75, 1, 0x0041f79a);
	}
}

void PVZ_1_0::ZEatable(bool b)
{
	if (b)
	{
		process->WriteMemory(0, 1, 0x00461457);
	}
	else
	{
		process->WriteMemory(1, 1, 0x00461457);
	}
}

void PVZ_1_0::StopZTimer(bool b)
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

void PVZ_1_0::StopZombies(bool b)
{
	if (b)
	{
		process->WriteMemory(0x54, 1, 0x0052ab2b);
		process->WriteMemory(0x54, 1, 0x0052ab34);
	}
	else
	{
		process->WriteMemory(0x64, 1, 0x0052ab2b);
		process->WriteMemory(0x44, 1, 0x0052ab34);
	}
}

void PVZ_1_0::StopBoss(bool b)
{
	if (b)
	{
		process->WriteMemory(0xc3, 1, 0x00536080);
	}
	else
	{
		process->WriteMemory(0x83, 1, 0x00536080);
	}
}

void PVZ_1_0::BalloonBoom(bool b)
{
	if (b)
	{
		process->WriteMemory(0x70, 1, 0x00525cab);
		process->WriteMemory(0x70, 1, 0x00525cc6);
	}
	else
	{
		process->WriteMemory(0x75, 1, 0x00525cab);
		process->WriteMemory(0x74, 1, 0x00525cc6);
	}
}

void PVZ_1_0::ZTimerSpeed(int value)
{
	process->WriteMemory(value, 4, eCode.ZTimer + 1);
}

void PVZ_1_0::GatherZombie(bool b, float pos)
{
	if (b)
	{
		static unsigned char code[] = { 0xc7, 0x46, 0x2c, 0, 0, 0, 0, 0xeb, 0x23 };
		(DWORD&)code[3] = (DWORD&)pos;
		process->WriteMemory(0x32, 1, 0x0052aa4a);
		process->WriteArrayMemory(code, sizeof(code), 0x0052ab18);
	}
	else
	{
		static unsigned char code[] = { 0xe8, 0xc3, 0x13, 0, 0, 0x84, 0xc0, 0x75, 0x0f };
		process->WriteMemory(0x84, 1, 0x0052aa4a);
		process->WriteArrayMemory(code, sizeof(code), 0x0052ab18);
	}
}

void PVZ_1_0::KillAllZombies()
{
	GetZombies();
	for (auto& rz : vz)
	{
		process->WriteMemory(3, 4, rz.addr + 0x28);
	}
}

void PVZ_1_0::ConfuseZombies()
{
	GetZombies();
	for (auto& rz : vz)
	{
		process->WriteMemory(1, 1, rz.addr + 0xb8);
	}
}

void PVZ_1_0::NaturalSeed(int* zombies, int znum)
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
		DWORD SeedAddr = process->ReadMemory(4, 0x6a9ec0, 1, 0x768) + 0x54d4;
		DWORD ListAddr = process->ReadMemory(4, 0x6a9ec0, 1, 0x768) + 0x6b4;
		for (int i = 0; i < 33; ++i)
		{
			process->WriteMemory(0, 1, SeedAddr + i);
		}
		for (int i = 0; i < znum; ++i)
		{
			process->WriteMemory(1, 1, SeedAddr + zombies[i]);
		}
		asm_.clear();
		asm_.mov_exx_dword_ptr(EDI, 0x6a9ec0);
		asm_.mov_exx_dword_ptr_exx_add(EDI, 0x768);
		asm_.call(0x004092e0);
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
		if (process->ReadMemory(4, 0x6a9ec0, 1, 0x7fc) == 2)
		{
			asm_.clear();
			asm_.mov_exx_dword_ptr(EBX, 0x6a9ec0);
			asm_.mov_exx_dword_ptr_exx_add(EBX, 0x768);
			asm_.call(0x0040df70);
			asm_.mov_exx_dword_ptr(EAX, 0x6a9ec0);
			asm_.mov_exx_dword_ptr_exx_add(EAX, 0x768);
			asm_.mov_exx_dword_ptr_exx_add(EAX, 0x15c);
			asm_.push_exx(EAX);
			asm_.call(0x0043a140);
			asm_.ret();
			process->WriteMemory(0x80, 1, 0x0043a153);
			SAFE_THREAD();
			process->WriteMemory(0x85, 1, 0x0043a153);
		}
	}
}

void PVZ_1_0::LimitSeed(int* zombies, int znum, bool thief, bool red)
{
	if (InGame())
	{
		DWORD SeedAddr = process->ReadMemory(4, 0x6a9ec0, 1, 0x768) + 0x54d4;
		DWORD ListAddr = process->ReadMemory(4, 0x6a9ec0, 1, 0x768) + 0x6b4;
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
				if (i >= 450 && i <= 453
					|| i >= 950 && i <= 953
					|| i >= 1450 && i <= 1453
					|| i >= 1950 && i <= 1953)
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
				if (i >= 500 && i <= 950
					|| i >= 1500 && i <= 1950)
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
		if (process->ReadMemory(4, 0x6a9ec0, 1, 0x7fc) == 2)
		{
			asm_.clear();
			asm_.mov_exx_dword_ptr(EBX, 0x6a9ec0);
			asm_.mov_exx_dword_ptr_exx_add(EBX, 0x768);
			asm_.call(0x0040df70);
			asm_.mov_exx_dword_ptr(EAX, 0x6a9ec0);
			asm_.mov_exx_dword_ptr_exx_add(EAX, 0x768);
			asm_.mov_exx_dword_ptr_exx_add(EAX, 0x15c);
			asm_.push_exx(EAX);
			asm_.call(0x0043a140);
			asm_.ret();
			process->WriteMemory(0x80, 1, 0x0043a153);
			SAFE_THREAD();
			process->WriteMemory(0x85, 1, 0x0043a153);
		}
	}
}

void PVZ_1_0::ResetSeed()
{
	if (InGame())
	{
		DWORD addr = process->ReadMemory(4, 0x6a9ec0, 1, 0x768) + 0x54d5;
		for (int i = 0; i < 33; ++i)
		{
			process->WriteMemory(0, 1, addr + i);
		}
		asm_.clear();
		asm_.mov_exx_dword_ptr(ESI, 0x6a9ec0);
		asm_.mov_exx_dword_ptr_exx_add(ESI, 0x768);
		asm_.mov_exx_dword_ptr_exx_add(ESI, 0x160);
		asm_.call(0x00425840);
		asm_.mov_exx_dword_ptr(EDI, 0x6a9ec0);
		asm_.mov_exx_dword_ptr_exx_add(EDI, 0x768);
		asm_.call(0x004092e0);
		if (process->ReadMemory(4, 0x6a9ec0, 1, 0x7fc) == 2)
		{
			asm_.mov_exx_dword_ptr(EBX, 0x6a9ec0);
			asm_.mov_exx_dword_ptr_exx_add(EBX, 0x768);
			asm_.call(0x0040df70);
			asm_.mov_exx_dword_ptr(EAX, 0x6a9ec0);
			asm_.mov_exx_dword_ptr_exx_add(EAX, 0x768);
			asm_.mov_exx_dword_ptr_exx_add(EAX, 0x15c);
			asm_.push_exx(EAX);
			asm_.call(0x0043a140);
			process->WriteMemory(0x80, 1, 0x0043a153);
		}
		asm_.ret();
		SAFE_THREAD();
		process->WriteMemory(0x85, 1, 0x0043a153);
	}
}

void PVZ_1_0::SeeLeftZombies(bool(&zombies)[33])
{
	memset(zombies, 0, sizeof(zombies));
	if (InGame())
	{
		DWORD Base = process->ReadMemory(4, 0x6a9ec0, 1, 0x768);
		DWORD WaveCount = process->ReadMemory(4, Base + 0x5564);
		DWORD CurWave = process->ReadMemory(4, Base + 0x557c);
		if (WaveCount == 0)
			return;
		DWORD LastAddr = Base + 0x6b4 + (WaveCount - 1) * 200;
		DWORD CurAddr = Base + 0x6b4 + CurWave * 200;
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

void PVZ_1_0::FlowerNPK(bool b)
{
	if (b)
	{
		process->WriteMemory(0, 1, 0x0051f017);
		process->WriteMemory(1020, 4, 0x6a9ec0, 2, 0x82c, 0x1f8);
	}
	else
	{
		process->WriteMemory(-1, 1, 0x0051f017);
	}
}

void PVZ_1_0::Medicine(bool b)
{
	if (b)
	{
		process->WriteMemory(0, 1, 0x0051f110);
		process->WriteMemory(1020, 4, 0x6a9ec0, 2, 0x82c, 0x1fc);
	}
	else
	{
		process->WriteMemory(-1, 1, 0x0051f110);
	}
}

void PVZ_1_0::Chocolate(bool b)
{
	if (b)
	{
		process->WriteMemory(0, 1, 0x0051ec38);
		process->WriteMemory(0, 1, 0x0051ecb8);
		process->WriteMemory(1020, 4, 0x6a9ec0, 2, 0x82c, 0x228);
	}
	else
	{
		process->WriteMemory(-1, 1, 0x0051ec38);
		process->WriteMemory(-1, 1, 0x0051ecb8);
	}
}

void PVZ_1_0::TreeNPK(bool b)
{
	if (b)
	{
		process->WriteMemory(0, 1, 0x0042d463);
		process->WriteMemory(1020, 4, 0x6a9ec0, 2, 0x82c, 0x230);
	}
	else
	{
		process->WriteMemory(-1, 1, 0x0042d463);
	}
}

void PVZ_1_0::TreeHeight(int h)
{
	if (GetMode() == 50)
	{
		process->WriteMemory(h - 1, 4, 0x6a9ec0, 2, 0x82c, 0xf4);
		asm_.clear();
		asm_.mov_exx(EDI, 0x6a9ec0);
		asm_.call(0x0042d1f0);
		asm_.ret();
		SAFE_THREAD();
	}
	else
		process->WriteMemory(h, 4, 0x6a9ec0, 2, 0x82c, 0xf4);
}

void PVZ_1_0::FlowerNum(int n)
{
	process->WriteMemory(n, 4, 0x6a9ec0, 2, 0x82c, 0x350);
}

void PVZ_1_0::ChangeFlower(int fid, int type)
{
	process->WriteMemory(type, 4, 0x6a9ec0, 2, 0x82c, 0x358 + fid * 0x58);
}

void PVZ_1_0::ChangeFSize(int fid, int size)
{
	process->WriteMemory(size, 4, 0x6a9ec0, 2, 0x82c, 0x37c + fid * 0x58);
}

void PVZ_1_0::ChangeFDir(int fid, int dir)
{
	process->WriteMemory(dir, 4, 0x6a9ec0, 2, 0x82c, 0x368 + fid * 0x58);
}

void PVZ_1_0::ChangeFPlace(int fid, int place)
{
	process->WriteMemory(place, 4, 0x6a9ec0, 2, 0x82c, 0x35c + fid * 0x58);
}

int PVZ_1_0::GetFlowerNum()
{
    return process->ReadMemory(4, 0x6a9ec0, 2, 0x82c, 0x350);
}

int PVZ_1_0::GetFlower(int fid)
{
    return process->ReadMemory(4, 0x6a9ec0, 2, 0x82c, 0x358 + fid * 0x58);
}

int PVZ_1_0::GetFSize(int fid)
{
    return process->ReadMemory(4, 0x6a9ec0, 2, 0x82c, 0x37c + fid * 0x58);
}

int PVZ_1_0::GetFDir(int fid)
{
    return process->ReadMemory(4, 0x6a9ec0, 2, 0x82c, 0x368 + fid * 0x58);
}

int PVZ_1_0::GetFPlace(int fid)
{
    return process->ReadMemory(4, 0x6a9ec0, 2, 0x82c, 0x35c + fid * 0x58);
}

void PVZ_1_0::Patch()
{
	process->WriteMemory(0x23, 1, 0x00534d01);
	process->WriteMemory(0x23, 1, 0x005354ba);
}