#include "luai.h"
#include "stopwatch.h"

LuaEnv *LuaEnv::instance = nullptr;

LuaEnv::LuaEnv()
{
    L = luaL_newstate();
    luaL_openlibs(L);
}

LuaEnv::~LuaEnv()
{
    lua_close(L);
}

bool LuaEnv::run()
{
    addFunctions();
    addModules();
    return runScript();
}

int L_F_AddClock(lua_State *L)
{
    uint interval = lua_tonumber(L, 1);
    const char *funcName = lua_tostring(L, 2);
    Stopwatch::getInstance()->addClock(interval, funcName);
    return 0;
}

int L_F_Now(lua_State *L)
{
    std::uint64_t nowTime = nowSec();
    lua_pushnumber(L, nowTime);
    return 1;
}

int L_F_GetPassedTime(lua_State *L)
{
    std::uint64_t passedTime = Stopwatch::getInstance()->getPassedTime();
    std::uint64_t passedTimeSec = passedTime / 1000;
    lua_pushnumber(L, passedTimeSec);
    return 1;
}

int L_M_Stopwatch(lua_State *L)
{
    luaL_Reg funcList[] = {
        {"addClock", L_F_AddClock},
        {"getPassedTime", L_F_GetPassedTime},
        {nullptr, nullptr}
    };

    luaL_newlib(L, funcList);

    return 1;
}

void LuaEnv::addFunctions()
{
    lua_register(L, "addClock", L_F_AddClock);
    lua_register(L, "now", L_F_Now);
}

void LuaEnv::addModules()
{
    luaL_Reg mods[] = {
        {"stopwatch", L_M_Stopwatch},
        {nullptr, nullptr}
    };

    for (luaL_Reg *mod = mods; mod->func != nullptr; ++mod) {
        luaL_requiref(L, mod->name, mod->func, 1);
        lua_settop(L, 0);
    }
}

bool LuaEnv::runScript()
{
    return luaL_dofile(L, "main.lua") == LUA_OK;
}

void Stopwatch::onClock(Clock *clock)
{
    lua_State *L = LuaEnv::getInstance()->getL();
    const char *funcName = clock->funcName.c_str();

    lua_getglobal(L, funcName);

    if (lua_isfunction(L, -1)) {
        lua_pcall(L, 0, 0, 0);
    }
}
