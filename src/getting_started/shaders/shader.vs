#version 330 core
float offset = 0.5f;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

out vec3 aPosOut;

void main()
{
    gl_Position = vec4(aPos.x + offset, -aPos.y, aPos.z, 1.0);
    aPosOut = aPos;
    ourColor = aColor;
}
