#version 330 core

layout (location = 0) in vec3 a_VertexPos;  // mesh vertex, per vertex
layout (location = 1) in float a_T;         // per instance
layout (location = 2) in vec3 a_Position;   // per instance

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    vec3 worldPos = a_VertexPos + a_Position;
    gl_Position = u_Projection * u_View * vec4(worldPos, 1.0);
}