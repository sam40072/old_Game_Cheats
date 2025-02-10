// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <Minhook.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <chrono>




uintptr_t base = (uintptr_t)GetModuleHandle(NULL);
uintptr_t GameAssembly = (uintptr_t)GetModuleHandle("GameAssembly.dll");


//changed to StartGame()
uintptr_t isEveryoneReadyOffset = 0x3DCA10;

typedef void(__fastcall* isEveryoneReadyType)(void* _this);
isEveryoneReadyType isEveryoneReadyOriginal;
void __fastcall isEveryoneReadyHook(void* _this)
{
    std::cout << "start game" << std::endl;
    
}


uintptr_t startGameOffset = 0x3F6B70; //need 

typedef void(__fastcall* startGameType)(void* _this);
startGameType startGameOriginal;

void __fastcall startGameHook(void* _this)
{
    std::cout << "game starting " << std::endl;
    startGameOriginal(_this);
}


//chat message
uintptr_t chatMessageOffset = 0x3DC930;
typedef void(__fastcall* SendChatMessageType)(void* _this, char* message);

SendChatMessageType ogCmdSendMessage;

void __fastcall tweakMessages(void* _this, char* message)
{
    std::cout << "sent message" << std::endl;
    isEveryoneReadyOriginal(_this);
}
//


//canSetTrashOnFire
uintptr_t spawnItemOffset = 0x4853A0; //need (putonfire)

typedef bool(__fastcall* CmdSpawnItemType)(void* _this);
CmdSpawnItemType CmdSpawnItem;
bool __fastcall spawnItem(void* _this)
{
    return 1;
}


//canFirePlayer
uintptr_t canFirePlayerOffset = 0x403830; //need

typedef bool(__fastcall* CanFirePlayerType)(void* _this);
CanFirePlayerType canFirePlaya;
bool __fastcall CanFire(void* _this)
{
    return 1;
}

//isManager
uintptr_t isManagerOffset = 0x4043C0; //need

typedef bool(__fastcall* IsManagerType)(void* _this);
IsManagerType isManager;
bool __fastcall amImanager(void* _this)
{
    return 1;
}

//canCallMeeting

uintptr_t canCallMeetingOffset = 0x403720; //need

typedef bool(__fastcall* canCallMeetingType)(void* _this);
canCallMeetingType canCallMeetingOg;
bool __fastcall canCallMeeting(void* _this)
{
    return 1;
}

//canLockDoor

uintptr_t CanLockDoorsOffset = 0x471360; //need

typedef bool(__fastcall* canLockDoorsType)(void* _this);
canLockDoorsType canLockDoorsOg;

bool __fastcall canLockDoorsHook(void* _this)
{
    return 1;
}

uintptr_t CanUnlockDoorsOffset = 0x4713A0; //need

typedef bool(__fastcall* canUnlockDoorsType)(void* _this);
canUnlockDoorsType canUnlockDoorsOg;

bool __fastcall canUnlockDoorsHook(void* _this)
{
    return 1;
}

//canpromote

uintptr_t CanPromoteOffset = 0x494B20; //need

typedef bool(__fastcall* canPromoteType)(void* _this);
canPromoteType canPromoteOg;

bool __fastcall canPromoteHook(void* _this)
{
    return 1;
}

//canDemote
uintptr_t CanDemoteOffset = 0x494A00; //need

typedef bool(__fastcall* canDemoteType)(void* _this);
canDemoteType canDemoteOg;

bool __fastcall canDemoteHook(void* _this)
{
    return 1;
}

//isFiredTestSet

uintptr_t RpcFirePlayerOffset = 0x4710B0;

typedef void(__fastcall* RpcFirePlayerType)(void* _this, bool value);
RpcFirePlayerType originalRpcFirePlayer;

// Hook function for RpcFirePlayer
void __fastcall hookedRpcFirePlayer(void* _this, bool value)
{
    originalRpcFirePlayer(_this, 0);
}


uintptr_t changeManagerOffset = 0x402580; //need

typedef void(__fastcall* serverChangeManagerType)(void* _this);
serverChangeManagerType originalChangeManager;

void __fastcall changeManagerHook(void* _this)
{
    originalChangeManager(_this);
}


//infinite vote


