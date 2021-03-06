#pragma once
#include "PVZ.h"

namespace PVZHelper::Cli
{
	public enum class InitErr : int
    {
        Success,
        ExeNotFound,
        OpenFailed,
        NotSupport
    };
	public ref class PVZController
    {
    private:
        PVZ* pvz;

	protected:
        ~PVZController();

    public:
        //初始化游戏
        InitErr InitGame();
#pragma region 常规
        //设置阳光
        void SetSun(int sun) { pvz->SetSun(sun); }
        //设置金钱，自动乘十
        void SetMoney(int money) { pvz->SetMoney(money); }
        //自动收集
        void AutoPickup(bool b) { pvz->AutoPickup(b); }
        //无视阳光
        void IgnoreSun(bool b) { pvz->IgnoreSun(b); }
        //启动除草机
        void StartCar() { pvz->StartCar(); }
        //恢复除草机
        void ResetCar() { pvz->ResetCar(); }
        //禁止删档
        void NoDelete(bool b) { pvz->NoDelete(b); }
        //禁止存档
        void NoSave(bool b) { pvz->NoSave(b); }
        //自由放置
        void Anywhere(bool b) { pvz->Anywhere(b); }
        //允许后台
        void NoPause(bool b) { pvz->NoPause(b); }
        //传送带无延迟
        void FastBelt(bool b) { pvz->FastBelt(b); }
        //显示隐藏小游戏
        void ShowHide(bool b) { pvz->ShowHide(b); }
        //全屏浓雾
        void FullFog(bool b) { pvz->FullFog(b); }
        //浓雾透视
        void SeeFog(bool b) { pvz->SeeFog(b); }
        //花瓶透视
        void SeeVase(bool b) { pvz->SeeVase(b); }
        //种植植物
        void Planting(int row, int col, int type, bool copy)
        {
            if (type == 48)
                type = 52;
            pvz->Planting(row, col, type, copy);
        }
        //放置僵尸
        void PutZombie(int row, int col, int type)
        {
            if (type == 33)
                type = 36;
            pvz->PutZombie(row, col, type);
        }
        //放梯子
        void PutLadder(int row, int col) { pvz->PutLadder(row, col); }
        //放墓碑
        void PutGrave(int row, int col) { pvz->PutGrave(row, col); }
        //自动搭梯
        void AutoLadder() { pvz->AutoLadder(); }
        //设置块
        void SetBlock(int row, int col, int type) { pvz->SetBlock(row, col, type); }
        //行判定
        void SetRow(int row, int type) { pvz->SetRow(row, type); }
#pragma endregion
#pragma region 穿越
        //生存解密跳关
        void JmpLevel(int level) { pvz->JmpLevel(level); }
        //混合
        void MixMode(int mode, int lv)
        {
            if (lv == 0)
            {
                lv = 1;
            }
            else if (mode > 42)
            {
                mode++;
                if (mode == 50)
                {
                    mode = 60;
                }
                else if (mode == 51)
                {
                    mode = 70;
                }
            }
            pvz->MixMode(mode, lv);
        }
        //全解锁
        void Unlock(bool b) { pvz->Unlock(b); }
        //直接过关
        void Win() { pvz->Win(); }
        //锁定阵型
        void Lock(bool b, int mode) { pvz->Lock(b, mode); }
        //小僵尸
        void Little(bool b) { pvz->Little(b); }
        //下雨
        void Rain(bool b) { pvz->Rain(b); }
        //暴风雨
        void Storm(bool b) { pvz->Storm(b); }
        //引力
        void Force(bool b) { pvz->Force(b); }
        //像柱子一样
        void LikeCol(bool b) { pvz->LikeCol(b); }
        //僵尸快跑
        void RunFast(bool b) { pvz->RunFast(b); }
        //启动传送门
        void StartDoor(bool b) { pvz->StartDoor(b); }
        //固定传送门
        void StableDoor(bool b) { pvz->StableDoor(b); }
        //生成传送门
        void SetDoor(int b1r, int b1c, int b2r, int b2c, int w1r, int w1c, int w2r, int w2c)
        {
            pvz->SetDoor(b1r, b1c, b2r, b2c, w1r, w1c, w2r, w2c);
        }
#pragma endregion
#pragma region 植物
        //玉米炮
        void CornNoWait(bool b) { pvz->CornNoWait(b); }
        //磁力菇
        void MagnetNoWait(bool b) { pvz->MagnetNoWait(b); }
        //土豆雷
        void PotatoNoWait(bool b) { pvz->PotatoNoWait(b); }
        //大嘴花
        void MouthNoWait(bool b) { pvz->MouthNoWait(b); }
        //强壮
        void PStrong() { pvz->PStrong(); }
        //正常
        void PNormal() { pvz->PNormal(); }
        //虚弱
        void PWeak() { pvz->PWeak(); }
        //毁灭菇无痕
        void NoMark(bool b) { pvz->NoMark(b); }
        //蘑菇免唤醒
        void NoSleep(bool b) { pvz->NoSleep(b); }
        //强三叶草
        void StrongWind(bool b) { pvz->StrongWind(b); }
        //强豌豆
        void StrongBean(bool b) { pvz->StrongBean(b); }
        //保护伞
        void Umbrella(bool b) { pvz->Umbrella(b); }
        //无冷却
        void NoCD(bool b) { pvz->NoCD(b); }
        //铲子锁定
        void LockScoop(bool b) { pvz->LockScoop(b); }
        //宝石随心移
        void AnyMove(bool b) { pvz->AnyMove(b); }
        //紫卡无限制
        void NoPurple(bool b) { pvz->NoPurple(b); }
        //锁定黄油
        void LockButter(bool b) { pvz->LockButter(b); }
        //锁定玉米
        void LockCorn(bool b) { pvz->LockCorn(b); }
        //爆炸全屏
        void FSBoom(bool b) { pvz->FSBoom(b); }
        //窝瓜全屏
        void FSSquash(bool b) { pvz->FSSquash(b); }
        //磁力菇全屏
        void FSMagnet(bool b) { pvz->FSMagnet(b); }
        //溅射全屏
        void FSSputter(bool b) { pvz->FSSputter(b); }
        //大嘴花全屏
        void FSMouth(bool b) { pvz->FSMouth(b); }
        //地刺全屏
        void FSPrick(bool b) { pvz->FSPrick(b); }
        //喷射全屏
        void FSSpout(bool b) { pvz->FSSpout(b); }
        //猫式全屏
        void FSCat(bool b) { pvz->FSCat(b); }
        //触碰伤害
        void CatTouch(bool b) { pvz->CatTouch(b); }
#pragma endregion
#pragma region 僵尸
        //立即爆炸
        void BoomIm() { pvz->BoomIm(); }
        //正常
        void BoomNormal() { pvz->BoomNormal(); }
        //不爆炸
        void BoomNever() { pvz->BoomNever(); }
        //隐形
        void VisiableNo() { pvz->VisiableNo(); }
        //正常
        void VisiableNormal() { pvz->VisiableNormal(); }
        //可见
        void VisiableYes() { pvz->VisiableYes(); }
        //无敌
        void ZStrong() { pvz->ZStrong(); }
        //正常
        void ZNormal() { pvz->ZNormal(); }
        //虚弱
        void ZWeak() { pvz->ZWeak(); }
        //不减速
        void NoSlow(bool b) { pvz->NoSlow(b); }
        //黄油
        void NoButter(bool b) { pvz->NoButter(b); }
        //屏蔽3人组
        void No3zGroup(bool b) { pvz->No3zGroup(b); }
        //不扔小鬼
        void NoIMP(bool b) { pvz->NoIMP(b); }
        //冰车无痕
        void NoICE(bool b) { pvz->NoICE(b); }
        //巨人可被吃
        void ZEatable(bool b) { pvz->ZEatable(b); }
        //暂停刷僵尸
        void StopZTimer(bool b) { pvz->StopZTimer(b); }
        //僵尸不前进
        void StopZombies(bool b) { pvz->StopZombies(b); }
        //定BOSS
        void StopBoss(bool b) { pvz->StopBoss(b); }
        //气球自爆
        void BalloonBoom(bool b) { pvz->BalloonBoom(b); }
        //加速刷僵尸
        void ZTimerSpeed(int value) { pvz->ZTimerSpeed(value); }
        //吸怪
        void GatherZombie(bool b, float pos) { pvz->GatherZombie(b, pos); }
        //秒杀
        void KillAllZombies() { pvz->KillAllZombies(); }
        //魅惑
        void ConfuseZombies() { pvz->ConfuseZombies(); }
#pragma endregion
#pragma region 调节
        //卡槽数量
        void SetCardNum(int num) { pvz->SetCardNum(num); }
        //修改卡片
        void SetCard(int cid, int type, bool copy)
        {
            if (type == 48)
                type = 52;
            pvz->SetCard(cid, type, copy);
        }
        //自然出怪
        void NaturalSeed(array<int>^ zombies);
        //极限出怪
        void LimitSeed(array<int>^ zombies, bool thief, bool red);
        //恢复初始
        void ResetSeed() { pvz->ResetSeed(); }
        //查看剩余
        System::Collections::Generic::IList<int>^ SeeLeftZombies();
#pragma endregion
#pragma region 花园
        //花肥
        void FlowerNPK(bool b) { pvz->FlowerNPK(b); }
        //杀虫剂
        void Medicine(bool b) { pvz->Medicine(b); }
        //巧克力
        void Chocolate(bool b) { pvz->Chocolate(b); }
        //树肥
        void TreeNPK(bool b) { pvz->TreeNPK(b); }
        //树高
        void TreeHeight(int h) { pvz->TreeHeight(h); }
        //花数
        void FlowerNum(int n) { pvz->FlowerNum(n); }
        //改花
        void ChangeFlower(int fid, int type) { pvz->ChangeFlower(fid, type); }
        //改花大小
        void ChangeFSize(int fid, int size) { pvz->ChangeFSize(fid, size); }
        //改花方向
        void ChangeFDir(int fid, int dir) { pvz->ChangeFDir(fid, dir); }
        //改花位置
        void ChangeFPlace(int fid, int place) { pvz->ChangeFPlace(fid, place); }
        //获取花数量
        int GetFlowerNum() { return pvz->GetFlowerNum(); }
        //花种类
        int GetFlower(int fid) { return pvz->GetFlower(fid); }
        //花大小
        int GetFSize(int fid) { return pvz->GetFSize(fid); }
        //花方向
        int GetFDir(int fid) { return pvz->GetFDir(fid); }
        //花位置
        int GetFPlace(int fid) { return pvz->GetFPlace(fid); }
#pragma endregion
    };
} // namespace PVZHelper::Cli
