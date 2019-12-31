#ifndef CYLINDER_H
#define CYLINDER_H

#include "OpenGLShape.h"
#include <memory>
#include <vector>
#include "Disc.h"
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "CircularShape.h"

class Cylinder : public CircularShape {
public:
    Cylinder(int param1, int param2, float param3, bool sceneview);
    ~Cylinder();
    void draw() override;
    void changeShapeSettings(int param1, int param2, float param3) override;

private:
    void createVertVector() override;
    int calcNumVerts() override;
    float radiusFunc(float y_pos) override;
    float radiusDeriv(float y_pos) override;

    const float height;
    std::unique_ptr<Disc> m_uppercap;
    std::unique_ptr<Disc> m_lowercap;
};

#endif // CYLINDER_H
