#ifndef DISCTRIANGLES_H
#define DISCTRIANGLES_H

#include "OpenGLShape.h"
#include <memory>
#include <vector>
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"

class DiscTriangles : public OpenGLShape
{
public:
    DiscTriangles(int param1, int param2, glm::vec3 normal, float y_pos, float radius);
    ~DiscTriangles();
    std::vector<float> getVertData() const;

private:
    void createVertVector() override;
    int calcNumVerts() override;

    const float m_radius;
    int m_slices;
    int m_rings;
    glm::vec3 m_normal;
    float m_ypos;
};

#endif // DISCTRIANGLES_H
