#include <cassert>
#include <cstdio>
#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "util.h"

using namespace pqwan;

File::File(const char *sFileName, std::ios_base::openmode mode)
    : is(sFileName, mode)
{
    if (!is.is_open()) {
        std::printf("Failed to open %s.\n", sFileName);
    } else {
        is.seekg(0, std::ios::end);
        std::size_t iLength = is.tellg();
        is.seekg(0, std::ios::beg);
        std::vector<char> pool(iLength);
        is.read(pool.data(), pool.size());
        fileContent.assign(pool.begin(), pool.end());
        assert(pool.size() == fileContent.size());
    }
}

File::~File()
{
    if (is.is_open()) {
        is.close();
    }
}

void pqwan::frameBufferSizeCb(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void pqwan::cursorPosCb(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    Camera *camera = Camera::getInstance();

    if (camera->FirstMouse) {
        camera->LastX = xpos;
        camera->LastY = ypos;
        camera->FirstMouse = false;
    }

    float xoffset = xpos - camera->LastX;

    // OpenGL 和 GLFW 的坐标系在y轴方向上正好相反
    // OpenGL y轴向上为正方向，GLFW y轴向下为正方向
    float yoffset = camera->LastY - ypos;

    camera->LastX = xpos;
    camera->LastY = ypos;

    camera->handleMouseMovement(xoffset, yoffset);
}

void pqwan::scrollCb(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera::getInstance()->handleMouseScroll(static_cast<float>(yoffset));
}

GLFWHelper::GLFWHelper() : hasCamera(false), lastFrameTime(0.0f)
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
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCb);

    return 0;
}

void GLFWHelper::workWithCamera()
{
    hasCamera = true;

    // 鼠标光标起始位置取屏幕/窗口正中央
    Camera::getInstance()->LastX = width / 2.0f;
    Camera::getInstance()->LastY = height / 2.0f;

    glfwSetCursorPosCallback(window, cursorPosCb);
    glfwSetScrollCallback(window, scrollCb);

    // 隐藏鼠标光标并将其留在窗口内
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void GLFWHelper::show(const std::function<void()>& func)
{
    while (!glfwWindowShouldClose(window)) {
        // 处理输入
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        if (hasCamera) {
            float currFrameTime = static_cast<float>(glfwGetTime());
            float deltaTime = currFrameTime - lastFrameTime;
            lastFrameTime = currFrameTime;

            Camera *camera = Camera::getInstance();

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                camera->handleKeyboard(Camera::FORWARD, deltaTime);
            } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                camera->handleKeyboard(Camera::BACKWARD, deltaTime);
            } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                camera->handleKeyboard(Camera::LEFT, deltaTime);
            } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                camera->handleKeyboard(Camera::RIGHT, deltaTime);
            }
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

Camera *Camera::instance = nullptr;

Camera::Camera() :
    FirstMouse(true), LastX(0.0f), LastY(0.0f), Zoom(45.0f),
    position(0.0f, 0.0f, 3.0f), up(0.0f, 1.0f, 0.0f), front(0.0f, 0.0f, -1.0f),
    yaw(-90.0f), pitch(0.0f), movementSpeed(2.5f), mouseSensitivity(0.1f)
{
    worldUp = up;
    onUpdateVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    // center 为相机看向的位置，故传入 position + front
    return glm::lookAt(position, position + front, up);
}

void Camera::handleKeyboard(Direction direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;

    switch (direction)
    {
    case FORWARD:
        position += front * velocity;
        break;
    case BACKWARD:
        position -= front * velocity;
        break;
    case LEFT:
        position -= right * velocity;
        break;
    case RIGHT:
        position += right * velocity;
        break;
    default:
        break;
    }
}

void Camera::handleMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // 往上下方向看的视角受限
    if (constrainPitch) {
        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }
    }

    onUpdateVectors();
}

void Camera::handleMouseScroll(float yoffset)
{
    Zoom -= yoffset;

    if (Zoom < 1.0f) {
        Zoom = 1.0f;
    }
    if (Zoom > 45.0f) {
        Zoom = 45.0f;
    }
}

void Camera::onUpdateVectors()
{
    glm::vec3 currFront;

    // yaw 的余弦值参与x分量的计算
    currFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    // pitch 的正弦值就是y分量，应注意其余弦值会影响水平分量
    currFront.y = sin(glm::radians(pitch));
    // yaw 的正弦值参与z分量的计算
    currFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    // 均需要归一化处理
    front = glm::normalize(currFront);
    // 对<相机正前方>和<世界正上方>做叉乘运算得到<相机正右方>
    right = glm::normalize(glm::cross(front, worldUp));
    // 对<相机正右方>和<相机正前方>做叉乘运算得到<相机正上方>
    up = glm::normalize(glm::cross(right, front));
}
