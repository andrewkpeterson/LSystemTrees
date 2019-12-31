#include "Cube.h"
#include <cmath>
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "gl/shaders/ShaderAttribLocations.h"

Cube::Cube(int param1, int param2, float param3) :
    OpenGLShape (true),
    m_halfside(.5),
    m_numsides(6),
    m_createdface(false),
    m_param1(glm::max(1, param1))
{
    prepareShape();
}

Cube::~Cube()
{
}

void Cube::createVertVector() {
    chooseSide(Side::XNEG);
    chooseSide(Side::XPOS);
    chooseSide(Side::YNEG);
    chooseSide(Side::YPOS);
    chooseSide(Side::ZNEG);
    chooseSide(Side::ZPOS);
}

void Cube::changeShapeSettings(int param1, int param2, float param3) {
    OpenGLShape::changeShapeSettings(param1, param2, param3);
    m_param1 = glm::max(1, param1);
    m_createdface = false;
    m_vertData.clear();
    m_vertData.reserve(m_floatsPerVertex * calcNumVerts());
    createVertVector();
    setVerticesAndBuildVAO();
}

int Cube::calcNumVerts() {
    return 12 * std::pow(m_param1,2) + 24*m_param1 + 3;
}

std::vector<float> Cube::makeStrip(std::vector<float> top_left, std::vector<float> normal, float row_inc,
                                   float col_inc, Direction vert, Direction horz, int row) {
    appendToVector(m_vertData, top_left);
    appendToVector(m_vertData, normal);
    std::vector<float> bottom_left = top_left; //creates separate COPY of top_left
    std::vector<float> upper_uv = {0.0f, float(row) / float(m_param1)};
    std::vector<float> bottom_uv = {0.0f, glm::abs(row_inc) + float(row) / float(m_param1)};
    appendToVector(m_vertData, upper_uv);

    bottom_left[vert] += row_inc;
    appendToVector(m_vertData, bottom_left);
    appendToVector(m_vertData, normal);
    appendToVector(m_vertData, bottom_uv);
    std::vector<float> upper = top_left;
    std::vector<float> lower = bottom_left;
    for (int col = 0; col < m_param1; col++) {
        upper[horz] += col_inc;
        upper_uv[0] += col_inc;
        appendToVector(m_vertData, upper);
        appendToVector(m_vertData, normal);
        appendToVector(m_vertData, upper_uv);

        lower[horz] += col_inc;
        bottom_uv[0] += col_inc;
        appendToVector(m_vertData, lower);
        appendToVector(m_vertData, normal);
        appendToVector(m_vertData, bottom_uv);
    }
    //return last vertex without normal
    return lower;
}

void Cube::makeSide(Side side, std::vector<float> top_left, std::vector<float> normal, Direction vert, Direction horz) {
    //call makeStrip and add degenerate triangles in between strips

    float row_inc; //increment to get to next row
    if (side == YPOS) {
        row_inc = float(1.0)/float(m_param1);
    } else if (side == XNEG || side == XPOS || side == YNEG || side == ZPOS || side == ZNEG) {
        row_inc = -float(1.0)/float(m_param1);
    }

    float col_inc; //increment to get to next column
    if (side == XNEG || side == ZPOS || side == YPOS || side == YNEG) {
        col_inc = float(1.0)/float(m_param1);
    } else if (side == XPOS || side == ZNEG){
        col_inc = -float(1.0)/float(m_param1);
    }

    for (int row = 0; row < m_param1; row++) {
        if (m_createdface) {
            appendToVector(m_vertData, top_left);
            appendToVector(m_vertData, normal);
            std::vector<float> uv = {0.0f, float(row) / float(m_param1)};
            appendToVector(m_vertData, uv);
        }
        std::vector<float> lastVert = makeStrip(top_left, normal, row_inc, col_inc, vert, horz, row);
        appendToVector(m_vertData, lastVert);
        appendToVector(m_vertData, normal);
        std::vector<float> uv = {1.0f, float(row) / float(m_param1)};
        appendToVector(m_vertData, uv);
        top_left[vert] += row_inc;
        m_createdface = true;
    }
    m_createdface = true;
}

void Cube::chooseSide(Side side) {
    //call makeSide for each side
    std::vector<float> top_left;
    std::vector<float> normal;
    if (side == XNEG) {
        top_left = {-m_halfside,m_halfside,-m_halfside};
        normal = {-1,0,0};
        makeSide(side, top_left, normal, Direction::CUBE_Y, Direction::CUBE_Z);
    } else if (side == XPOS) {
        top_left = {m_halfside,m_halfside,m_halfside};
        normal = {1,0,0};
        makeSide(side, top_left, normal, Direction::CUBE_Y, Direction::CUBE_Z);
    } else if (side == YNEG) {
        top_left= {-m_halfside,-m_halfside,m_halfside};
        normal = {0,-1,0};
        makeSide(side, top_left, normal, Direction::CUBE_Z, Direction::CUBE_X);
    } else if (side == YPOS) {
        top_left = {-m_halfside,m_halfside,-m_halfside};
        normal = {0,1,0};
        makeSide(side, top_left, normal, Direction::CUBE_Z, Direction::CUBE_X);
    } else if (side == ZNEG) {
        top_left = {m_halfside,m_halfside,-m_halfside};
        normal = {0,0,-1};
        makeSide(side, top_left, normal, Direction::CUBE_Y, Direction::CUBE_X);
    } else {
        top_left = {-m_halfside,m_halfside,m_halfside};
        normal = {0,0,1};
        makeSide(side, top_left, normal, Direction::CUBE_Y, Direction::CUBE_X);
    }
}
