#version 330 core

out vec4 OutColor;

uniform vec3 u_TopColor;
uniform vec3 u_HorizonColor;

in VS_OUT
{
    vec3 VertPosModelSpace;
} frag_in;

void main()
{
    float t = clamp(normalize(frag_in.VertPosModelSpace).y, 0.0, 1.0);
    t = pow(t, 0.6);

    vec3 color = mix(u_HorizonColor, u_TopColor, t);

    OutColor = vec4(color, 1.0);
}