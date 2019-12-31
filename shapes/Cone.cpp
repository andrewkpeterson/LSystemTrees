#include "Cone.h"

#include "Disc.h"
#include <cmath>
#include <memory>
#include <iostream>
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "gl/shaders/ShaderAttribLocations.h"
#include "Cube.h"
#include <algorithm>

Cone::Cone(int param1, int param2, float param3, bool sceneview) :
    CircularShape (param1, param2, param3, sceneview, true),
    height(1.0),
    base(std::make_unique<Disc>(param1, param2, glm::vec3(0,-1,0), -.5, false, .5, sceneview))
{
    prepareShape();
}

Cone::~Cone()
{
}

void Cone::draw() {
    base->draw();
    OpenGLShape::draw();
}

void Cone::changeShapeSettings(int param1, int param2, float param3) {
    CircularShape::changeShapeSettings(param1, param2, param3);
    m_vertData.clear();
    m_vertData.reserve(m_floatsPerVertex * calcNumVerts());
    base->changeShapeSettings(param1, param2, param3);
    createVertVector();
    setVerticesAndBuildVAO();
}

void Cone::createVertVector() {
    float lower_height;
    float lower_rad;
    for (int ring = 0; ring < m_rings - 1; ring++) {
        lower_height = -.5 + ring * height / m_rings;
        lower_rad = .5 - .5 * ring / m_rings;
        makeCircularStrip(lower_height, lower_rad, height / m_rings, .5 / m_rings);
    }

    createTopStrip();
}

void Cone::createTopStrip() {
    float lower_height;
    float lower_rad;
    float vert_comp = .5 / height;
    float x, z;
    lower_height = -.5 + (m_rings - 1) * height / m_rings;
    lower_rad = .5 - .5 * (m_rings - 1) / m_rings;
    x = lower_rad * cos(0);
    z = lower_rad * sin(0);
    float y_norm = vert_comp*glm::sqrt(std::pow(x,2) + std::pow(z,2));
    glm::vec3 lower_vert = {lower_rad * cos(0), lower_height, lower_rad * sin(0)};
    glm::vec3 normal = {x,y_norm, z};
    normal = glm::normalize(normal);
    for (int slice_idx = 0; slice_idx <= m_slices; slice_idx++) {
        float theta = slice_idx * (2 * M_PI / m_slices);
        float x, z;
        x = lower_rad * cos(theta);
        z = lower_rad * sin(theta);
        float y_norm = vert_comp*glm::sqrt(std::pow(x,2) + std::pow(z,2));
        normal = {x,y_norm, z};
        normal = glm::normalize(normal);

        lower_vert = {lower_rad * cos(theta), lower_height, lower_rad * sin(theta)};
        appendVec3ToVector(m_vertData, lower_vert);
        appendVec3ToVector(m_vertData, normal);
        appendToVector(m_vertData, std::vector<float>({1 - (lower_height + .5f), slice_idx / float(m_slices)}));

        float next_theta = (slice_idx + 1) * (2 * M_PI / m_slices);
        float next_x = lower_rad * cos(next_theta);
        float next_z = lower_rad * sin(next_theta);
        y_norm = vert_comp*glm::sqrt(std::pow((x + next_x)/2,2) + std::pow((z + next_z)/2,2));

        glm::vec3 top_normal = {(x + next_x) / 2, y_norm, (z + next_z) / 2};
        top_normal = glm::normalize(top_normal);
        appendVec3ToVector(m_vertData, glm::vec3(0,.5,0));
        appendVec3ToVector(m_vertData, top_normal);
        appendToVector(m_vertData, std::vector<float>({0.0f, slice_idx / float(m_slices)}));
    }
}

float Cone::radiusFunc(float y_pos) {
    return .25 - .5*y_pos;
}

float Cone::radiusDeriv(float y_pos) {
    return -.5;
}

int Cone::calcNumVerts() {
    return 2*m_rings*m_slices + 2*m_rings;
}
