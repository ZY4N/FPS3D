#version 460

uniform sampler2D tex;
uniform float colorMerge;
uniform vec4 meshColor;
//uniform vec3 rayDirection;

in vec2 frag_texCoord;
in vec3 frag_normal;

out vec4 FragColor;

void main() {
    float light = (1.0 + dot(normalize(vec3(1.0, -1.0, 1.0)), -frag_normal)) * 0.5;
    vec4 color = colorMerge * meshColor + (1.0 - colorMerge) * texture(tex, frag_texCoord);
    FragColor = vec4(light * color.xyz, color.w);
}
