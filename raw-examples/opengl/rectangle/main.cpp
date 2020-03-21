#include <cmath>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define BUFSIZE 1024

using namespace std;

// 窗口大小变化时回调
void frameBufferSizeCB(GLFWwindow *window, int width, int height);
// 处理输入
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 440 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 440 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Hello OpenGL", nullptr, nullptr);
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

    int flag;
    char buf[BUFSIZE];

    // 顶点着色器
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &flag);
    if (!flag) {
        glGetShaderInfoLog(vertexShader, BUFSIZE, nullptr, buf);
        cout << "error: SHADER::VERTEX::COMPILATION" << endl;
    }

    // 片段着色器
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &flag);
    if (!flag) {
        glGetShaderInfoLog(fragmentShader, BUFSIZE, nullptr, buf);
        cout << "error: SHADER::FRAGMENT::COMPILATION" << endl;
    }

    // 着色器程序
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &flag);
    if (!flag) {
        glGetProgramInfoLog(shaderProgram, BUFSIZE, nullptr, buf);
        cout << "error: SHADER::PROGRAM::LINKING" << endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 定义4个顶点，其中2个顶点用了2次
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // 左下
        -0.5f,  0.5f, 0.0f, // 左上
         0.5f,  0.5f, 0.0f, // 右上
         0.5f, -0.5f, 0.0f  // 右下
    };

    /**
     * 1 + 2
     * + + +
     * 0 + 3
     */
    unsigned int indices[] = {
        0, 1, 3, // 第一个三角形
        2, 1, 3  // 第二个三角形
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

    // 使用索引缓冲对象将索引数据拷贝至缓冲中
    unsigned int eleBufferObj;
    glGenBuffers(1, &eleBufferObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufferObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /**
     * 设置顶点属性
     * 0: 位置值为0，vertexShader中的location = 0
     * 3: 顶点属性是一个vec3，由3个值组成
     * GL_FLOAT: 数据类型为float
     * 3 * sizeof(float): 连续的顶点属性组之间的间隔
     * (void*)0: 位置数据在缓冲中相对起始位置的偏移量
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vArrayObj);
        // 绘制6个顶点
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vArrayObj);
    glDeleteBuffers(1, &vBufferObj);
    glDeleteBuffers(1, &eleBufferObj);

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
