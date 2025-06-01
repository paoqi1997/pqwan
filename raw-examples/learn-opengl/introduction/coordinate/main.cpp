#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <util.h>

#include "helper.h"

using std::cout;
using std::endl;

int main()
{
    GLFWHelper glfwHelper;

    if (glfwHelper.initWindow() != 0) {
        cout << "error: GLFWHelper::initWindow()" << endl;
        return 1;
    }

    int screenWidth = glfwHelper.width;
    int screenHeight = glfwHelper.height;

    // 加载OpenGL函数指针
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        cout << "error: gladLoadGLLoader()" << endl;
        return 2;
    }

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    ShaderHelper shaderHelper;

    if (!shaderHelper.init()) {
        cout << "error: ShaderHelper::init()" << endl;
        return 3;
    }

    int shaderProgram = shaderHelper.getShaderProgram();

    // 定义36个顶点
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
    };

    // 使用一个顶点缓冲对象将顶点数据拷贝至缓冲中
    unsigned int vBufferObj;
    glGenBuffers(1, &vBufferObj);
    glBindBuffer(GL_ARRAY_BUFFER, vBufferObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /**
     * 一个顶点数组对象，它储存以下内容：
     * 1: glEnableVertexAttribArray和glDisableVertexAttribArray的调用
     * 2: 通过glVertexAttribPointer设置的顶点属性
     * 3: 通过glVertexAttribPointer调用与顶点属性相关联的顶点缓冲对象
     */
    unsigned int vArrayObj;
    glGenVertexArrays(1, &vArrayObj);
    glBindVertexArray(vArrayObj);

    /**
     * 设置顶点属性
     * 0: 位置值为0，vertexShader中的location = 0
     * 3: 顶点属性是一个vec3，由3个值组成
     * GL_FLOAT: 数据类型为float
     * GL_FALSE: 是否希望数据被标准化
     * 5 * sizeof(float): 连续的顶点属性组之间的间隔
     * (void*)0: 位置数据在缓冲中相对起始位置的偏移量
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // OpenGL的(0,0)坐标位于左下角，而图像的(0,0)坐标通常位于左上角，故图像需要垂直翻转
    stbi_set_flip_vertically_on_load(true);

    int iWidth, iHeight, nChannels;
    unsigned char *data;

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // 设置Wrap相关的参数，即指定纹理坐标超出范围时的处理方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置Filter相关的参数，即指定纹理坐标映射至数组中的处理方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("container.jpg", &iWidth, &iHeight, &nChannels, 0);
    if (data) {
        /**
         * GL_TEXTURE_2D: 目标纹理
         * 0: 详细级别编号，级别n表示多级渐远纹理/缩小贴图的级别，其中0表示基本图像级别...Level n is the nth mipmap reduction image.
         * GL_RGB: 指定纹理中的颜色组件，或者说以何种格式储存纹理
         * iWidth: 纹理宽度
         * iHeight: 纹理高度
         * 0: 必须为0
         * GL_RGB: 指定像素数据的格式
         * GL_UNSIGNED_BYTE: 指定像素数据的数据类型
         * data: 数据
         */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "error: stbi_load()" << endl;
    }

    stbi_image_free(data);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("awesomeface.png", &iWidth, &iHeight, &nChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "error: stbi_load()" << endl;
    }

    stbi_image_free(data);

    shaderHelper.use();

    // 告诉OpenGL哪个采样器对应哪个纹理单元
    glUniform1i(glGetUniformLocation(shaderProgram, "sampler1"), 1);
    glUniform1i(glGetUniformLocation(shaderProgram, "sampler2"), 2);

    auto loop = [&](){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glm::mat4 model(1.0f);
        glm::mat4 view(1.0f);
        glm::mat4 projection(1.0f);

        // 从局部空间变换到世界空间
        // 1. 模型会绕轴(0.5, 1.0)随时间旋转
        // 2. 每秒旋转50°
        model = glm::rotate(model, float(glfwGetTime()) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        // 将世界坐标变换到观察空间
        // 整体向后平移3个单位
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        // 从观察空间变换到裁剪空间
        // 创建一个透视投影矩阵
        // 1. FOV表示视野宽度
        // 2. 宽高比用于防止图像被拉伸变形
        // 3. near用于剔除掉离相机太近的物体
        // 4. far用于剔除掉离相机太远的物体
        projection = glm::perspective(glm::radians(45.0f), float(screenWidth) / float(screenHeight), 0.1f, 100.0f);

        shaderHelper.use();

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        // 将变换矩阵传递给着色器
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(vArrayObj);
        // 绘制36个顶点
        glDrawArrays(GL_TRIANGLES, 0, 36);
    };

    glfwHelper.show(loop);

    glDeleteVertexArrays(1, &vArrayObj);
    glDeleteBuffers(1, &vBufferObj);

    return 0;
}
