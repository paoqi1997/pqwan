#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <util.h>

using std::cout;
using std::endl;

int main()
{
    pqwan::GLFWHelper glfwHelper;

    if (glfwHelper.initWindow() != 0) {
        cout << "error: GLFWHelper::initWindow()" << endl;
        return 1;
    }

    glfwHelper.workWithCamera();

    pqwan::Camera *camera = pqwan::Camera::getInstance();

    int screenWidth = glfwHelper.width;
    int screenHeight = glfwHelper.height;

    // 加载OpenGL函数指针
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        cout << "error: gladLoadGLLoader()" << endl;
        return 2;
    }

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    pqwan::ShaderHelper shaderHelper, lightShaderHelper;

    if (!shaderHelper.init()) {
        cout << "error: ShaderHelper::init()" << endl;
        return 3;
    }
    if (!lightShaderHelper.init("light_shader.vert", "light_shader.frag")) {
        cout << "error(light_shader): ShaderHelper::init()" << endl;
        return 4;
    }

    // 定义36个顶点
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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

    pqwan::Sphere lightSphere;
    lightSphere.loadVertices();

    float aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    auto doFunc = [&](){
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 物体
        shaderHelper.use();

        shaderHelper.uniformVec3("light.position", lightPos);
        shaderHelper.uniformVec3("viewPos", camera->getPosition());

        // 光照属性
        glm::vec3 lightColor;

        double currFrameTime = glfwGetTime();

        lightColor.x = static_cast<float>(std::sin(currFrameTime * 2.0));
        lightColor.y = static_cast<float>(std::sin(currFrameTime * 0.7));
        lightColor.z = static_cast<float>(std::sin(currFrameTime * 1.3));

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);   // 降低影响
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响

        shaderHelper.uniformVec3("light.ambient", ambientColor);
        shaderHelper.uniformVec3("light.diffuse", diffuseColor);
        shaderHelper.uniformVec3("light.specular", 1.0f, 1.0f, 1.0f); // 最大强度

        // 材质属性
        shaderHelper.uniformVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        shaderHelper.uniformVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        shaderHelper.uniformVec3("material.specular", 0.5f, 0.5f, 0.5f); // 中等亮度
        shaderHelper.uniformFloat("material.shininess", 32.0f);

        glm::mat4 model(1.0f);

        // 将世界坐标变换到观察空间
        // 获取相机的 lookAt 矩阵
        glm::mat4 view = camera->getViewMatrix();

        // 从观察空间变换到裁剪空间
        // 创建一个透视投影矩阵
        // 1. FOV表示视野宽度
        // 2. 宽高比用于防止图像被拉伸变形
        // 3. near用于剔除掉离相机太近的物体
        // 4. far用于剔除掉离相机太远的物体
        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), aspect, 0.1f, 100.0f);

        shaderHelper.uniformMat4("model", model);
        shaderHelper.uniformMat4("view", view);
        shaderHelper.uniformMat4("projection", projection);

        glBindVertexArray(vArrayObj);
        // 绘制36个顶点
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 灯
        lightShaderHelper.use();

        glm::mat4 lightModel(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));

        lightShaderHelper.uniformMat4("model", lightModel);
        lightShaderHelper.uniformMat4("view", view);
        lightShaderHelper.uniformMat4("projection", projection);

        lightSphere.draw();
    };

    glfwHelper.show(doFunc);

    glDeleteVertexArrays(1, &vArrayObj);
    glDeleteBuffers(1, &vBufferObj);

    return 0;
}
