# Configure 3rd party libraries for OpenGL

看不懂以下内容可以参考这篇[教程](https://learnopengl-cn.github.io/01%20Getting%20started/01%20OpenGL/)。

## [GLFW](https://www.glfw.org)

通过CMake构建GLFW。

## [GLAD](https://glad.dav1d.de)

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
