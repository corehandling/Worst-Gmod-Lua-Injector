#include "Environment.h"

lua_State* g_InitializedState = nullptr;

int getrawmetatable(lua_State* rl)
{
    luaL_checkany(rl, 1);
    if (!lua_getmetatable(rl, 1))
    {
        lua_pushnil(rl);
    }
    return 1;
}

void push_global(lua_State* L, const char* name, lua_CFunction func)
{
    lua_pushcclosure(L, func, 0);
    lua_setfield(L, LUA_GLOBALSINDEX, name);
}

void InitializeEnvironment(lua_State* L)
{
    if (g_InitializedState == L)
    {
        return;
    }
    push_global(L, "getrawmetatable", getrawmetatable);
    g_InitializedState = L;
	return;
}