#include "SpecialShape1.h"
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

SpecialShape1::SpecialShape1(int param1, int param2, float param3, bool sceneview) :
    CircularShape (param1, param2, param3, sceneview, true),
    height(1.0),
    base(std::make_unique<Disc>(param1, param2, glm::vec3(0,-1,0), -.5, false, .3, sceneview)),
    top(std::make_unique<Disc>(param1, param2, glm::vec3(0,1,0), .5, false, .3, sceneview))
{
    prepareShape();
}

SpecialShape1::~SpecialShape1()
{
}

void SpecialShape1::changeShapeSettings(int param1, int param2, float param3) {
    CircularShape::changeShapeSettings(param1, param2, param3);
    m_vertData.clear();
    m_vertData.reserve(m_floatsPerVertex * calcNumVerts());
    createVertVector();
    base->changeShapeSettings(param1, param2, param3);
    top->changeShapeSettings(param1, param2, param3);
    setVerticesAndBuildVAO();
}

void SpecialShape1::draw() {
    base->draw();
    top->draw();
    OpenGLShape::draw();
}

void SpecialShape1::createVertVector() {
    for (int ring = 0; ring < m_rings; ring++) {
        float lower_height = -.5 + ring * height / m_rings;
        float upper_height = -.5 + (ring + 1) * height / m_rings;
        float run = radiusFunc(lower_height) - radiusFunc(upper_height);
        makeCircularStrip(lower_height, radiusFunc(lower_height), height / m_rings, run);
    }
}

float SpecialShape1::radiusFunc(float y_pos) {
    return 8.0368*std::pow(y_pos, 5) - 3.9797*std::pow(y_pos, 4) + .9254*std::pow(y_pos, 3) + .5029*std::pow(y_pos, 2) + -.7337*y_pos + .4230;
}

float SpecialShape1::radiusDeriv(float y_pos) {
    return 5*8.0368*std::pow(y_pos, 4) - 4*3.9797*std::pow(y_pos, 3) + 3*.9254*std::pow(y_pos, 2) + 2*.5029*y_pos + -.7337;
}

int SpecialShape1::calcNumVerts() {
    return 2*m_rings*m_slices + 2*m_rings;
}

