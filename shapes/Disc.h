#ifndef DISC_H
#define DISC_H

#include "OpenGLShape.h"
#include <memory>
#include <vector>
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"

class Disc : public OpenGLShape {
public:
    Disc(int param1, int param2, glm::vec3 normal, float y_pos, bool made_ring, float radius, bool sceneview);
    ~Disc();
    void changeShapeSettings(int param1, int param2, float param3) override;
    std::vector<float> getVertData() const;

private:
    void createVertVector() override;
    int calcNumVerts() override;

    const float m_radius;
    int m_slices;
    int m_rings;
    glm::vec3 m_normal;
    float m_ypos;
    bool made_first_ring;
    bool m_sceneview;
};

#endif // DISC_H
