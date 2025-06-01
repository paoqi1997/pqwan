#include <iostream>

#include <glad/glad.h>

#include "helper.h"
#include "util.h"

void frameBufferSizeFn(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWHelper::GLFWHelper()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWHelper::~GLFWHelper()
{
    glfwTerminate();
}

int GLFWHelper::initWindow(int width, int height, const char *title)
{
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "error: glfwCreateWindow()" << std::endl;
        glfwTerminate();
        return 1;
    }

    this->width = width;
    this->height = height;

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeFn);

    return 0;
}

void GLFWHelper::show(const std::function<void()>& func)
{
    while (!glfwWindowShouldClose(window)) {
        // 处理输入
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        func();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

bool ShaderHelper::init(const char *vertexShaderFilePath, const char *fragmentShaderFilePath)
{
    int vertexShader = loadVertexShader(vertexShaderFilePath);
    int fragmentShader = loadFragmentShader(fragmentShaderFilePath);
    return linkProgram(vertexShader, fragmentShader);
}

void ShaderHelper::use()
{
    glUseProgram(shaderProgram);
}

int ShaderHelper::loadVertexShader(const char *vertexShaderFilePath)
{
    pqwan::File vertexShaderFile(vertexShaderFilePath);
    const char *shaderSource = vertexShaderFile.getFileContent();

    // 顶点着色器
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shaderSource, nullptr);
    glCompileShader(vertexShader);

    int succ;
    constexpr int bufSize = 1024;
    char infoLog[bufSize];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        glGetShaderInfoLog(vertexShader, bufSize, nullptr, infoLog);
        std::cout << "error: SHADER::VERTEX::COMPILATION\n" << infoLog << std::endl;
        return -1;
    }

    return vertexShader;
}

int ShaderHelper::loadFragmentShader(const char *fragmentShaderFilePath)
{
    pqwan::File fragmentShaderFile(fragmentShaderFilePath);
    const char *shaderSource = fragmentShaderFile.getFileContent();

    // 片元着色器
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shaderSource, nullptr);
    glCompileShader(fragmentShader);

    int succ;
    constexpr int bufSize = 1024;
    char infoLog[bufSize];

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        glGetShaderInfoLog(fragmentShader, bufSize, nullptr, infoLog);
        std::cout << "error: SHADER::FRAGMENT::COMPILATION\n" << infoLog << std::endl;
        return -1;
    }

    return fragmentShader;
}

bool ShaderHelper::linkProgram(int vertexShader, int fragmentShader)
{
    if (vertexShader == -1 || fragmentShader == -1) {
        return false;
    }

    // 着色器程序
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int succ;
    constexpr int bufSize = 1024;
    char infoLog[bufSize];

    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &succ);
    if (!succ) {
        glGetShaderInfoLog(shaderProgram, bufSize, nullptr, infoLog);
        std::cout << "error: SHADER::PROGRAM::LINKING\n" << infoLog << std::endl;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}
