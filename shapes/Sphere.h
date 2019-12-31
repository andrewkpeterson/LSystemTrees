#ifndef SPHERE_H
#define SPHERE_H

#include "OpenGLShape.h"
#include <memory>
#include <vector>
#include "Disc.h"
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "CircularShape.h"

class Sphere : public OpenGLShape {
public:
    Sphere(int param1, int param2, float param3);
    ~Sphere();
    void changeShapeSettings(int param1, int param2, float param3) override;

private:
    void createVertVector() override;
    int calcNumVerts() override;

    const float radius;
    int m_rings;
    int m_slices;
};

#endif // SPHERE_H
