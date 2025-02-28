-- https://xmake.io/#/zh-cn/manual/project_target
target("app")
    set_kind("binary")
    set_symbols("debug")
    add_files("*.cpp")
    add_includedirs("../lua54")
    add_includedirs("../lua-5.4.7/src")
    add_links("lua54")
    if is_plat("windows") then
        local rel_path = "../xbuild-lua/windows/x64/debug"
        local abs_path = path.absolute(rel_path)
        add_linkdirs(abs_path)
    elseif is_plat("linux") then
        local rel_path = "../xbuild-lua/linux/x86_64/debug"
        local abs_path = path.absolute(rel_path)
        add_linkdirs(abs_path)
        add_rpathdirs(abs_path)
    end