uintptr_t canVoteOffset = 0x401C00; //need


typedef bool(__fastcall* canVoteForType)(void* _this);
canVoteForType canVoteOG;

typedef void(__fastcall* GetNetworkisHostType)(void* _this, bool value);
GetNetworkisHostType originalGetNetworkisHost;


std::chrono::steady_clock::time_point lastCallTime;
const int cooldownDuration = 2000; // Cooldown duration in milliseconds (2 seconds

bool __fastcall canVoteHook(void* _this)
{
    auto now = std::chrono::steady_clock::now();
    auto durationSinceLastCall = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastCallTime).count();

    if (durationSinceLastCall >= cooldownDuration)
    {
        originalChangeManager(_this);

        // Update the last call time
        lastCallTime = now;
    }
    else
    {
        
    }
    return 1;
}

uintptr_t isHostOffset = 0x4BB450; //need


// Hook function for get_NetworkisHost
void __fastcall hookedGetNetworkisHost(void* _this, bool value)
{
    std::cout << "get_NetworkisHost called. Forcing return to true." << std::endl;
    originalGetNetworkisHost(_this, 1);
}


uintptr_t replaceWithPlayerOffset = 0x436620;

typedef void(__fastcall* replaceWithPlayerType)(void* _this);
replaceWithPlayerType replaceWithPlayerOriginal;

void __fastcall replaceWithPlayerHook(void* _this)
{
    replaceWithPlayerOriginal(_this);
}

//spectator

uintptr_t setServerRoleOffset = 0x436480;

typedef void(__fastcall* serverSetPlayerRoleType)(void* _this, int playerRole);
serverSetPlayerRoleType originalSetServerRole;

void __fastcall serverSetRoleHook(void* _this, int playerRole)
{
    std::cout << "set server role called" << std::endl;
    std::cout << playerRole << std::endl;
    return originalSetServerRole(_this, 1);
    std::cout << "replacing??" << std::endl;
    return replaceWithPlayerOriginal(_this);
}

///start lobby player

uintptr_t startLobbyPlayerOffset = 0x4B9730; //need

typedef void(__fastcall* startLobbyPlayerType)(void* _this);
startLobbyPlayerType startLobbyPlayerOriginal;


uintptr_t cheatModeAwakeOffset = 0x3F2EA0;

typedef void(__fastcall* cheatModeAwakeType)(void* _this);
cheatModeAwakeType cheatModeAwakeOriginal;

void __fastcall cheatModeAwakeHook(void* _this)
{
    cheatModeAwakeOriginal(_this);
}

void __fastcall startLobbyPlayerHook(void* _this)
{
   
    originalGetNetworkisHost(_this, 1);
    startLobbyPlayerOriginal(_this);
}

uintptr_t kickPlayerOffset = 0x45A2E0; //button
uintptr_t kickPlayerFunctionOffset = 0x3F5AE0; //function
uintptr_t protectedBullshitOffset = 0x3F7690; //protected
uintptr_t finalKickPlayerOffset = 0x3F5A30; //rpc

typedef void(__fastcall* finalKickPlayerType)(void* _this, void* NetworkIdentity);
finalKickPlayerType finalKickPLayerOriginal;

void __fastcall finalKickPlayerHook(void* _this, void* Networkidentity)
{
    std::cout << "can't kick me straight function" << std::endl;
}

typedef void(__fastcall* protectedBullshitTpye)(void* _this, void* NetworkIdentity);
protectedBullshitTpye protectedBullshitOriginal;

void __fastcall protectedBullshitHook(void* _this, void* NetworkIdentity)
{
    
}

typedef void(__fastcall* kickPlayerType)(void* _this);
kickPlayerType kickPlayerOriginal;

typedef void(__fastcall* kickPlayerFunctionType)(void* _this, void* NetworkIdentity);
kickPlayerFunctionType kickPlayerFunctionOriginal;

void __fastcall kickPlayerHook(void* _this)
{
    std::cout << "button press kick" << std::endl;
    protectedBullshitOriginal(_this, _this);
}

uintptr_t promoteToAssistantOffset = 0x477AA0;

typedef void(__fastcall* promoteToAssistantType)(void* _this, void* NetworkIdentity);
promoteToAssistantType promoteToAssistantOriginal;

