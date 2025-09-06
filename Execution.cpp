#include "Execution.h"
#include "MinHook/MinHook.h"
#include <stdio.h>

lua_pcall_t o_lua_pcall = nullptr;
lua_State* g_CapturedLuaState = nullptr;
bool g_bStateCaptured = false;

int __cdecl hk_lua_pcall(lua_State* L, int nargs, int nresults, int errfunc)
{
    if (!g_bStateCaptured && L && lua_getfield && lua_toboolean)
    {
        lua_getfield(L, LUA_GLOBALSINDEX, "CLIENT");
        bool isClient = lua_toboolean(L, -1) != 0;
        lua_settop(L, -2);
        if (isClient)
        {
            g_CapturedLuaState = L;
            g_bStateCaptured = true;
        }
    }
    return o_lua_pcall(L, nargs, nresults, errfunc);
}

lua_State* WaitForLuaState()
{
    if (MH_Initialize() != MH_OK)
    {
        return nullptr;
    }
	if (!LoadLuaFunctions())
	{
        return nullptr;
	}
    if (MH_CreateHook(lua_pcall, &hk_lua_pcall, (void**)&o_lua_pcall) != MH_OK)
    {
        return nullptr;
    }
    if (MH_EnableHook(lua_pcall) != MH_OK)
    {
        return nullptr;
    }
    while (!g_bStateCaptured)
    {
        Sleep(1000);
    }
    g_bStateCaptured = false;
    MH_DisableHook(o_lua_pcall);
    MH_RemoveHook(o_lua_pcall);
    MH_Uninitialize();
	return g_CapturedLuaState;
}

void Execute(lua_State* L, const char* source)
{
    HMODULE hModule = GetModuleHandleA("tier0.dll");
    if (!hModule) return;
    uintptr_t funcAddress = (uintptr_t)GetProcAddress(hModule, "ThreadInMainThread");
    if (!funcAddress) return;
    DWORD g_MainThreadId = 0;
    BYTE* funcBytes = (BYTE*)funcAddress;
    for (int i = 0; i < 50; i++)
    {
        // cmp eax, dword
        if (funcBytes[i] == 0x3B && funcBytes[i + 1] == 0x05)
        {
            uintptr_t offset = *(uintptr_t*)(funcBytes + i + 2);
            g_MainThreadId = *(DWORD*)offset;
            break;
        }
    }
    if (!g_MainThreadId) return;
    BYTE originalBytes[3];
    memcpy(originalBytes, funcBytes, sizeof(originalBytes));
    HANDLE hMainThread = OpenThread(THREAD_ALL_ACCESS, FALSE, g_MainThreadId);
    if (hMainThread)
    {
        SuspendThread(hMainThread);
        DWORD oldProtect;
        if (VirtualProtect((void*)funcAddress, sizeof(originalBytes), PAGE_EXECUTE_READWRITE, &oldProtect))
        {
            // mov al, 1; ret
            BYTE patch[] = { 0xB0, 0x01, 0xC3 };
            memcpy((void*)funcAddress, patch, sizeof(patch));
            if (L && source && luaL_loadstring(L, source) == 0)
            {
                lua_pcall(L, 0, 0, 0);
                lua_settop(L, 0);
            }
            memcpy((void*)funcAddress, originalBytes, sizeof(originalBytes));
            VirtualProtect((void*)funcAddress, sizeof(originalBytes), oldProtect, &oldProtect);
        }
        ResumeThread(hMainThread);
        CloseHandle(hMainThread);
    }
}