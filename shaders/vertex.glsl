#version 460

uniform mat4 projectionMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_texcoord;
layout (location = 2) in vec3 vertex_normal;
//layout (location = 3) in vec3 vertex_color;

out vec2 frag_texCoord;
out vec3 frag_normal;

void main() {	
    gl_Position = projectionMat * viewMat * modelMat * vec4(vertex_position, 1.0);
    frag_texCoord = vertex_texcoord;
}
