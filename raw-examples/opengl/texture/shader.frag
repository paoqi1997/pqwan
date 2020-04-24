#version 440 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D sampler1;
uniform sampler2D sampler2;

void main()
{
    /*
    // 将纹理颜色与顶点颜色混合
    FragColor = texture(sampler1, TexCoord) * vec4(ourColor, 1.0);
    */

    /**
     * min(x, y, a) = (1-a)*x + a*y
     * 根据a对x和y进行线性插值
     *
     * for example:
     *   vec3 colorRGB_0, colorRGB_1;
     *   float alpha;
     *   resultRGB = colorRGB_0 * (1.0 - alpha) + colorRGB_1 * alpha;
     */
    FragColor = mix(texture(sampler1, TexCoord), texture(sampler2, TexCoord), 0.2);
}
