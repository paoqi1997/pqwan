# C++ with JavaScript

## Build V8

获取 depot_tools 工具。

```
PS C:\Users\paoqi> mkdir workspace
PS C:\Users\paoqi> cd workspace
PS C:\Users\paoqi\workspace> git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
```

为 depot_tools 添加好环境变量后更新 depot_tools 工具。

```
PS C:\Users\paoqi\workspace> gclient
```

添加环境变量。

```
# No downloadable toolchain found. In order to use your locally installed version of Visual Studio to build Chrome please set DEPOT_TOOLS_WIN_TOOLCHAIN=0.
PS C:\Users\paoqi\workspace> $env:DEPOT_TOOLS_WIN_TOOLCHAIN=0
```

获取 V8 源码。

```
PS C:\Users\paoqi\workspace> mkdir v8
PS C:\Users\paoqi\workspace> cd v8
PS C:\Users\paoqi\workspace\v8> fetch v8

PS C:\Users\paoqi\workspace> cd v8
PS C:\Users\paoqi\workspace\v8\v8> gclient sync
```

编译。

```
PS C:\Users\paoqi\workspace\v8\v8> $env:vs2022_install="E:\Microsoft Visual Studio\2022\Community"

PS C:\Users\paoqi\workspace\v8\v8> py -3 tools/dev/gm.py x64.debug
```

### TPs

[Building V8 from source](https://v8.dev/docs/build)

[Checking out and Building Chromium for Windows](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/windows_build_instructions.md)