void __fastcall kickPlayerFunctionHook(void* _this, void* NetworkIdentity)
{
    std::cout << "kick player function hook" << std::endl;

}

void __fastcall promoteToAssistantHook(void* _this, void* NetworkIdentity)
{
    std::cout << "just trying all kicks" << std::endl;

    
    
}


typedef void(__fastcall* set_canInteracType)(void* _this, bool value);
set_canInteracType set_canInteractOriginal;

uintptr_t set_canInteractOffset = 0x4B9270;

void __fastcall set_canInteractHook(void* _this, bool value)
{
    std::cout << "can interact set to 1" << std::endl;
    set_canInteractOriginal(_this, 1);
}

uintptr_t CmdFirePlayerOffset = 0x438CF0;

typedef void(__fastcall* CmdFirePlayerType)(void* _this, void* NetworkIdentity, bool onlydemote, bool resestDesk);
CmdFirePlayerType CmdFirePlayerOriginal;

void __fastcall CmdFirePlayerFunction(void* _this, void* NetworkIdentity, bool onlydemote, bool resestDesk)
{
    std::cout << "CmdFirePlayer called onlydemote: " << onlydemote << " reset desk: " << resestDesk << std::endl;
    CmdFirePlayerOriginal(_this, NetworkIdentity, onlydemote, resestDesk);
}

uintptr_t startGhostOFfset = 0x442CF0;

typedef void(__fastcall* startGhostType)(void* _this);
startGhostType startGhostOriginal;

void __fastcall startGhostHook(void* _this)
{
    std::cout << "start ghost function called " << std::endl;
}

uintptr_t changeLevelOffset = 0x3C32E0;

typedef void(__fastcall* changeLevelType)(void* _this, int inc);
changeLevelType changeLevelOriginal;

void __fastcall changeLevelHook(void* _this, int inc)
{
    std::cout << "level int: " << inc << std::endl;
    changeLevelOriginal(_this, inc);
}

uintptr_t setNetworkJanitorOffset = 0x440880;

typedef void(__fastcall* setJanitorAmount)(void* _this, int value);
setJanitorAmount setNetworkJanitorsOriginal;

void __fastcall setNetworkJanitorshook(void* _this, int value)
{
    setNetworkJanitorsOriginal(_this, 99);
}








void createConsole()
{
        AllocConsole();
        // Redirect standard output and standard error
        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);
        freopen_s(&f, "CONOUT$", "w", stderr);
        // Redirect standard input
        freopen_s(&f, "CONIN$", "r", stdin);

        // Redirect wide character output streams
        std::ios::sync_with_stdio(false);
        std::wcout.sync_with_stdio(false);
        std::wcin.sync_with_stdio(false);
}

void init()
{
    MH_Initialize();
    createConsole();
    printf("starting...\n");
}




