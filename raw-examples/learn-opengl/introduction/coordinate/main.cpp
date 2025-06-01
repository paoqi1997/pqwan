#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <util.h>

#define BUFSIZE 1024

using namespace std;

// 窗口大小变化时回调
void frameBufferSizeCB(GLFWwindow *window, int width, int height);
// 处理输入
void processInput(GLFWwindow *window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;

    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Hello OpenGL", nullptr, nullptr);
    if (window == nullptr) {
        cout << "error: glfwCreateWindow()" << endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCB);

    // 加载OpenGL函数指针
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        cout << "error: gladLoadGLLoader()" << endl;
        return 1;
    }

    // xxxxxx
    glEnable(GL_DEPTH_TEST);

    int flag;
    char infoLog[BUFSIZE];
    const char *shaderSource;

    // 顶点着色器
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    pqwan::File oVertFile("shader.vert");
    shaderSource = oVertFile.getFileContent();
    glShaderSource(vertexShader, 1, &shaderSource, nullptr);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &flag);
    if (!flag) {
        glGetShaderInfoLog(vertexShader, BUFSIZE, nullptr, infoLog);
        cout << "error: SHADER::VERTEX::COMPILATION\n" << infoLog << endl;
    }

    // 片元着色器
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    pqwan::File oFragFile("shader.frag");
    shaderSource = oFragFile.getFileContent();
    glShaderSource(fragmentShader, 1, &shaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &flag);
    if (!flag) {
        glGetShaderInfoLog(fragmentShader, BUFSIZE, nullptr, infoLog);
        cout << "error: SHADER::FRAGMENT::COMPILATION\n" << infoLog << endl;
    }

    // 着色器程序
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &flag);
    if (!flag) {
        glGetProgramInfoLog(shaderProgram, BUFSIZE, nullptr, infoLog);
        cout << "error: SHADER::PROGRAM::LINKING\n" << infoLog << endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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
     * 8 * sizeof(float): 连续的顶点属性组之间的间隔
     * (void*)0: 位置数据在缓冲中相对起始位置的偏移量
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

    // OpenGL的(0,0)坐标位于左下角，而图像的(0,0)坐标通常位于左上角，故图像需要垂直翻转
    stbi_set_flip_vertically_on_load(true);

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

    glUseProgram(shaderProgram);
    // 告诉OpenGL哪个采样器对应哪个纹理单元
    glUniform1i(glGetUniformLocation(shaderProgram, "sampler1"), 1);
    glUniform1i(glGetUniformLocation(shaderProgram, "sampler2"), 2);

    // 渲染循环
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glm::mat4 model(1.0f);
        glm::mat4 view(1.0f);
        glm::mat4 projection(1.0f);

        model = glm::rotate(model, float(glfwGetTime()) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), float(screenWidth) / float(screenHeight), 0.1f, 100.0f);

        glUseProgram(shaderProgram);

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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vArrayObj);
    glDeleteBuffers(1, &vBufferObj);

    glfwTerminate();

    return 0;
}

void frameBufferSizeCB(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
