#include "stdafx.h"
#include "..\PVZHelper\PVZ_1_0.h"
#include "..\PVZHelper\PVZ_1_2_CN_ENDLESS.h"
#include "..\PVZHelper\PVZ_1_2_CN_REPAIR.h"
#include "..\PVZHelper\PVZ_1_2_EN_1073.h"
#include "PVZHelper.Cli.h"
#pragma comment(lib,"User32.lib")
#pragma comment(lib,"Advapi32.lib")
using namespace PVZHelper::Cli;

InitErr PVZController::InitGame()
{
    Process process;
    switch (process.OpenByName((char*)"popcapgame1.exe"))
    {
    case NOT_FOUND:
        switch (process.OpenByName((char*)"PlantsVsZombies.exe"))
        {
        case NOT_FOUND:
            return InitErr::ExeNotFound;
        case OPEN_FAILED:
            return InitErr::OpenFailed;
        case SUCCESS:
            break;
        }
        break;
    case OPEN_FAILED:
        return InitErr::OpenFailed;
    case SUCCESS:
        break;
    }
    if (process.IsValid())
    {
        delete pvz;
        if (process.ReadMemory(4, 0x42748e) == 0xff563de8)
        {
            pvz = new PVZ_1_0(process);
        }
        else if (process.ReadMemory(4, 0x429a83) == 0xffff7489)
        {
            pvz = new PVZ_1_2_EN_1073(process);
        }
        else if (process.ReadMemory(4, 0x428902) == 0x03c109e8)
        {
            pvz = new PVZ_1_2_CN_REPAIR(process);
        }
        else if (process.ReadMemory(4, 0x434d57) == 0x5b5d5e5f)
        {
            pvz = new PVZ_1_2_CN_ENDLESS(process);
        }
        else
        {
            pvz = NULL;
            return InitErr::NotSupport;
        }
        pvz->Patch();
        pvz->InitExtraCode();
    }
    return InitErr::Success;
}

array<bool>^ PVZController::SeeLeftZombies()
{
    bool zombies[33];
    pvz->SeeLeftZombies(zombies);
    array<bool>^ result = gcnew array<bool>(33);
    for (int i = 0; i < 33; i++)
    {
        result[i] = zombies[i];
    }
    return result;
}
