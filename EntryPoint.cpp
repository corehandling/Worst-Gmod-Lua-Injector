#include <Windows.h>

#include "Execution.h"
#include "Environment.h"

void PipeServer()
{
    while (true)
    {
        HANDLE hPipe = CreateNamedPipeA(
            "\\\\.\\pipe\\GmodPipe",
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            65536,
            65536,
            0,
            nullptr
        );
        if (hPipe == INVALID_HANDLE_VALUE)
        {
            Sleep(1000);
            continue;
        }
        if (ConnectNamedPipe(hPipe, nullptr) || GetLastError() == ERROR_PIPE_CONNECTED)
        {
            DWORD messageLength = 0;
            DWORD bytesRead = 0;
            if (ReadFile(hPipe, &messageLength, sizeof(messageLength), &bytesRead, nullptr))
            {
                char* luaCode = new char[messageLength + 1];
                if (ReadFile(hPipe, luaCode, messageLength, &bytesRead, nullptr))
                {
                    luaCode[bytesRead] = '\0';
                    lua_State* LuaState = WaitForLuaState();
                    if (LuaState)
                    {
                        InitializeEnvironment(LuaState);
                        Execute(LuaState, luaCode);
                    }
                }

                delete[] luaCode;
            }
        }
        DisconnectNamedPipe(hPipe);
        CloseHandle(hPipe);
    }
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)PipeServer, nullptr, 0, nullptr);
    }
    return TRUE;
}