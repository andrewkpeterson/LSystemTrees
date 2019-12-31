#ifndef SPECIALSHAPE1_H
#define SPECIALSHAPE1_H

#include "OpenGLShape.h"
#include <memory>
#include <vector>
#include "Disc.h"
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "CircularShape.h"

class SpecialShape1 : public CircularShape {
public:
    SpecialShape1(int param1, int param2, float param3, bool sceneview);
    ~SpecialShape1();
    void draw() override;
    void changeShapeSettings(int param1, int param2, float param3) override;

private:
    void createVertVector() override;
    int calcNumVerts() override;
    float radiusFunc(float y_pos) override;
    float radiusDeriv(float y_pos) override;

    const float height;
    std::unique_ptr<Disc> base;
    std::unique_ptr<Disc> top;
};

#endif // SPECIALSHAPE1_H
