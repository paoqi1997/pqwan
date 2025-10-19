#version 440 core

struct Material {
    vec3 ambient;    // 在环境光照下表面的颜色
    vec3 diffuse;    // 在漫反射光照下表面的颜色
    vec3 specular;   // 在镜面光照下表面的颜色
    float shininess; // 高光指数
};

struct Light {
    vec3 position; // 光源位置
    vec3 ambient;  // 环境光
    vec3 diffuse;  // 漫反射
    vec3 specular; // 镜面光
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    /// 环境光照
    vec3 ambient = light.ambient * material.ambient;

    /// 漫反射光照
    vec3 norm = normalize(Normal);
    // 计算光线方向
    // 从表面指向光源的向量
    vec3 lightDir = normalize(light.position - FragPos);
    // 计算漫反射强度
    // dot: 结果是一个标量
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    /// 镜面光照
    // 从表面指向相机的向量
    vec3 viewDir = normalize(viewPos - FragPos);
    // 从光源指向表面的向量
    // 入射光 -> 反射光
    vec3 reflectDir = reflect(-lightDir, norm);
    // 计算镜面反射强度
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
