#ifndef LUA_I_H
#define LUA_I_H

#include <luacore.h>

class LuaEnv
{
public:
    static LuaEnv* getInstance() {
        if (instance == nullptr) {
            instance = new LuaEnv();
        }
        return instance;
    }
    LuaEnv(const LuaEnv&) = delete;
    LuaEnv& operator = (const LuaEnv&) = delete;
    bool run();
    lua_State* getL() const { return L; }
private:
    LuaEnv();
    ~LuaEnv();
    void addFunctions();
    void addModules();
    bool runScript();
    static LuaEnv *instance;
    lua_State *L;
};

#endif // LUA_I_H
