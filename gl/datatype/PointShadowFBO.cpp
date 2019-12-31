#include "PointShadowFBO.h"
#include "GL/glew.h"
#include "gl/GLDebug.h"

PointShadowFBO::PointShadowFBO(int shadow_width, int shadow_height, glm::vec3 lightpos) :
    m_width(shadow_width), m_height(shadow_height),
    m_pos(lightpos)
{
    CS123::GL::checkError();
    glGenFramebuffers(1, &m_handle);
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (int i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    calcTransforms();
    CS123::GL::checkError();
}

PointShadowFBO::~PointShadowFBO()
{
    glDeleteFramebuffers(1, &m_handle);
}

void PointShadowFBO::calcTransforms() {
    float aspect = (float)m_width/(float)m_height;
    float near = .001f;
    float far = 25.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
    //right side
    shadowTransforms.push_back(shadowProj *
                     glm::lookAt(m_pos, m_pos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    //left side
    shadowTransforms.push_back(shadowProj *
                     glm::lookAt(m_pos, m_pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    //up side
    shadowTransforms.push_back(shadowProj *
                     glm::lookAt(m_pos, m_pos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    //down side
    shadowTransforms.push_back(shadowProj *
                     glm::lookAt(m_pos, m_pos + glm::vec3(0.0,-1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
    //forward side
    shadowTransforms.push_back(shadowProj *
                     glm::lookAt(m_pos, m_pos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
    //backward side
    shadowTransforms.push_back(shadowProj *
                     glm::lookAt(m_pos, m_pos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
}

void PointShadowFBO::setUpShader(std::shared_ptr<CS123::GL::Shader> shader) {
    shader->setUniform("lightPos", m_pos);
    shader->setUniform("far_plane", float(25.0));
    for (int i = 0; i < shadowTransforms.size(); i++) {
        shader->setUniformArrayByIndex("shadowMatrices", shadowTransforms[i], i);
    }
}

void PointShadowFBO::bindCubeMap() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
}

int PointShadowFBO::getHeight() {
    return m_height;
}

int PointShadowFBO::getWidth() {
    return m_height;
}

void PointShadowFBO::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
}

void PointShadowFBO::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
