# Learn OpenGL

学习 OpenGL。

## 目录结构

详情如下所示：

```
-- 3rd-party
   |
   |-- include
       |
       |-- glad
       |-- GLFW
       |-- ...
   |-- lib
-- assets
-- introduction  # 入门部分
   |
   |-- rectangle # 矩形
   |-- texture   # 纹理
   |-- ...
-- util
-- README.md
```

## 部署本地文档

获得 [LearnOpenGL-CN](https://github.com/LearnOpenGL-CN/LearnOpenGL-CN) 的 repo 后执行以下命令：

```
# 部署前需要安装 mkdocs
>pip install -i https://pypi.tuna.tsinghua.edu.cn/simple mkdocs

# 在 LearnOpenGL-CN-new-theme 目录执行
>python setup.py install
>mkdocs build
```

## Configure 3rd party libraries for OpenGL

以下内容提炼自 LearnOpenGL CN 入门部分的 [创建窗口](https://learnopengl-cn.github.io/01%20Getting%20started/02%20Creating%20a%20window/) 章节，
原版英文教程的对应章节在[这里](https://learnopengl.com/Getting-started/Creating-a-window)。

### [GLFW](https://www.glfw.org)

通过CMake构建GLFW。

### [GLAD](https://glad.dav1d.de)

API.gl选择4.4，Profile选择Core，点击Generate按钮来生成库文件。

和GLFW一样，请通过CMake构建GLAD。

拿到 glad.zip 后对其进行解压，在 glad 文件夹下创建 CMakeLists.txt 文件并添加以下内容：

```
cmake_minimum_required (VERSION 3.4)
project (glad)

include_directories (./include)

file (GLOB SRCS ./src/*.c)
file (GLOB INC_GLAD ./include/glad/*.h)
file (GLOB INC_KHR ./include/KHR/*.h)

source_group ("src" FILES ${SRCS})
source_group ("include\\glad" FILES ${INC_GLAD})
source_group ("include\\KHR" FILES ${INC_KHR})

if (WIN32)
    add_library (glad STATIC ${SRCS} ${INC_GLAD} ${INC_KHR})
endif ()
```

### [stb](https://github.com/nothings/stb)

直接包含该头文件即可。

```
>curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
```

### [GLM](https://github.com/g-truc/glm)

直接包含所需的头文件即可。

```
>curl -O https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip
```

## Tips

一些有用的小技巧。

### 命令行编译sln工程

如果安装的是Visual Studio 2019，并且是默认安装，那么请给以下路径设置环境变量：

```
C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE
```

通过CMake建立sln工程。

```
# 32位工程
>cmake -S . -B build -G "Visual Studio 16 2019" -A Win32
# 64位工程
>cmake -S . -B build -G "Visual Studio 16 2019" -A x64
```

编译sln工程。

```
>devenv xxx.sln /build Debug
```
