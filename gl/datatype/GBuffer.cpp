#include "GBuffer.h"
#include "GL/glew.h"
#include <iostream>
#include <vector>
#include "glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "lib/CS123SceneData.h"

GBuffer::GBuffer(int width, int height) :
    m_width(width), m_height(height), fbo_handle(0),
    position_tex(0), normal_tex(0), diffuse_tex(0)
{
    glGenFramebuffers(1, &fbo_handle);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);

    //position color buffer
    glGenTextures(1, &position_tex);
    glBindTexture(GL_TEXTURE_2D, position_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position_tex, 0);

    //normal color buffer
    glGenTextures(1, &normal_tex);
    glBindTexture(GL_TEXTURE_2D, normal_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal_tex, 0);


    //color + specular color buffer
    glGenTextures(1, &diffuse_tex);
    glBindTexture(GL_TEXTURE_2D, diffuse_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, diffuse_tex, 0);

    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
            glDrawBuffers(3, attachments);

    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    else
        std::cout << "Framebuffer is complete" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

GBuffer::~GBuffer()
{

}

void GBuffer::bindGBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
}

void GBuffer::unbindGBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::bindPositionTexture() {
    glBindTexture(GL_TEXTURE_2D, position_tex);
}

void GBuffer::bindNormalTexture() {
    glBindTexture(GL_TEXTURE_2D, normal_tex);
}

void GBuffer::bindDiffuseTexture() {
    glBindTexture(GL_TEXTURE_2D, diffuse_tex);
}

void GBuffer::bindSpecularTexture(){
    glBindTexture(GL_TEXTURE_2D, specular_tex);
}
