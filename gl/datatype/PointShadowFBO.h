#ifndef SHADOWFBO_H
#define SHADOWFBO_H

#include "glm.hpp"
#include "glm/gtx/transform.hpp"
#include <vector>
#include "gl/shaders/Shader.h"
#include <memory>

class PointShadowFBO
{
public:
    PointShadowFBO(int shadow_width, int shadow_height, glm::vec3 lightpos);
    ~PointShadowFBO();

    int getWidth();
    int getHeight();
    void bind();
    void unbind();
    void calcTransforms();
    void setUpShader(std::shared_ptr<CS123::GL::Shader> shader);
    void bindCubeMap();

private:
    std::vector<glm::mat4x4> shadowTransforms;
    unsigned int depthCubemap;
    unsigned int m_handle;
    int m_width;
    int m_height;
    glm::vec3 m_pos;
};

#endif // SHADOWFBO_H
