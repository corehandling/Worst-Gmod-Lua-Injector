#pragma once

#include <windows.h>
#include <cstdint>

#define LUA_GLOBALSINDEX (-10002)

typedef struct lua_State lua_State;
typedef int (*lua_CFunction)(lua_State* L);

typedef int(__cdecl* luaL_loadstring_t)(lua_State* L, const char* s);
extern luaL_loadstring_t luaL_loadstring;

typedef void(__cdecl* luaL_checkany_t)(lua_State* L, int arg);
extern luaL_checkany_t luaL_checkany;

typedef void(__cdecl* lua_pushboolean_t)(lua_State* L, int b);
extern lua_pushboolean_t lua_pushboolean;

typedef int(__cdecl* lua_setmetatable_t)(lua_State* L, int index);
extern lua_setmetatable_t lua_setmetatable;

typedef void(__cdecl* lua_setfield_t)(lua_State* L, int index, const char* k);
extern lua_setfield_t lua_setfield;

typedef void(__cdecl* lua_pushcclosure_t)(lua_State* L, lua_CFunction fn, int n);
extern lua_pushcclosure_t lua_pushcclosure;

typedef void(__cdecl* lua_pushnil_t)(lua_State* L);
extern lua_pushnil_t lua_pushnil;

typedef int(__cdecl* lua_getmetatable_t)(lua_State* L, int index);
extern lua_getmetatable_t lua_getmetatable;

typedef int(__cdecl* lua_pcall_t)(lua_State* L, int nargs, int nresults, int errfunc);
extern lua_pcall_t lua_pcall;

typedef void(__cdecl* lua_settop_t)(lua_State* L, int index);
extern lua_settop_t lua_settop;

typedef void(__cdecl* lua_getfield_t)(lua_State* L, int index, const char* k);
extern lua_getfield_t lua_getfield;

typedef int(__cdecl* lua_toboolean_t)(lua_State* L, int index);
extern lua_toboolean_t lua_toboolean;

bool LoadLuaFunctions();