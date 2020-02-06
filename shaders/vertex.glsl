#version 330 core

layout (location = 0) in vec3 aPos;

out vec2 FragCoord;

void main()
{
    FragCoord = aPos.xy;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
