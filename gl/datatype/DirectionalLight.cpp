#include "DirectionalLight.h"
#include <iostream>

DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 center, glm::vec3 target) :
    m_color(color), m_center(center), m_target(target), fbo_handle(0),
    depthmap(0), theta(0)
{
    glGenFramebuffers(1, &fbo_handle);

    glGenTextures(1, &depthmap);
    glBindTexture(GL_TEXTURE_2D, depthmap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthmap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    std::cout << m_pos.y << std::endl;
    m_pos.x = m_center.x + sin(0);
    m_pos.y = m_center.y;
    m_pos.z = m_center.z + cos(0);
    computeTransformation();
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::computeTransformation() {
    glm::mat4x4 projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 10.0f);
    glm::mat4x4 view = glm::lookAt(m_pos, m_target, glm::vec3(0.0f,1.0f,0.0f));
    transformation = projection * view;
}

glm::mat4x4 DirectionalLight::getTransformation() {
    return transformation;
}

void DirectionalLight::update(float time_elapsed) {
    theta += time_elapsed;
    m_pos.x = m_center.x + sin(theta);
    m_pos.z = m_center.z + cos(theta);
}

glm::vec3 DirectionalLight::getColor() {
    return m_color;
}

glm::vec3 DirectionalLight::getPos() {
    return m_pos;
}

glm::vec3 DirectionalLight::getDir() {
    return m_target - m_pos;
}

void DirectionalLight::bindFBO() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
}

void DirectionalLight::unbindFBO() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void DirectionalLight::bindDepthMap() {
    glBindTexture(GL_TEXTURE_2D, depthmap);

}
