# C++ with JavaScript

## V8

### d8

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

PS C:\Users\paoqi\workspace\v8> cd v8
PS C:\Users\paoqi\workspace\v8\v8> gclient sync
```

编译。

```
PS C:\Users\paoqi\workspace\v8\v8> $env:vs2022_install="E:\Microsoft Visual Studio\2022\Community"

PS C:\Users\paoqi\workspace\v8\v8> py -3 tools/dev/gm.py x64.debug
```

运行 d8.exe。

```
PS C:\Users\paoqi\workspace\v8\v8> cd out/x64.debug

PS C:\Users\paoqi\workspace\v8\v8\out\x64.debug> ./d8
```

### lib

确实是可以尝试自己编译库，不过既然有现成的还是直接用现成的吧……

现成的由 [puerts](https://puerts.github.io/docs/puerts/unreal/install) 提供。

### TPs

[Building V8 from source](https://v8.dev/docs/build)

[从源代码构建 V8](https://v8.node.org.cn/docs/build)

[Checking out and Building Chromium for Windows](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/windows_build_instructions.md)

[在windows下使用vs2022编译v8引擎的稳定版本(2023.7.22)](https://www.cnblogs.com/xy0797/p/17574354.html)

[windows平台vs2019编译v8小记](https://www.cnblogs.com/PeterZ1997/p/18131427)

[Windows 编译 V8 静态库踩坑记录](https://toyobayashi.github.io/2020/07/04/V8/)

[Could not build v8 on windows with VS2019 #624](https://github.com/v8/v8.dev/issues/624)

#### 链接错误

[VS2017链接V8引擎库遇到的一些问题](https://blog.csdn.net/weixin_43939128/article/details/104678515)

[windos vs2019 || clion cmake clang msvc 编译v8 引擎](https://www.cnblogs.com/cfas/p/15657434.html)

[Building Chrome V8 on Windows](https://gist.github.com/jhalon/5cbaab99dccadbf8e783921358020159)

#### MSVC

[/Zc:__cplusplus（启用更新的 __cplusplus 宏）](https://learn.microsoft.com/zh-cn/cpp/build/reference/zc-cplusplus?view=msvc-170)
