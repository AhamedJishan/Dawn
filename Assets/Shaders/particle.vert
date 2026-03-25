#version 330 core

layout (location = 0) in vec3 a_VertexPos;  // mesh vertex, per vertex
layout (location = 1) in float a_T;         // per instance
layout (location = 2) in vec3 a_Position;   // per instance

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_ScaleOverTime[5];

void main()
{
    vec3 transformedPos = a_VertexPos * u_ScaleOverTime[0] + a_Position;
    //vec3 worldPos = a_VertexPos + a_Position;
    gl_Position = u_Projection * u_View * vec4(transformedPos, 1.0);
}