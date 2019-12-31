#include "Sphere.h"
#include <cmath>
#include <memory>
#include <iostream>
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "gl/shaders/ShaderAttribLocations.h"
#include <algorithm>

Sphere::Sphere(int param1, int param2, float param3) :
    OpenGLShape (true),
    radius(.5),
    m_slices(glm::max(3, param2)),
    m_rings(glm::max(2, param1))
{
    prepareShape();
}

Sphere::~Sphere() {

}

void Sphere::changeShapeSettings(int param1, int param2, float param3) {
    OpenGLShape::changeShapeSettings(param1, param2, param3);
    m_slices = glm::max(3, param2);
    m_rings = glm::max(2, param1);
    m_vertData.clear();
    m_vertData.reserve(m_floatsPerVertex * calcNumVerts());
    createVertVector();
    setVerticesAndBuildVAO();
}

void Sphere::createVertVector() {
    for (int ring = 0; ring < m_rings; ring++) {
        float upper_phi = ring * M_PI / m_rings;
        float lower_phi = (ring + 1) * M_PI / m_rings;

        for (int slice = 0; slice <= m_slices; slice++) {
            float theta = -slice * 2 * M_PI / m_slices;
            glm::vec3 upper_vert = {radius * sin(upper_phi) * cos(theta),
                                    radius * cos(upper_phi),
                                    radius * sin(upper_phi) * sin(theta)};
            glm::vec3 upper_norm = glm::normalize(upper_vert);
            std::vector<float> upper_uv = {float(slice) / float(m_slices), float(ring) / float(m_rings)};
            glm::vec3 lower_vert = {radius * sin(lower_phi) * cos(theta),
                                    radius * cos(lower_phi),
                                    radius * sin(lower_phi) * sin(theta)};
            glm::vec3 lower_norm = glm::normalize(lower_vert);
            std::vector<float> lower_uv = {float(slice) / float(m_slices), float(ring + 1) / float(m_rings)};
            appendVec3ToVector(m_vertData, upper_vert);
            appendVec3ToVector(m_vertData, upper_norm);
            appendToVector(m_vertData, upper_uv);
            appendVec3ToVector(m_vertData, lower_vert);
            appendVec3ToVector(m_vertData, lower_norm);
            appendToVector(m_vertData, lower_uv);
        }
    }
}

int Sphere::calcNumVerts() {
    return 2*m_rings*m_slices + 2*m_rings;
}
