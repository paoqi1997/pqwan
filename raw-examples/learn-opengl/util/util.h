#ifndef PQWAN_UTIL_H
#define PQWAN_UTIL_H

#include <fstream>
#include <functional>
#include <string>
#include <vector>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace pqwan
{

template <typename T>
class Singleton
{
public:
    static T* getInstance() {
        if (instance == nullptr) {
            instance = new T();
        }
        return instance;
    }
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator = (const Singleton&) = delete;
    Singleton& operator = (Singleton&&) = delete;
protected:
    Singleton() = default;
    virtual ~Singleton() = default;
private:
    static T* instance;
};

template <typename T>
T* Singleton<T>::instance = nullptr;

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
    void show(const std::function<void()>& doFunc);
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
    void uniformVec3(const std::string& name, float x, float y, float z);
    void uniformVec3(const std::string& name, const glm::vec3& vector);
    void uniformMat4(const std::string& name, const glm::mat4& matrix);
    int getShaderProgram() const { return shaderProgram; }
private:
    int loadVertexShader(const char *vertexShaderFilePath);
    int loadFragmentShader(const char *fragmentShaderFilePath);
    bool linkProgram(int vertexShader, int fragmentShader);
private:
    int shaderProgram;
};

class Camera : public Singleton<Camera>
{
    // Singleton<Camera> 访问 Camera 私有的构造函数
    friend class Singleton<Camera>;
public:
    enum class Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
    };
    glm::mat4 getViewMatrix();
    glm::vec3 getPosition() const { return position; }
    void handleKeyboard(Direction direction, float deltaTime);
    void handleMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void handleMouseScroll(float yoffset);
private:
    Camera();
    void onUpdateVectors();
public:
    bool FirstMouse; // 是否首次移动鼠标
    float LastX;     // 上一帧鼠标光标的x坐标
    float LastY;     // 上一帧鼠标光标的y坐标
    float Zoom;      // FOV
private:
    glm::vec3 position; // 相机位置，即一个指向相机位置的向量
    glm::vec3 front;    // 相机朝向，即相机的正前方
    glm::vec3 right;    // 相机的正右方
    glm::vec3 up;       // 相机的正上方
    glm::vec3 worldUp;  // 世界坐标系的正上方
    float yaw;   // 偏航角，视线在 xz 平面上的投影和x轴的夹角，是描述如何往左或者往右看的角
    float pitch; // 俯仰角，视线和 xz 平面的夹角，是描述如何往上或者往下看的角
    float movementSpeed;    // 鼠标移动速度的基准值
    float mouseSensitivity; // 鼠标灵敏度
};

// 球体
class Sphere
{
public:
    Sphere(float radius = 1.0f, int xSegments = 64, int ySegments = 64);
    ~Sphere();
    void loadVertices();
    void draw();
private:
    void generateVertices(float radius, int xSegments, int ySegments);
    void generateIndices(float radius, int xSegments, int ySegments);
private:
    std::vector<glm::vec3> positions;  // 顶点坐标
    std::vector<glm::vec3> normals;    // 法线
    std::vector<glm::vec2> uvs;        // UV
    std::vector<unsigned int> indices; // 顶点顺序索引
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
};

} // namespace pqwan

#endif // PQWAN_UTIL_H
