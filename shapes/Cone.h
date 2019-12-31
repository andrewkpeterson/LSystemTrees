#ifndef CONE_H
#define CONE_H

#include "OpenGLShape.h"
#include <memory>
#include <vector>
#include "Disc.h"
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "CircularShape.h"

class Cone : public CircularShape {
public:
    Cone(int param1, int param2, float param3, bool sceneview);
    ~Cone();
    void draw() override;
    void changeShapeSettings(int param1, int param2, float param3) override;

private:
    void createVertVector() override;
    int calcNumVerts() override;
    float radiusFunc(float y_pos) override;
    float radiusDeriv(float y_pos) override;

    /**
     * Creates the top strip of the cone with vertex normals at the
     * singularity that are normal to the face they belong to.
     */
    void createTopStrip();

    const float height;
    std::unique_ptr<Disc> base;
};
#endif // CONE_H
