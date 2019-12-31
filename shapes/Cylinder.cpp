#include "Cylinder.h"
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

Cylinder::Cylinder(int param1, int param2, float param3, bool sceneview) :
    CircularShape (param1, param2, param3, sceneview, true),
    height(1.0),
    m_uppercap(std::make_unique<Disc>(param1, param2, glm::vec3(0,1,0), .5, false, .5, sceneview)),
    m_lowercap(std::make_unique<Disc>(param1, param2, glm::vec3(0,-1,0), -.5, false, .5, sceneview))
{
    prepareShape();
}

Cylinder::~Cylinder()
{
}

void Cylinder::changeShapeSettings(int param1, int param2, float param3) {
    CircularShape::changeShapeSettings(param1, param2, param3);
    m_vertData.clear();
    m_vertData.reserve(m_floatsPerVertex * calcNumVerts());
    createVertVector();
    m_uppercap->changeShapeSettings(param1, param2, param3);
    m_lowercap->changeShapeSettings(param1, param2, param3);
    setVerticesAndBuildVAO();
}

void Cylinder::draw() {
    m_uppercap->draw();
    m_lowercap->draw();
    OpenGLShape::draw();
}

void Cylinder::createVertVector() {
    for (int ring = 0; ring < m_rings; ring++) {
        float lower_height = -.5 + ring * height / m_rings;
        makeCircularStrip(lower_height, .5, height / m_rings, 0);
    }
}

float Cylinder::radiusFunc(float y_pos) {
    return .5;
}

float Cylinder::radiusDeriv(float y_pos) {
    return 0;
}

int Cylinder::calcNumVerts() {
    return 2*m_rings*m_slices + 2*m_rings;
}
