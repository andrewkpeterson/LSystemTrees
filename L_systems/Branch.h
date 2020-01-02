#ifndef BRANCH_H
#define BRANCH_H

#include "shapes/OpenGLShape.h"
#include <memory>
#include <vector>
#include "shapes/Disc.h"
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "shapes/CircularShape.h"

class Branch : public CircularShape {
public:
    Branch(int param1, int param2, float param3, bool sceneview, float b_rad, float t_rad);
    ~Branch();
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
    float base_rad;
    float top_rad;
};

#endif // BRANCH_H
