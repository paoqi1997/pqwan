-- https://xmake.io/#/zh-cn/manual/project_target

target("app")
    set_kind("binary")
    set_symbols("debug")
    add_files("*.cpp")
    add_includedirs("../v8_10.6.194/Inc")

    local mode = "dynamic"

    local is_windows = is_plat("windows")
    local is_static = mode == "static"
    local is_dynamic = mode == "dynamic"
    local is_windows_static = is_windows and is_static
    local is_windows_dynamic = is_windows and is_dynamic

    if is_windows_static then
        add_links("wee8")
        add_links("ADVAPI32")
        add_links("WINMM")

        local rel_lib_dir = "../v8_10.6.194/Lib/Win64"
        local abs_lib_dir = path.absolute(rel_lib_dir)
        add_linkdirs(abs_lib_dir)
    elseif is_windows_dynamic then
        add_links("v8.dll") -- v8.dll.lib

        local rel_lib_dir = "../v8_10.6.194/Lib/Win64DLL"
        local abs_lib_dir = path.absolute(rel_lib_dir)
        add_linkdirs(abs_lib_dir)

        local exe_dir = path.absolute("../xbuild/windows/x64/debug")

        local lib_v8_name = "v8.dll"
        local lib_v8_libbase_name = "v8_libbase.dll"
        local lib_v8_libplatform_name = "v8_libplatform.dll"
        local lib_zlib_name = "zlib.dll"

        local src_lib_v8_path = path.join(abs_lib_dir, lib_v8_name)
        local src_lib_v8_libbase_path = path.join(abs_lib_dir, lib_v8_libbase_name)
        local src_lib_v8_libplatform_path = path.join(abs_lib_dir, lib_v8_libplatform_name)
        local src_lib_zlib_path = path.join(abs_lib_dir, lib_zlib_name)

        local dst_lib_v8_path = path.join(exe_dir, lib_v8_name)
        local dst_lib_v8_libbase_path = path.join(exe_dir, lib_v8_libbase_name)
        local dst_lib_v8_libplatform_path = path.join(exe_dir, lib_v8_libplatform_name)
        local dst_lib_zlib_path = path.join(exe_dir, lib_zlib_name)

        -- 动态链接库要和可执行文件在同一目录下
        after_build(function (target)
            if not os.exists(dst_lib_v8_path) then
                os.cp(src_lib_v8_path, exe_dir)
            end

            if not os.exists(dst_lib_v8_libbase_path) then
                os.cp(src_lib_v8_libbase_path, exe_dir)
            end

            if not os.exists(dst_lib_v8_libplatform_path) then
                os.cp(src_lib_v8_libplatform_path, exe_dir)
            end

            if not os.exists(dst_lib_zlib_path) then
                os.cp(src_lib_zlib_path, exe_dir)
            end
        end)
    end
