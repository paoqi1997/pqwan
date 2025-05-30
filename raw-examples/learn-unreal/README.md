# Learn Unreal

学习 Unreal。

## 工程实践

### TPs

[虚幻引擎随笔：意外的重新编译或生成引擎源码](https://www.bilibili.com/opus/552142557286366282)

[定制UE4引擎，如何避免C++项目重编引擎](https://www.bilibili.com/opus/464162093554185164)

[定制UE引擎，如何避免C++项目重编引擎](https://zhuanlan.zhihu.com/p/593960535)

[《UE4开发笔记》Tip 1 编译完全指南](https://zhuanlan.zhihu.com/p/509308558)

## 编译 UE4 源码

### 下载源码及依赖

下载并解压 UnrealEngine-4.27.2-release.zip。

下载 Commit.gitdeps.xml 文件并替换掉 UnrealEngine-4.27.2-release/Engine/Build 目录的同名文件。

执行以下命令：

```
PS E:\UnrealEngine-4.27.2-release> .\Setup.bat
```

### 生成 UE4.sln 文件

通过 Visual Studio Installer 为 Visual Studio 2022 安装 .NET Framework 4.6.2 目标包。

执行以下命令：

```
PS E:\UnrealEngine-4.27.2-release> .\GenerateProjectFiles.bat
```

### 编译

在[此处](https://www.nuget.org/packages/Microsoft.NETFramework.ReferenceAssemblies.net45)下载 microsoft.netframework.referenceassemblies.net45.1.0.3.nupkg 并将后缀改为 zip。

将其解压后把 build/.NETFramework/v4.5 目录复制到 `C:\Program Files (x86)\Reference Assemblies\Microsoft\Framework\.NETFramework` 目录中。

打开 UE4.sln。

在 Engine\Plugins\Runtime\Steam\SteamVR\Source\SteamVRInputDevice\Private\SteamVRInputDeviceFunctionLibrary.cpp 文件添加以下内容：

```cpp
#pragma warning(disable:4834)
```

在 Engine\Plugins\Runtime\AR\Microsoft\HoloLensAR\Source\HoloLensTargetPlatform\HoloLensTargetPlatform.Build.cs 文件添加以下内容：

```cs
bEnableUndefinedIdentifierWarnings = false;
```

将解决方案配置修改为 Development Editor，解决方案平台修改为 Win64，编译 UE4.vcxproj。

编译完成后运行 UnrealEngine-4.27.2-release/Engine/Binaries/Win64/UE4Editor.exe。

### TPs

[UE4(Unreal Engine 4)运行setup.bat发生403报错的问题](https://blog.csdn.net/weixin_42173136/article/details/130608031)

[Open .net framework 4.5 project in VS 2022. Is there any workaround?](https://stackoverflow.com/questions/70022194/open-net-framework-4-5-project-in-vs-2022-is-there-any-workaround)

[UE4源码编译报错C4834 放弃具有 “nodiscard“ 属性的函数的返回值](https://blog.csdn.net/qq_59095655/article/details/132896929)

[UE5源码编译问题———报错 C4834 放弃具有 “nodiscard“ 属性的函数的返回值](https://blog.csdn.net/weixin_43945471/article/details/135490948)

[UE4源码编译报错](https://blog.csdn.net/qq_39049179/article/details/123736101)

[UE4.27 Compile Engine Editor Error:C4668](https://zhuanlan.zhihu.com/p/586056293)

[Can’t build Unreal Editor 4.27.2 from source](https://forums.unrealengine.com/t/cant-build-unreal-editor-4-27-2-from-source/654850)
