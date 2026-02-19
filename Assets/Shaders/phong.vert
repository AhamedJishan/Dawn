#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

// to be set by Renderer
uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

void main()
{
	vec4 worldPos = u_Model * vec4(a_Position, 1.0);
    gl_Position = u_ViewProjection * worldPos;

    vs_out.FragPos = worldPos.xyz;
    vs_out.Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    vs_out.TexCoord = a_TexCoord;
}