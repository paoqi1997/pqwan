#version 440 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    /**
     * mix(x, y, a) = (1-a)*x + a*y
     * 根据a对x及y进行线性插值
     *
     * like this:
     *   vec3 colorRGB_0, colorRGB_1;
     *   float alpha;
     *   resultRGB = colorRGB_0 * (1.0 - alpha) + colorRGB_1 * alpha;
     */
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
