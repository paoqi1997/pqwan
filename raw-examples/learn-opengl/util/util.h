#ifndef PQWAN_UTIL_H
#define PQWAN_UTIL_H

#include <fstream>
#include <functional>
#include <string>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace pqwan
{

class File
{
public:
    File(const char *sFileName, std::ios_base::openmode mode = std::ios_base::in);
    ~File();
    const char* getFileContent() const { return fileContent.c_str(); }
private:
    std::ifstream is;
    std::string fileContent;
};

// 窗口大小变化时回调
void frameBufferSizeCb(GLFWwindow* window, int width, int height);
// xxx
void cursorPosCb(GLFWwindow* window, double xposIn, double yposIn);
// xxx
void scrollCb(GLFWwindow* window, double xoffset, double yoffset);

class GLFWHelper
{
public:
    GLFWHelper();
    ~GLFWHelper();
    int initWindow(int width = 800, int height = 600, const char *title = "Hello OpenGL");
    void workWithCamera();
    void show(const std::function<void()>& func);
public:
    int width;
    int height;
private:
    GLFWwindow *window;
    bool hasCamera;
    float lastFrameTime;
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

class Camera
{
public:
    enum Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
    };
public:
    static Camera* getInstance() {
        if (instance == nullptr) {
            instance = new Camera();
        }
        return instance;
    }
    Camera(const Camera&) = delete;
    Camera& operator = (const Camera&) = delete;
    glm::mat4 getViewMatrix();
    void handleKeyboard(Direction direction, float deltaTime);
    void handleMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void handleMouseScroll(float yoffset);
private:
    void onUpdateVectors();
public:
    bool FirstMouse;
    float LastX;
    float LastY;
    float Zoom;
private:
    Camera();
    static Camera *instance;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
};

} // namespace pqwan

#endif // PQWAN_UTIL_H
