#version 460

uniform sampler2D tex;
uniform float colorMerge;
uniform vec4 meshColor;
//uniform vec3 rayDirection;

in vec2 frag_texCoord;
in vec3 frag_normal;

out vec4 FragColor;

void main() {
    //float light = dot(rayDirection, frag_normal);
    FragColor = (colorMerge * meshColor + (1.0 - colorMerge) * texture(tex, frag_texCoord));
}
