#version 330 core
in vec3 fColor;
out vec4 color;

uniform samplerCube skybox;

void main()
{
    color = vec4(fColor, 1.0f);   
}

