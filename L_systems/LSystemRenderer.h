#ifndef LSYSTEMPARSER_H
#define LSYSTEMPARSER_H

#include <memory>
#include "gl/shaders/Shader.h"

class LSystemRenderer
{
public:
    LSystemRenderer(std::shared_ptr<CS123::GL::Shader> shader);
    ~LSystemRenderer();
    void renderTree(std::string treestring);

private:
    std::shared_ptr<CS123::GL::Shader> m_shader;

};

#endif // LSYSTEMPARSER_H
