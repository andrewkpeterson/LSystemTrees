#ifndef BRANCHTRIANGLES_H
#define BRANCHTRIANGLES_H

#include "shapes/CircularShapeTriangles.h"
#include "shapes/DiscTriangles.h"

class BranchTriangles : public CircularShapeTriangles {
public:
    BranchTriangles(int param1, int param2, float param3, float b_rad, float t_rad);
    ~BranchTriangles();
    void draw() override;
    void changeShapeSettings(int param1, int param2, float param3) override;

private:
    void createVertVector() override;
    int calcNumVerts() override;
    float radiusFunc(float y_pos) override;
    float radiusDeriv(float y_pos) override;
    std::vector<float> &getBaseVerts();
    std::vector<float> &getTopVerts();

    const float height;
    const std::unique_ptr<DiscTriangles> base;
    const std::unique_ptr<DiscTriangles> top;
    float base_rad;
    float top_rad;
};

#endif // BRANCHTRIANGLES_H
