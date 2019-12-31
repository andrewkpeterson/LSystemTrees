#ifndef TORUS_H
#define TORUS_H

#include "OpenGLShape.h"
#include <memory>
#include <vector>
#include "Disc.h"
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"

class Torus : public OpenGLShape
{
public:
    Torus(int param1, int param2, float param3);
    ~Torus();
    void changeShapeSettings(int param1, int param2, float param3) override;

private:
    void createVertVector() override;
    int calcNumVerts() override;

    int m_rings;
    int m_slices;
    float min_rad;
    float maj_rad;
};

#endif // TORUS_H
