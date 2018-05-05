#pragma once

#include "Process.h"
#include "Asm.h"
#include <vector>

#define _call(addr)	\
	call $ + 7}\
	_asm{jmp short $ + 8}\
	_asm{push addr}\
	_asm{ret

#define _jmp(addr)	\
	push addr}\
	_asm{ret

#define GET_ADDR(f)	(unsigned char*)f
#define HEAD_OF(f)	(*GET_ADDR(f) == 0xeb ? 2 : 5)
#define SIZE_OF(f)	(*GET_ADDR(f) == 0xeb ? *(GET_ADDR(f)+1) : *(unsigned int*)(GET_ADDR(f)+1))

#define DECLARE_VIRTUAL(flag)	\
	virtual bool InGame()##flag\
	virtual int	 GetMode()##flag\
	virtual void SetSun(int)##flag\
	virtual void SetMoney(int)##flag\
	virtual void IgnoreSun(bool)##flag\
	virtual void AutoPickup(bool)##flag\
	virtual void StartCar()##flag\
	virtual void ResetCar()##flag\
	virtual void Planting(int row, int col, int type, bool copy)##flag\
	virtual void NoPause(bool)##flag\
	virtual void PutZombie(int row, int col, int type)##flag\
	virtual void PutLadder(int row, int col)##flag\
	virtual void PutGrave(int row, int col)##flag\
	virtual void AutoLadder()##flag\
	/*virtual void PutRake(int row, int col)##flag*/\
	virtual void SetBlock(int row, int col, int type)##flag\
	virtual void SetRow(int row, int type)##flag\
	virtual void NoDelete(bool)##flag\
	virtual void NoSave(bool)##flag\
	virtual void Anywhere(bool)##flag\
	virtual void FastBelt(bool)##flag\
	virtual void ShowHide(bool)##flag\
	virtual void FullFog(bool)##flag\
	virtual void SeeFog(bool)##flag\
	virtual void SeeVase(bool)##flag\
	virtual void JmpLevel(int)##flag\
	virtual void MixMode(int mode, int level = 0)##flag\
	virtual void Unlock(bool)##flag\
	virtual void Win()##flag\
	virtual void Lock(bool,int = 0)##flag\
	virtual void Little(bool)##flag\
	virtual void Rain(bool)##flag\
	virtual void Storm(bool)##flag\
	virtual void Force(bool)##flag\
	virtual void LikeCol(bool)##flag\
	virtual void RunFast(bool)##flag\
	virtual void SetCardNum(int)##flag\
	virtual void SetCard(int cid, int type, bool copy = false)##flag\
	virtual void FSBoom(bool)##flag\
	virtual void FSSquash(bool)##flag\
	virtual void FSMagnet(bool)##flag\
	virtual void FSSputter(bool)##flag\
	virtual void FSMouth(bool)##flag\
	virtual void FSPrick(bool)##flag\
	virtual void FSSpout(bool)##flag\
	virtual void FSCat(bool)##flag\
	virtual void CatTouch(bool)##flag\
	virtual void InitExtraCode()##flag\
	virtual void StartDoor(bool)##flag\
	virtual void StableDoor(bool)##flag\
	virtual void SetDoor(int b1r, int b1c, int b2r, int b2c, int w1r, int w1c, int w2r, int w2c)##flag\
	virtual void CornNoWait(bool)##flag\
	virtual void MagnetNoWait(bool)##flag\
	virtual void PotatoNoWait(bool)##flag\
	virtual void MouthNoWait(bool)##flag\
	virtual void PStrong()##flag\
	virtual void PNormal()##flag\
	virtual void PWeak()##flag\
	virtual void NoMark(bool)##flag\
	virtual void NoSleep(bool)##flag\
	virtual void StrongWind(bool)##flag\
	virtual void StrongBean(bool)##flag\
	virtual void Umbrella(bool)##flag\
	virtual void NoCD(bool)##flag\
	virtual void LockScoop(bool)##flag\
	virtual void AnyMove(bool)##flag\
	virtual void NoPurple(bool)##flag\
	virtual void LockButter(bool)##flag\
	virtual void LockCorn(bool)##flag\
	virtual void BoomIm()##flag\
	virtual void BoomNormal()##flag\
	virtual void BoomNever()##flag\
	virtual void VisiableNo()##flag\
	virtual void VisiableNormal()##flag\
	virtual void VisiableYes()##flag\
	virtual void ZStrong()##flag\
	virtual void ZNormal()##flag\
	virtual void ZWeak()##flag\
	virtual void NoSlow(bool)##flag\
	virtual void NoButter(bool)##flag\
	virtual void No3zGroup(bool)##flag\
	virtual void NoIMP(bool)##flag\
	virtual void NoICE(bool)##flag\
	virtual void ZEatable(bool)##flag\
	virtual void StopZTimer(bool)##flag\
	virtual void StopZombies(bool)##flag\
	virtual void StopBoss(bool)##flag\
	virtual void BalloonBoom(bool)##flag\
	virtual void ZTimerSpeed(int)##flag\
	virtual void GatherZombie(bool, float = 0.0f)##flag\
	virtual void KillAllZombies()##flag\
	virtual void ConfuseZombies()##flag\
	virtual void NaturalSeed(int* zombies, int znum)##flag\
	virtual void LimitSeed(int* zombies, int znum, bool thief, bool red)##flag\
	virtual void ResetSeed()##flag\
	virtual void SeeLeftZombies(bool(&zombies)[33])##flag\
	virtual void FlowerNPK(bool)##flag\
	virtual void Medicine(bool)##flag\
	virtual void Chocolate(bool)##flag\
	virtual void TreeNPK(bool)##flag\
	virtual void TreeHeight(int)##flag\
	virtual void FlowerNum(int)##flag\
	virtual void ChangeFlower(int fid, int type)##flag\
	virtual void ChangeFSize(int fid, int size)##flag\
	virtual void ChangeFDir(int fid, int dir)##flag\
	virtual void ChangeFPlace(int fid, int place)##flag\
    virtual int GetFlowerNum()##flag\
	virtual int GetFlower(int fid)##flag\
	virtual int GetFSize(int fid)##flag\
	virtual int GetFDir(int fid)##flag\
	virtual int GetFPlace(int fid)##flag\
	virtual void Patch()##flag\


struct Object
{
	DWORD addr;
};

struct Plant : public Object
{
	int Row;
	int Col;
	int Type;
	bool Sleeping;
};

struct Item : public Object
{
	int Row;
	int Col;
	int Type;
};

struct Car : public Object
{
	int Row;
};

struct Zombie : public Object
{
	int Row;
	int Type;
};


class PVZ
{
public:
	PVZ(Process& p) :asm_(2){ process = new Process(p); }
	~PVZ(){ delete process; }
	DECLARE_VIRTUAL(= 0;)
public:
	Process* process;
	std::vector<Plant> vp;
	std::vector<Item> vi;
	std::vector<Car> vc;
	std::vector<Zombie> vz;
	Asm asm_;

	struct ExtraCodeAddr
	{
		DWORD Row;
		DWORD Col;
		DWORD RHurt;
		DWORD CHurt;
		DWORD CatR;
		DWORD CatC;
		DWORD Throw;
		DWORD CatDir;
		DWORD Split;
		DWORD ZTimer;
		DWORD Thread;
	}eCode;
};

struct Map
{
	Map(Process* p){ process = p; }
	Process* process;
	int Type;
	int GetRowCount()
	{
		return Type == 2 || Type == 3 ? 6 : 5;
	}
};

#define DECLARE_ASM	\
	void asm_plant(int row, int col, int type, bool copy); \
	void asm_plant_paper(int row, int col, int type); \
	void asm_put_zombie(int row, int col, int type); \
	void asm_put_ladder(int row, int col); \
	void asm_put_grave(int row, int col);

