#include "Disc.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "gl/shaders/ShaderAttribLocations.h"
#include "Cube.h"
#include <algorithm>

Disc::Disc(int param1, int param2, glm::vec3 normal, float m_ypos, bool made_ring, float radius, bool sceneview) :
    OpenGLShape (true),
    m_radius(radius),
    m_slices(glm::max(3, param2)),
    m_rings(glm::max(1, param1)),
    m_normal(normal),
    m_ypos(m_ypos),
    made_first_ring(true),
    m_sceneview(sceneview)
{
    prepareShape();
}

Disc::~Disc()
{
}

void Disc::changeShapeSettings(int param1, int param2, float param3) {
    OpenGLShape::changeShapeSettings(param1, param2, param3);
    m_slices = glm::max(3, param2);
    m_rings = glm::max(1, param1);
    made_first_ring = false;
    m_vertData.clear();
    m_vertData.reserve(m_floatsPerVertex * calcNumVerts());
    createVertVector();
    setVerticesAndBuildVAO();
}

void Disc::createVertVector() {

    for (int ring_idx = 0; ring_idx < m_rings; ring_idx++) {
        float outer_rad = m_radius - (ring_idx * m_radius / m_rings);
        float inner_rad = m_radius - ((ring_idx + 1) * m_radius / m_rings);
        glm::vec3 outer_vert = {outer_rad * cos(0), m_ypos, outer_rad * sin(0)};
        if (made_first_ring) {
            //repeat first vert of ring so that degenerate triangle is created
            appendVec3ToVector(m_vertData, outer_vert);
            appendVec3ToVector(m_vertData, m_normal);
            appendToVector(m_vertData, std::vector<float>({outer_rad*glm::cos(0.0f), outer_rad*glm::sin(0.0f)}));
        }
        glm::vec3 inner_vert;
        float theta_sign;
        if (m_ypos > 0) {
            theta_sign = 1;
        } else {
            theta_sign = -1;
        }
        for (int slice_idx = 0; slice_idx < m_slices; slice_idx++) {
            float theta;
            if (m_sceneview) {
                theta = -theta_sign * slice_idx * (2 * M_PI / m_slices);
            } else {
                theta = theta_sign * slice_idx * (2 * M_PI / m_slices);
            }
            outer_vert = {outer_rad * cos(theta), m_ypos, outer_rad * sin(theta)};
            inner_vert = {inner_rad * cos(theta), m_ypos, inner_rad * sin(theta)};
            appendVec3ToVector(m_vertData, outer_vert);
            appendVec3ToVector(m_vertData, m_normal);
            appendToVector(m_vertData, std::vector<float>({outer_rad*glm::cos(theta), outer_rad*glm::sin(theta)}));
            appendVec3ToVector(m_vertData, inner_vert);
            appendVec3ToVector(m_vertData, m_normal);
            appendToVector(m_vertData, std::vector<float>({inner_rad*glm::cos(theta), inner_rad*glm::sin(theta)}));
        }
        outer_vert = {outer_rad * cos(0), m_ypos, outer_rad * sin(0.0f)};
        appendVec3ToVector(m_vertData, outer_vert);
        appendVec3ToVector(m_vertData, m_normal);
        appendToVector(m_vertData, std::vector<float>({outer_rad*glm::cos(0.0f), outer_rad*glm::sin(0.0f)}));
        made_first_ring = true;
    }
}

std::vector<float> Disc::getVertData() const {
    return m_vertData;
}

int Disc::calcNumVerts() {
    if (m_sceneview) {
        return 2*m_rings*m_slices + 2*m_rings;
    } else {
        return 2*m_rings*m_slices + 2*m_rings - 1;
    }
}
