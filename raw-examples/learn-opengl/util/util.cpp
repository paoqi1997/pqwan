#include <cassert>

// https://learn.microsoft.com/zh-cn/cpp/c-runtime-library/math-constants?view=msvc-170
#define _USE_MATH_DEFINES
#include <cmath>

#include <cstdio>
#include <iostream>
#include <vector>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

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

void GLFWHelper::show(const std::function<void()>& doFunc)
{
    Camera *camera = Camera::getInstance();

    while (!glfwWindowShouldClose(window)) {
        // 处理输入
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        if (hasCamera) {
            float currFrameTime = static_cast<float>(glfwGetTime());
            float deltaTime = currFrameTime - lastFrameTime;
            lastFrameTime = currFrameTime;

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                camera->handleKeyboard(Camera::Direction::FORWARD, deltaTime);
            } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                camera->handleKeyboard(Camera::Direction::BACKWARD, deltaTime);
            } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                camera->handleKeyboard(Camera::Direction::LEFT, deltaTime);
            } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                camera->handleKeyboard(Camera::Direction::RIGHT, deltaTime);
            }
        }

        doFunc();

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

void ShaderHelper::uniformFloat(const std::string& name, float value)
{
    GLint uniformLocation = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1f(uniformLocation, value);
}

void ShaderHelper::uniformVec3(const std::string& name, float x, float y, float z)
{
    GLint uniformLocation = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform3f(uniformLocation, x, y, z);
}

void ShaderHelper::uniformVec3(const std::string& name, const glm::vec3& vector)
{
    GLint uniformLocation = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform3fv(uniformLocation, 1, glm::value_ptr(vector));
}

void ShaderHelper::uniformMat4(const std::string& name, const glm::mat4& matrix)
{
    GLint uniformLocation = glGetUniformLocation(shaderProgram, name.c_str());
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
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

Camera::Camera() :
    FirstMouse(true), LastX(0.0f), LastY(0.0f), Zoom(45.0f),
    position(0.0f, 0.0f, 3.0f), front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f),
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
    case Direction::FORWARD:
        position += front * velocity;
        break;
    case Direction::BACKWARD:
        position -= front * velocity;
        break;
    case Direction::LEFT:
        position -= right * velocity;
        break;
    case Direction::RIGHT:
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
    currFront.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    // pitch 的正弦值就是y分量，应注意其余弦值会影响水平分量
    currFront.y = std::sin(glm::radians(pitch));
    // yaw 的正弦值参与z分量的计算
    currFront.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));

    // 均需要归一化处理
    front = glm::normalize(currFront);
    // 对<相机正前方>和<世界正上方>做叉乘运算得到<相机正右方>
    right = glm::normalize(glm::cross(front, worldUp));
    // 对<相机正右方>和<相机正前方>做叉乘运算得到<相机正上方>
    up = glm::normalize(glm::cross(right, front));
}

Sphere::Sphere(float radius, int xSegments, int ySegments) : vao(0), vbo(0), ebo(0)
{
    generateVertices(radius, xSegments, ySegments);
    generateIndices(radius, xSegments, ySegments);
}

Sphere::~Sphere()
{
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
    }
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
    }
    if (ebo != 0) {
        glDeleteBuffers(1, &ebo);
    }
}

void Sphere::generateVertices(float radius, int i_xSegments, int i_ySegments)
{
    constexpr float PI = M_PI;

    float xSegments = static_cast<float>(i_xSegments);
    float ySegments = static_cast<float>(i_ySegments);

    float xStep = 2 * PI / xSegments; // [0, 2π]
    float yStep = PI / ySegments;     // [-π/2, π/2]

    for (int y = 0; y <= ySegments; ++y) {
        float ySegment = y / ySegments;
        float yAngle = y * yStep;

        for (int x = 0; x <= xSegments; ++x) {
            float xSegment = x / xSegments;
            float xAngle = x * xStep;

            // 球面坐标公式
            float xPos = radius * std::sin(yAngle) * std::cos(xAngle);
            float yPos = radius * std::cos(yAngle);
            float zPos = radius * std::sin(yAngle) * std::sin(xAngle);

            float xNormal = xPos / radius;
            float yNormal = yPos / radius;
            float zNormal = zPos / radius;

            positions.emplace_back(glm::vec3{ xPos, yPos, zPos });
            normals.emplace_back(glm::vec3{ xNormal, yNormal, zNormal });
            uvs.emplace_back(glm::vec2{ xSegment, ySegment });
        }
    }
}

void Sphere::generateIndices(float radius, int xSegments, int ySegments)
{
    bool isOdd = false;

    // 形成多个三角形条带
    // if xSegments = 3, ySegments = 3
    // 8(11) 9 10
    // 4( 7) 5  6
    // 0( 3) 1  2
    for (int y = 0; y < ySegments; ++y) {
        if (!isOdd) {
            // 0, 4, 1, 5, 2, 6, 3, 7
            // 4(7) 5 6 7
            // 0(3) 1 2 3
            for (int x = 0; x <= xSegments; ++x) {
                // 当前行顶点 + 下一行同列顶点
                indices.push_back(y * (xSegments + 1) + x);
                indices.push_back((y + 1) * (xSegments + 1) + x);
            }
        } else {
            // 不反向
            // 8, 4, 9, 5, 10, 6, 11, 7
            // 8(11) 9 10 11
            // 4( 7) 5  6  7
            // [3, 7, 8] [7, 8, 4]
            // [7, 8, 4]: 顶点4和7的连接关系跨越了整个u轴

            // 反向
            // 11, 7, 10, 6, 9, 5, 8, 4
            // 11(8) 10 9 8
            //  7(4)  6 5 4
            // [3, 7, 11] [7, 11, 7]
            for (int x = xSegments; x >= 0; --x) {
                // 下一行顶点 + 当前行同列顶点
                indices.push_back((y + 1) * (xSegments + 1) + x);
                indices.push_back(y * (xSegments + 1) + x);
            }
        }

        isOdd = !isOdd;
    }
}

void Sphere::loadVertices()
{
    std::vector<float> vertices;

    for (std::size_t i = 0; i < positions.size(); ++i) {
        vertices.push_back(positions[i].x);
        vertices.push_back(positions[i].y);
        vertices.push_back(positions[i].z);

        vertices.push_back(normals[i].x);
        vertices.push_back(normals[i].y);
        vertices.push_back(normals[i].z);

        vertices.push_back(uvs[i].x);
        vertices.push_back(uvs[i].y);
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Sphere::draw()
{
    glBindVertexArray(vao);
    // 以三角形条带的形式绘制
    glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
}
