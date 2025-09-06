#include "LuaFunctions.h"
#include <cstdarg>

HMODULE hLua = nullptr;

lua_pcall_t lua_pcall = nullptr;
luaL_checkany_t luaL_checkany = nullptr;
lua_pushcclosure_t lua_pushcclosure = nullptr;
luaL_loadstring_t luaL_loadstring = nullptr;
lua_pushboolean_t lua_pushboolean = nullptr;
lua_setmetatable_t lua_setmetatable = nullptr;
lua_setfield_t lua_setfield = nullptr;
lua_pushnil_t lua_pushnil = nullptr;
lua_getmetatable_t lua_getmetatable = nullptr;
lua_settop_t lua_settop = nullptr;
lua_getfield_t lua_getfield = nullptr;
lua_toboolean_t lua_toboolean = nullptr;

bool LoadLuaFunctions()
{
    HMODULE hLua = GetModuleHandleA("lua_shared.dll");
    if (!hLua)
    {
        return false;
    }
    lua_pcall = (lua_pcall_t)GetProcAddress(hLua, "lua_pcall");
    luaL_checkany = (luaL_checkany_t)GetProcAddress(hLua, "luaL_checkany");
    luaL_loadstring = (luaL_loadstring_t)GetProcAddress(hLua, "luaL_loadstring");
    lua_pushboolean = (lua_pushboolean_t)GetProcAddress(hLua, "lua_pushboolean");
    lua_setmetatable = (lua_setmetatable_t)GetProcAddress(hLua, "lua_setmetatable");
    lua_setfield = (lua_setfield_t)GetProcAddress(hLua, "lua_setfield");
    lua_pushcclosure = (lua_pushcclosure_t)GetProcAddress(hLua, "lua_pushcclosure");
    lua_pushnil = (lua_pushnil_t)GetProcAddress(hLua, "lua_pushnil");
    lua_getmetatable = (lua_getmetatable_t)GetProcAddress(hLua, "lua_getmetatable");
    lua_settop = (lua_settop_t)GetProcAddress(hLua, "lua_settop");
    lua_getfield = (lua_getfield_t)GetProcAddress(hLua, "lua_getfield");
    lua_toboolean = (lua_toboolean_t)GetProcAddress(hLua, "lua_toboolean");
    return true;
}