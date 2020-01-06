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
    OpenGLShape::draw();
}

void LeafTriangles::createVertVector() {
    for (int ring = 0; ring < m_rings; ring++) {
        float lower_height = ring * height / m_rings;
        float upper_height = (ring + 1) * height / m_rings;
        float run = radiusFunc(lower_height) - radiusFunc(upper_height);
        makeCircularStrip(lower_height, radiusFunc(lower_height), height / m_rings, run);
    }
}

float LeafTriangles::radiusFunc(float y_pos) {
    return -y_pos*y_pos+1;
}

float LeafTriangles::radiusDeriv(float y_pos) {
    return -2.0*y_pos;
}
int LeafTriangles::calcNumVerts() {
    return 2*m_rings*m_slices + 2*m_rings;
}
