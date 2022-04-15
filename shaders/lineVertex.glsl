#version 460

uniform mat4 projectionMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

layout (location = 0) in vec3 vertex_position;

void main() {
    gl_Position = projectionMat * viewMat * modelMat * vec4(vertex_position, 1.0);
}
