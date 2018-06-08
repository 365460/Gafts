
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture_diffuse1;

uniform vec3 warning_color;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoord) + vec4(warning_color, 0.3);
}
