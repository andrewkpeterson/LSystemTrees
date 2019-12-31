#version 400 core

in vec3 worldspace_position; // world-space position
in vec3 worldspace_normal;   // world-space normal

out vec3 fragColor;

void main(){
    vec3 worldspace_toLight = normalize(vec3(10.0) - worldspace_position);
    fragColor = vec3(0.3 + 0.7 * max(0.0, dot(normalize(worldspace_normal), worldspace_toLight)));
    //fragColor = vec3(1.0,0,0);
}
