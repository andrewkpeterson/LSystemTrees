#include "OpenGLShape.h"

#include "gl/datatype/VAO.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include <iostream>

using namespace CS123::GL;

OpenGLShape::OpenGLShape(bool uv) :
    m_size(0),
    m_drawMode(VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES),
    m_numVertices(0),
    m_VAO(nullptr),
    m_floatsPerVertex(6),
    m_uv(uv)
{
}

OpenGLShape::~OpenGLShape()
{
}

void OpenGLShape::prepareShape() {
    m_vertData.reserve(calcNumVerts() * m_floatsPerVertex);
    createVertVector();
    setVerticesAndBuildVAO();
}

void OpenGLShape::setVerticesAndBuildVAO() {
    setVertexData(m_vertData, m_vertData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, calcNumVerts());
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    if (m_uv) {
        setAttribute(ShaderAttrib::TEXCOORD0, 2, 24, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    }
    buildVAO();
}

void OpenGLShape::changeShapeSettings(int param1, int param2, float param3) {
    m_size = 0;
    m_numVertices = 0;
    m_VAO = nullptr;
    m_vertData.clear();
}


/**
 * @param data - Vector of floats containing the vertex data.
 * @param size - number of elements in the array.
 * @param drawMode - Drawing mode
 * @param numVertices - Number of vertices to be rendered.
 */
void OpenGLShape::setVertexData(std::vector<float> data, int size, VBO::GEOMETRY_LAYOUT drawMode, int numVertices) {
    m_vertData = data;
    m_size = size;
    m_drawMode = drawMode;
    m_numVertices = numVertices;
}


/**
 * @param name OpenGL handle to the attribute location. These are specified in ShaderAttribLocations.h
 * @param numElementsPerVertex Number of elements per vertex. Must be 1, 2, 3 or 4 (e.g. position = 3 for x,y,z)
 * @param offset Offset in BYTES from the start of the array to the beginning of the first element
 * @param type Primitive type (FLOAT, INT, UNSIGNED_BYTE)
 * @param normalize
 */
void OpenGLShape::setAttribute(GLuint name, GLuint numElementsPerVertex, int offset,
                               VBOAttribMarker::DATA_TYPE type, bool normalize) {
    m_markers.push_back(VBOAttribMarker(name, numElementsPerVertex, offset, type, normalize));
}

void OpenGLShape::buildVAO() {
    CS123::GL::VBO vbo = VBO(m_vertData.data(), m_size, m_markers, m_drawMode);
    m_VAO = std::make_unique<VAO>(vbo, m_numVertices);
}

void OpenGLShape::appendToVector(std::vector<float> &vec, std::vector<float> toAppend) {
    for (int i = 0; i < toAppend.size(); ++i) {
        vec.push_back(toAppend[i]);
    }
}

void OpenGLShape::appendVec3ToVector(std::vector<float> &vec, glm::vec3 toAppend) {
    for (int i = 0; i < 3; ++i) {
        if (glm::abs(toAppend[1]) < .001) {
            i++;
            i--;
        }
        vec.push_back(toAppend[i]);
    }
}

void OpenGLShape::draw() {
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}


void OpenGLShape::createVertVector() {

}


int OpenGLShape::calcNumVerts() {

}
