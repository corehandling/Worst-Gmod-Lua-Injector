#pragma once

#include "LuaFunctions.h"

lua_State* WaitForLuaState();

void Execute(lua_State* L, const char* source);