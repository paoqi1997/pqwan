#ifndef HELPER_H
#define HELPER_H

#include <functional>

#include <GLFW/glfw3.h>

// 窗口大小变化时回调
void frameBufferSizeCb(GLFWwindow* window, int width, int height);

class GLFWHelper
{
public:
    GLFWHelper();
    ~GLFWHelper();
    int initWindow(int width = 800, int height = 600, const char *title = "Hello OpenGL");
    void show(const std::function<void()>& func);
public:
    int width;
    int height;
private:
    GLFWwindow *window;
};

class ShaderHelper
{
public:
    bool init(const char *vertexShaderFilePath = "shader.vert", const char *fragmentShaderFilePath = "shader.frag");
    void use();
    int getShaderProgram() { return shaderProgram; }
private:
    int loadVertexShader(const char *vertexShaderFilePath);
    int loadFragmentShader(const char *fragmentShaderFilePath);
    bool linkProgram(int vertexShader, int fragmentShader);
private:
    int shaderProgram;
};

#endif // HELPER_H
