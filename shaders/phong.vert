#version 400 core

layout(location = 0) in vec3 objectspace_position;
layout(location = 1) in vec3 objectspace_normal;

uniform mat4 model, view, projection;

out vec3 worldspace_position; // world-space position
out vec3 worldspace_normal;   // world-space normal

void main() {
    worldspace_position = (model * vec4(objectspace_position, 1.0)).xyz;
    worldspace_normal = normalize(mat3(transpose(inverse(model))) * objectspace_normal);

    gl_Position = projection * view * vec4(worldspace_position, 1.0);
}
