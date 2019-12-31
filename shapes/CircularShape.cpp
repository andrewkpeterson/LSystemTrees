#include "CircularShape.h"
#include <memory>
#include <vector>
#include "Disc.h"
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "CircularShape.h"

CircularShape::CircularShape(int param1, int param2, float param3, bool sceneview, bool uv) :
    OpenGLShape(true),
    made_first_ring(false),
    m_slices(glm::max(3, param2)),
    m_rings(glm::max(1, param1)),
    m_sceneview(sceneview)
{

}

CircularShape::~CircularShape()
{

}

void CircularShape::changeShapeSettings(int param1, int param2, float param3) {
    OpenGLShape::changeShapeSettings(param1, param2, param3);
    made_first_ring = false;
    m_slices = glm::max(3, param2);
    m_rings = glm::max(1, param1);
}

void CircularShape::makeCircularStrip(float lower_height, float lower_rad, float rise, float run) {
    //place strips around
    glm::vec3 lower_vert;
    glm::vec3 upper_vert;
    glm::vec3 normal;
    float upper_height = lower_height + rise;
    float upper_rad = lower_rad - run;
    if (made_first_ring) {
        float x, z;
        x = upper_rad * cos(0);
        z = upper_rad * sin(0);
        float vert_comp = -radiusDeriv(lower_height);
        float y_norm = vert_comp*glm::sqrt(std::pow(x,2) + std::pow(z,2));
        normal = {x,y_norm, z};
        normal = glm::normalize(normal);
        lower_vert = {lower_rad * cos(0), lower_height, lower_rad * sin(0)};
        appendVec3ToVector(m_vertData, lower_vert);
        appendVec3ToVector(m_vertData, normal);
        appendToVector(m_vertData, std::vector<float>({1 - (upper_height + .5f), 0}));
        made_first_ring = true;
    }
    for (int slice_idx = 0; slice_idx <= m_slices; slice_idx++) {
        float theta = slice_idx * (2 * M_PI / m_slices);
        float x, z;
        x = lower_rad * cos(theta);
        z = lower_rad * sin(theta);

        float lower_y_norm = -radiusDeriv(lower_height)*glm::sqrt(std::pow(x,2) + std::pow(z,2));
        normal = {x,lower_y_norm, z};
        normal = glm::normalize(normal);
        lower_vert = {lower_rad * cos(theta), lower_height, lower_rad * sin(theta)};
        appendVec3ToVector(m_vertData, lower_vert);
        appendVec3ToVector(m_vertData, normal);
        appendToVector(m_vertData, std::vector<float>({1 - (lower_height + .5f), slice_idx / float(m_slices)}));

        x = upper_rad * cos(theta);
        z = upper_rad * sin(theta);
        float upper_y_norm = -radiusDeriv(upper_height)*glm::sqrt(std::pow(x,2) + std::pow(z,2));
        normal = {x,upper_y_norm, z};
        normal = glm::normalize(normal);
        upper_vert = {upper_rad * cos(theta), upper_height, upper_rad * sin(theta)};
        appendVec3ToVector(m_vertData, upper_vert);
        appendVec3ToVector(m_vertData, normal);
        appendToVector(m_vertData, std::vector<float>({1 - (upper_height + .5f), slice_idx / float(m_slices)}));
    }
}
