#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec3 aPosOut;

void main()
{
    FragColor = vec4(aPosOut, 1.0f);
}
