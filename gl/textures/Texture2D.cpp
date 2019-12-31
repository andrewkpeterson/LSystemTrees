#include "Texture2D.h"

#include <utility>

namespace CS123 { namespace GL {

Texture2D::Texture2D(unsigned char *data, int width, int height, GLenum type) :
    Texture()
{
    GLenum internalFormat = type == GL_FLOAT ? GL_RGBA32F : GL_RGBA;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, type, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() const {
    // TODO [Task 2]
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture2D::unbind() const {
    // TODO Don't forget to unbind!
    glBindTexture(GL_TEXTURE_2D, 0);

}

}}
