#include "LeafTriangles.h"
#include "shapes/DiscTriangles.h"
#include <cmath>
#include <memory>
#include <iostream>
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "gl/shaders/ShaderAttribLocations.h"
#include "shapes/Cube.h"
#include <algorithm>

LeafTriangles::LeafTriangles(int param1, int param2, float param3, float b_rad, float t_rad) :
    CircularShapeTriangles (param1, param2, param3),
    height(1.0),
    base(std::make_unique<DiscTriangles>(param1, param2, glm::vec3(0,-1,0), -.5, b_rad)),
    top(std::make_unique<DiscTriangles>(param1, param2, glm::vec3(0,1,0), .5, t_rad)),
    base_rad(b_rad),
    top_rad(t_rad)
{
    m_vertData.reserve(calcNumVerts() * m_floatsPerVertex);
    createVertVector();
    setVertexData(m_vertData, m_vertData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, m_vertData.size() / 8);
    //appendToVector(m_vertData, base->getVertData());
    //appendToVector(m_vertData, top->getVertData());
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    setAttribute(ShaderAttrib::TEXCOORD0, 2, 24, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
}

LeafTriangles::~LeafTriangles()
{
}

void LeafTriangles::changeShapeSettings(int param1, int param2, float param3) {

}

void LeafTriangles::draw() {
    base->draw();
    top->draw();
    OpenGLShape::draw();
}

void LeafTriangles::createVertVector() {
    for (int ring = 0; ring < m_rings; ring++) {
        float lower_height = -.5 + ring * height / m_rings;
        float upper_height = -.5 + (ring + 1) * height / m_rings;
        float run = radiusFunc(lower_height) - radiusFunc(upper_height);
        makeCircularStrip(lower_height, radiusFunc(lower_height), height / m_rings, run);
    }
}

float LeafTriangles::radiusFunc(float y_pos) {
    return glm::sin(M_PI*(y_pos+.5));
}

float LeafTriangles::radiusDeriv(float y_pos) {
    return M_PI*glm::cos(M_PI*(y_pos+.5));
}

int LeafTriangles::calcNumVerts() {
    return 2*m_rings*m_slices + 2*m_rings;
}
