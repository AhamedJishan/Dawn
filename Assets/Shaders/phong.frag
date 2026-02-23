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

// to be set by Renderer
uniform vec3 u_CameraPosition;
uniform vec3 u_AmbientColor;
uniform vec3 u_DirectionalLightColor;
uniform vec3 u_DirectionalLightDirection;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} frag_in;

void main()
{
    vec3 phong;

    vec4 baseColor = vec4(u_DiffuseColor, 1);
    if (u_HasDiffuseMap)
    {
        baseColor *= texture(u_DiffuseTexture, frag_in.TexCoord);
    }

    vec3 specularColor = vec3(u_SpecularColor);
    if (u_HasSpecularMap)
    {
        specularColor *= texture(u_SpecularTexture, frag_in.TexCoord).rgb;
    }

    vec3 normal = normalize(frag_in.Normal);
    vec3 lightDir = normalize(-u_DirectionalLightDirection);
    vec3 viewDir = normalize(u_CameraPosition - frag_in.FragPos);
    vec3 halfDir = normalize(lightDir + viewDir);

    // Ambient
    phong = u_AmbientColor * baseColor.rgb;

    // Diffuse
    float diffuseFactor = max(dot(lightDir, normal), 0);
    phong += diffuseFactor * u_DirectionalLightColor * baseColor.rgb;

    // Specular
    if (diffuseFactor > 0)
    {
        float specularFactor = pow(max(dot(normal, halfDir), 0), u_Shininess);
        phong += specularFactor * u_DirectionalLightColor * specularColor;
    }

    OutColor = vec4(phong, baseColor.a);
}