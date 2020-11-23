#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection*u_View*vec4(position, 1.0);
}