void main()
{
    init();
    //if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + chatMessageOffset), &tweakMessages, reinterpret_cast<LPVOID*>(&ogCmdSendMessage)) == MH_OK)
    //{
    //    MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + chatMessageOffset));
    //    printf("unlock all skins hooked.\n");
   // }
   // else
    //{
    //    printf("Hook creation failed\n");
   // }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + spawnItemOffset), &spawnItem, reinterpret_cast<LPVOID*>(&CmdSpawnItem)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + spawnItemOffset));
        printf("you can now set trashcans on fire.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + canFirePlayerOffset), &CanFire, reinterpret_cast<LPVOID*>(&canFirePlaya)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + canFirePlayerOffset));
        printf("you can now fire players.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + isManagerOffset), &amImanager, reinterpret_cast<LPVOID*>(&isManager)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + isManagerOffset));
        printf("you are the manager maybe??\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + canCallMeetingOffset), &canCallMeeting, reinterpret_cast<LPVOID*>(&canCallMeetingOg)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + canCallMeetingOffset));
        printf("you can now call meetings.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + CanLockDoorsOffset), &canLockDoorsHook, reinterpret_cast<LPVOID*>(&canLockDoorsOg)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + CanLockDoorsOffset));
        printf("you can now lock doors.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + CanUnlockDoorsOffset), &canUnlockDoorsHook, reinterpret_cast<LPVOID*>(&canUnlockDoorsOg)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + CanUnlockDoorsOffset));
        printf("you can now unlock doors.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + CanPromoteOffset), &canPromoteHook, reinterpret_cast<LPVOID*>(&canPromoteOg)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + CanPromoteOffset));
        printf("you can now promote people.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + CanDemoteOffset), &canDemoteHook, reinterpret_cast<LPVOID*>(&canDemoteOg)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + CanDemoteOffset));
        printf("you can now demote players.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + isHostOffset), &hookedGetNetworkisHost, reinterpret_cast<LPVOID*>(&originalGetNetworkisHost)) == MH_OK)
    {   
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + isHostOffset));
        printf("force host.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + canVoteOffset), &canVoteHook, reinterpret_cast<LPVOID*>(&canVoteOG)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + canVoteOffset));
        printf("can vote hook.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + changeManagerOffset), &changeManagerHook, reinterpret_cast<LPVOID*>(&originalChangeManager)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + changeManagerOffset));
        printf("change manager with one vote hook.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + startLobbyPlayerOffset), &startLobbyPlayerHook, reinterpret_cast<LPVOID*>(&startLobbyPlayerOriginal)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + startLobbyPlayerOffset));
        printf("start() lobby player for forcing host hook.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + kickPlayerOffset), &kickPlayerHook, reinterpret_cast<LPVOID*>(&kickPlayerOriginal)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + kickPlayerOffset));
        printf("kick player hook.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + kickPlayerFunctionOffset), &kickPlayerFunctionHook, reinterpret_cast<LPVOID*>(&kickPlayerFunctionOriginal)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + kickPlayerFunctionOffset));
        printf("kick function hook.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + startGameOffset), &startGameHook, reinterpret_cast<LPVOID*>(&startGameOriginal)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + startGameOffset));
        printf("start game function hook.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    //if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + promoteToAssistantOffset), &promoteToAssistantHook, reinterpret_cast<LPVOID*>(&promoteToAssistantOriginal)) == MH_OK)
   // {
    //    MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + promoteToAssistantOffset));
    //    printf("promote to asistant hook");
   // }
  //  else
   // {
  //      printf("Hook creation failed\n");
  //  }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + protectedBullshitOffset), &protectedBullshitHook, reinterpret_cast<LPVOID*>(&protectedBullshitOriginal)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + protectedBullshitOffset));
        printf("kick function hook.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + finalKickPlayerOffset), &finalKickPlayerHook, reinterpret_cast<LPVOID*>(&finalKickPLayerOriginal)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + finalKickPlayerOffset));
        printf("final kick function hook.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }
    if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + setNetworkJanitorOffset), &setNetworkJanitorshook, reinterpret_cast<LPVOID*>(&setNetworkJanitorsOriginal)) == MH_OK)
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + setNetworkJanitorOffset));
        printf("set jantiros function hook.\n");
    }
    else
    {
        printf("Hook creation failed\n");
    }

   // if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + chatMessageOffset), &tweakMessages, reinterpret_cast<LPVOID*>(&ogCmdSendMessage)) == MH_OK)
    //{
      //  MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + chatMessageOffset));
       // printf("final kick function hook.\n");
    //}
    //else
    //{
     //   printf("Hook creation failed\n");
    //}

    //if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + setServerRoleOffset), &serverSetRoleHook, reinterpret_cast<LPVOID*>(&originalSetServerRole)) == MH_OK)
    //{
      //  MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + setServerRoleOffset));
        //printf("set server role hook.\n");
    //}
    //else
    //{
      //  printf("Hook creation failed\n");
    //}
    //if (MH_CreateHook(reinterpret_cast<LPVOID>(GameAssembly + replaceWithPlayerOffset), &replaceWithPlayerHook, reinterpret_cast<LPVOID*>(&replaceWithPlayerOriginal)) == MH_OK)
    //{
      //  MH_EnableHook(reinterpret_cast<LPVOID>(GameAssembly + replaceWithPlayerOffset));
        //printf("replace with player hook.\n");
    //}
    //else
    //{
      //  printf("Hook creation failed\n");
    //}

    

}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main,0,0,0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

