#version 330 core

out vec4 OutColor;

uniform vec3 u_TopColor;
uniform vec3 u_HorizonColor;

in VS_OUT
{
    vec3 VertPosModelSpace;
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
    float t = clamp(normalize(frag_in.VertPosModelSpace).y, 0.0, 1.0);
    t = pow(t, 0.6);

    vec3 top = SRGBToLinear(u_TopColor);
    vec3 horizon = SRGBToLinear(u_HorizonColor);

    vec3 color = LinearToSRGB(mix(horizon, top, t));

    OutColor = vec4(color, 1.0);
}