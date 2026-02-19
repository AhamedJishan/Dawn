#version 330 core

out vec4 OutColor;

// to be set by Phong Material
uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_SpecularTexture;
uniform sampler2D u_NormalTexture;
uniform bool u_HasDiffuseMap;
uniform bool u_HasSpecularMap;
uniform bool u_HasNormalMap;
uniform vec3 u_DiffuseColor;
uniform vec3 u_SpecularColor;
uniform float u_Shininess;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} frag_in;

void main()
{
    vec3 baseColor = u_HasDiffuseMap ? texture(u_DiffuseTexture, frag_in.TexCoord).rgb : u_DiffuseColor;
    
    OutColor = vec4(baseColor, 1.0);

    // TODO: Actual Phong Shader tbdl
}