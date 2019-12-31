#include "Torus.h"
#include "OpenGLShape.h"
#include <memory>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "gl/shaders/ShaderAttribLocations.h"
#include "CircularShape.h"

Torus::Torus(int param1, int param2, float param3) :
    OpenGLShape (false),
    min_rad(glm::max(.5 * 1 / 100.0, .5 * param3 / 100.0)),
    maj_rad(.5 - min_rad),
    m_slices(glm::max(3, param2)),
    m_rings(glm::max(3, param1))
{
    prepareShape();
}

void Torus::changeShapeSettings(int param1, int param2, float param3) {
    OpenGLShape::changeShapeSettings(param1, param2, param3);
    min_rad = glm::max(.5 * 1 / 100.0, .5 * param3 / 100.0);
    maj_rad = .5 - min_rad;
    m_slices = glm::max(3, param2);
    m_rings = glm::max(3, param1);
    m_vertData.clear();
    m_vertData.reserve(m_floatsPerVertex * calcNumVerts());
    createVertVector();
    setVerticesAndBuildVAO();
}

int Torus::calcNumVerts() {
    return 2 * m_rings * m_slices + 2 * m_rings;
}

Torus::~Torus() {

}

void Torus::createVertVector() {
    for (int ring = 0; ring < m_rings; ring++) {
        float min_curr = ring * 2 * M_PI / m_rings;
        float min_next = (ring + 1) * 2 * M_PI / m_rings;

        for (int slice = 0; slice <= m_slices; slice++) {
            float maj = slice * 2 * M_PI / m_slices;
            glm::vec3 curr_vert = {(maj_rad + min_rad * cos(min_curr)) * cos(maj),
                                    min_rad * sin(min_curr),
                                    (maj_rad + min_rad * cos(min_curr)) * sin(maj)};
            glm::vec3 curr_normal = {cos(min_curr) * cos(maj),
                                     sin(min_curr),
                                     cos(min_curr) * sin(maj)};
            curr_normal = glm::normalize(curr_normal);
            glm::vec3 next_vert = {(maj_rad + min_rad * cos(min_next)) * cos(maj),
                                    min_rad * sin(min_next),
                                    (maj_rad + min_rad * cos(min_next)) * sin(maj)};
            glm::vec3 next_normal = {cos(min_next) * cos(maj),
                                     sin(min_next),
                                     cos(min_next) * sin(maj)};
            next_normal = glm::normalize(next_normal);
            appendVec3ToVector(m_vertData, curr_vert);
            appendVec3ToVector(m_vertData, curr_normal);
            appendVec3ToVector(m_vertData, next_vert);
            appendVec3ToVector(m_vertData, next_normal);
        }
    }
}

