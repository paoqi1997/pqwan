-- https://xmake.io/#/zh-cn/manual/project_target

local LUA_SRC = "../lua-5.4.7/src/*.c"
local LUA_C   = "../lua-5.4.7/src/lua.c"
local LUAC_C  = "../lua-5.4.7/src/luac.c"

target("lua54.lib")
    set_kind("static")
    set_basename("lua54")
    set_symbols("debug")
    add_files(LUA_SRC)
    remove_files(LUA_C)
    remove_files(LUAC_C)

target("lua54.dll")
    set_kind("shared")
    set_basename("lua54")
    set_symbols("debug")
    add_files(LUA_SRC)
    remove_files(LUA_C)
    remove_files(LUAC_C)
    if is_plat("windows") then
        add_defines("LUA_BUILD_AS_DLL")
    end

target("lua54.exe")
    set_kind("binary")
    set_basename("lua54")
    set_symbols("debug")
    add_files(LUA_C)
    add_deps("lua54.dll")

target("luac54.exe")
    set_kind("binary")
    set_basename("luac54")
    set_symbols("debug")
    add_files(LUA_SRC)
    remove_files(LUA_C)
