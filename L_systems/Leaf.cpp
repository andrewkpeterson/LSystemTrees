#include "Leaf.h"
#include "shapes/Disc.h"
#include <cmath>
#include <memory>
#include <iostream>
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "gl/shaders/ShaderAttribLocations.h"
#include "shapes/Cube.h"
#include <algorithm>

Leaf::Leaf(int param1, int param2, float param3, bool sceneview, float b_rad, float t_rad) :
    CircularShape (param1, param2, param3, sceneview, true),
    height(1.0),
    base(std::make_unique<Disc>(param1, param2, glm::vec3(0,-1,0), -.5, false, b_rad, sceneview)),
    top(std::make_unique<Disc>(param1, param2, glm::vec3(0,1,0), .5, false, t_rad, sceneview)),
    base_rad(b_rad),
    top_rad(t_rad)
{
    prepareShape();
}

Leaf::~Leaf()
{
}

void Leaf::changeShapeSettings(int param1, int param2, float param3) {
    CircularShape::changeShapeSettings(param1, param2, param3);
    m_vertData.clear();
    m_vertData.reserve(m_floatsPerVertex * calcNumVerts());
    createVertVector();
    base->changeShapeSettings(param1, param2, param3);
    top->changeShapeSettings(param1, param2, param3);
    setVerticesAndBuildVAO();
}

void Leaf::draw() {
    base->draw();
    top->draw();
    OpenGLShape::draw();
}

void Leaf::createVertVector() {
    for (int ring = 0; ring < m_rings; ring++) {
        float lower_height = -.5 + ring * height / m_rings;
        float upper_height = -.5 + (ring + 1) * height / m_rings;
        float run = radiusFunc(lower_height) - radiusFunc(upper_height);
        makeCircularStrip(lower_height, radiusFunc(lower_height), height / m_rings, run);
    }
}

float Leaf::radiusFunc(float y_pos) {
    return glm::sin(M_PI*(y_pos+.5));
}

float Leaf::radiusDeriv(float y_pos) {
    return M_PI*glm::cos(M_PI*(y_pos+.5));
}

int Leaf::calcNumVerts() {
    return 2*m_rings*m_slices + 2*m_rings;
}
