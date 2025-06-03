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
// 鼠标移动时回调
void cursorPosCb(GLFWwindow* window, double xposIn, double yposIn);
// 使用鼠标滚轮时回调
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
    bool FirstMouse; // 是否首次移动鼠标
    float LastX;     // 上一帧鼠标光标的x坐标
    float LastY;     // 上一帧鼠标光标的y坐标
    float Zoom;      // FOV
private:
    Camera();
    static Camera *instance;
    glm::vec3 position; // 相机位置，即一个指向相机位置的向量
    glm::vec3 front;    // 相机朝向，即相机的正前方
    glm::vec3 up;       // 相机的正上方
    glm::vec3 right;    // 相机的正右方
    glm::vec3 worldUp;  // 世界坐标系的正上方
    float yaw;   // 偏航角，视线在 xz 平面上的投影和x轴的夹角，是描述如何往左或者往右看的角
    float pitch; // 俯仰角，视线和 xz 平面的夹角，是描述如何往上或者往下看的角
    float movementSpeed;    // 鼠标移动速度的基准值
    float mouseSensitivity; // 鼠标灵敏度
};

} // namespace pqwan

#endif // PQWAN_UTIL_H
