#version 440 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // 将世界空间坐标传递给片元着色器
    FragPos = vec3(model * vec4(aPos, 1.0f));
    // 法线矩阵×法向量
    // inverse: 计算矩阵的逆矩阵
    // transpose: 计算矩阵的转置矩阵
    // 法线矩阵为模型矩阵左上角3×3部分的逆矩阵的转置矩阵
    Normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = projection * view * vec4(FragPos, 1.0f);
}
