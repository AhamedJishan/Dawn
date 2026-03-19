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
uniform float u_FogDensity;
uniform vec3 u_FogColor;
uniform vec3 u_CameraPosition;
uniform vec3 u_AmbientColor;
uniform vec3 u_DirectionalLightColor;
uniform vec3 u_DirectionalLightDirection;
uniform float u_DirectionalLightIntensity;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} frag_in;

vec3 SRGBToLinear(vec3 c) {
    return pow(c, vec3(2.2));
}
vec4 SRGBToLinear(vec4 c) {
    return vec4(pow(c.rgb, vec3(2.2)), c.a);
}
vec3 LinearToSRGB(vec3 c) {
    return pow(c, vec3(1.0 / 2.2));
}
vec4 LinearToSRGB(vec4 c) {
    return vec4(pow(c.rgb, vec3(1.0 / 2.2)), c.a);
}

void main()
{
    vec3 phong;

    vec4 baseColor = SRGBToLinear(vec4(u_DiffuseColor, 1));
    if (u_HasDiffuseMap)
    {
        baseColor *= SRGBToLinear(texture(u_DiffuseTexture, frag_in.TexCoord));
    }

    vec3 specularColor = SRGBToLinear(vec3(u_SpecularColor));
    if (u_HasSpecularMap)
    {
        specularColor *= SRGBToLinear(texture(u_SpecularTexture, frag_in.TexCoord).rgb);
    }

    vec3 normal = normalize(frag_in.Normal);
    vec3 lightDir = normalize(-u_DirectionalLightDirection);
    vec3 viewDir = normalize(u_CameraPosition - frag_in.FragPos);
    vec3 halfDir = normalize(lightDir + viewDir);

    vec3 directionalLightColor = u_DirectionalLightIntensity * SRGBToLinear(u_DirectionalLightColor);

    // --- AMBIENT ---
    phong = SRGBToLinear(u_AmbientColor) * baseColor.rgb;

    // --- DIFFUSE ---
    float diffuseFactor = max(dot(lightDir, normal), 0);
    phong += diffuseFactor * directionalLightColor * baseColor.rgb;

    // --- SPECULAR ---
    if (diffuseFactor > 0)
    {
        float specularFactor = pow(max(dot(normal, halfDir), 0), u_Shininess);
        phong += specularFactor * directionalLightColor * specularColor;
    }

    // --- FOG (Exponential squared) ---
    float dist = length(frag_in.FragPos - u_CameraPosition);
    float fogIntensity = 1 - exp(-pow(dist * u_FogDensity, 2.0));

    vec3 finalColor = LinearToSRGB(mix(phong, SRGBToLinear(u_FogColor), fogIntensity));
    
    OutColor = vec4(finalColor, baseColor.a);
}