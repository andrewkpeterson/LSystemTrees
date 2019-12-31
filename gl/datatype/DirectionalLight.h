#ifndef DIRECTIONALSHADOWFBO_H
#define DIRECTIONALSHADOWFBO_H

#include "glm.hpp"
#include "glm/gtx/transform.hpp"
#include <vector>
#include "gl/shaders/Shader.h"
#include <memory>

class DirectionalLight
{
public:
    DirectionalLight(glm::vec3 color, glm::vec3 center, glm::vec3 target);
    ~DirectionalLight();
    void update(float time_elapsed);
    glm::vec3 getColor();
    glm::vec3 getPos();
    glm::vec3 getDir();
    void computeTransformation();
    glm::mat4x4 getTransformation();
    void bindFBO();
    void unbindFBO();
    void bindDepthMap();

private:
    unsigned int fbo_handle;
    unsigned int depthmap;
    glm::vec3 m_color;
    glm::vec3 m_center;
    glm::vec3 m_pos;
    glm::vec3 m_target;
    glm::mat4x4 transformation;
    float theta;
};

#endif // DIRECTIONALSHADOWFBO_H
