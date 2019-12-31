#include "SSAOFBO.h"
#include "GBuffer.h"
#include "GL/glew.h"
#include <iostream>
#include <vector>
#include "glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "lib/CS123SceneData.h"

SSAOFBO::SSAOFBO(int width, int height) :
    fbo_handle(0), ssao_color_buffer(0)
{
    glGenFramebuffers(1, &fbo_handle);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);

    glGenTextures(1, &ssao_color_buffer);
    glBindTexture(GL_TEXTURE_2D, ssao_color_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_color_buffer, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    else
        std::cout << "Framebuffer is complete" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

SSAOFBO::~SSAOFBO()
{

}

void SSAOFBO::bindFBO() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
}

void SSAOFBO::unbindFBO() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAOFBO::bindSSAOTexture() {
    glBindTexture(GL_TEXTURE_2D, ssao_color_buffer);
}
