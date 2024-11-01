-- https://xmake.io/#/zh-cn/manual/project_target
target("app")
    set_kind("binary")
    set_symbols("debug")
    add_files("*.cpp")
    add_includedirs("../lua54")
    add_includedirs("../lua-5.4.7/src")
    add_links("lua54")
    if is_plat("windows") then
        add_linkdirs("../xbuild-lua/windows/x64/debug")
    end
