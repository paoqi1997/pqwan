#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

    /**
     *     1
     *   +   +
     * 0 + + + 2
     */
    float vertices[] = {
        // positions        // colors
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 0
         0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 1
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // 2
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
     * 6 * sizeof(float): 连续的顶点属性组之间的间隔
     * (void*)0: 位置数据在缓冲中相对起始位置的偏移量
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shaderProgram);

    // 渲染循环
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vArrayObj);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
