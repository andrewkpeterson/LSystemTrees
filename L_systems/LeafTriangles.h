#ifndef LEAFTRIANGLES_H
#define LEAFTRIANGLES_H


#include "shapes/CircularShapeTriangles.h"
#include "shapes/DiscTriangles.h"

class LeafTriangles : public CircularShapeTriangles {
public:
    LeafTriangles(int param1, int param2, float param3, float b_rad, float t_rad);
    ~LeafTriangles();
    void draw() override;
    void changeShapeSettings(int param1, int param2, float param3) override;

private:
    void createVertVector() override;
    int calcNumVerts() override;
    float radiusFunc(float y_pos) override;
    float radiusDeriv(float y_pos) override;

    const float height;
    float base_rad;
    float top_rad;
};

#endif // LEAFTRIANGLES_H
