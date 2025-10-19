#version 440 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    /// 环境光照
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    /// 漫反射光照
    vec3 norm = normalize(Normal);
    // 计算光线方向
    // 从表面指向光源的向量
    vec3 lightDir = normalize(lightPos - FragPos);
    // 计算漫反射强度
    // dot: 结果是一个标量
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    /// 镜面光照
    float specularStrength = 0.5;
    // 从表面指向相机的向量
    vec3 viewDir = normalize(viewPos - FragPos);
    // 从光源指向表面的向量
    // 入射光 -> 反射光
    vec3 reflectDir = reflect(-lightDir, norm);
    // 计算镜面反射强度
    // shininess: 高光指数
    float shininess = 32;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
