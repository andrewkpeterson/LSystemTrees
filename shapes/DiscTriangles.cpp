#include "DiscTriangles.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "gl/shaders/ShaderAttribLocations.h"
#include "Cube.h"
#include <algorithm>

DiscTriangles::DiscTriangles(int param1, int param2, glm::vec3 normal, float m_ypos, float radius) :
    OpenGLShape (true),
    m_radius(radius),
    m_slices(glm::max(3, param2)),
    m_rings(glm::max(1, param1)),
    m_normal(normal),
    m_ypos(m_ypos)
{
    m_vertData.reserve(calcNumVerts() * m_floatsPerVertex);
    createVertVector();
    setVertexData(m_vertData, m_vertData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, m_vertData.size() / 8);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    setAttribute(ShaderAttrib::TEXCOORD0, 2, 24, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
}

DiscTriangles::~DiscTriangles()
{
}


void DiscTriangles::createVertVector() {

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    for (int ring_idx = 0; ring_idx <= m_rings; ring_idx++) {
        float outer_rad = m_radius - (ring_idx * m_radius / m_rings);
        float inner_rad = m_radius - ((ring_idx + 1) * m_radius / m_rings);
        glm::vec3 outer_vert = {outer_rad * cos(0), m_ypos, outer_rad * sin(0)};
        glm::vec3 inner_vert;
        float theta_sign;
        if (m_ypos > 0) {
            theta_sign = -1;
        } else {
            theta_sign = 1;
        }
        for (int slice_idx = 0; slice_idx <= m_slices; slice_idx++) {
            float theta = theta_sign * slice_idx * (2 * M_PI / m_slices);
            outer_vert = {outer_rad * cos(theta), m_ypos, outer_rad * sin(theta)};
            inner_vert = {inner_rad * cos(theta), m_ypos, inner_rad * sin(theta)};
            positions.push_back(outer_vert);
            normals.push_back(m_normal);
            uvs.push_back(glm::vec2(outer_rad*glm::cos(theta), outer_rad*glm::sin(theta)));
            positions.push_back(inner_vert);
            normals.push_back(m_normal);
            uvs.push_back(glm::vec2(inner_rad*glm::cos(theta), inner_rad*glm::sin(theta)));
        }
    }

    for (int i = 0; i + 3 < positions.size(); i += 2) {
        //create first triangle of sector (3,2,1)

        //append 3rd vertex in sector
        appendVec3ToVector(m_vertData, positions[i+2]);
        appendVec3ToVector(m_vertData, normals[i+2]);
        m_vertData.push_back(uvs[i+2].x);
        m_vertData.push_back(uvs[i+2].y);

        //append 2nd vertex in sector
        appendVec3ToVector(m_vertData, positions[i+1]);
        appendVec3ToVector(m_vertData, normals[i+1]);
        m_vertData.push_back(uvs[i+1].x);
        m_vertData.push_back(uvs[i+1].y);

        //append 1st vertex in sector
        appendVec3ToVector(m_vertData, positions[i]);
        appendVec3ToVector(m_vertData, normals[i]);
        m_vertData.push_back(uvs[i].x);
        m_vertData.push_back(uvs[i].y);


        //create second triangle of sector (2,3,4)
        //append 2nd vertex in sector
        appendVec3ToVector(m_vertData, positions[i+1]);
        appendVec3ToVector(m_vertData, normals[i+1]);
        m_vertData.push_back(uvs[i+1].x);
        m_vertData.push_back(uvs[i+1].y);

        //append 3rd vertex in sector
        appendVec3ToVector(m_vertData, positions[i+2]);
        appendVec3ToVector(m_vertData, normals[i+2]);
        m_vertData.push_back(uvs[i+2].x);
        m_vertData.push_back(uvs[i+2].y);

        //append 4th vertex in sector
        appendVec3ToVector(m_vertData, positions[i+3]);
        appendVec3ToVector(m_vertData, normals[i+3]);
        m_vertData.push_back(uvs[i+3].x);
        m_vertData.push_back(uvs[i+3].y);

    }

}

std::vector<float> DiscTriangles::getVertData() const {
    return m_vertData;
}

int DiscTriangles::calcNumVerts() {
    return 1;
}
