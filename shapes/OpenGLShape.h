#ifndef OPENGLSHAPE_H
#define OPENGLSHAPE_H

#include "GL/glew.h"

#include <memory>
#include <vector>
#include "glm/glm.hpp"

#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"


namespace CS123 { namespace GL {

class VAO;

}}

using namespace CS123::GL;

class OpenGLShape {
public:
    OpenGLShape(bool uv);

    virtual ~OpenGLShape();

    /** Initialize the VBO with the given vertex data. */
    void setVertexData(std::vector<float> data, int size, VBO::GEOMETRY_LAYOUT drawMode, int numVertices);

    /** Enables the specified attribute and calls glVertexAttribPointer with the given arguments. */
    void setAttribute(GLuint index, GLuint numElementsPerVertex, int offset, VBOAttribMarker::DATA_TYPE type,
                      bool normalize);

    /** Build the VAO given the specified vertex data and atrributes */
    void buildVAO();

    /** Draw the initialized geometry. */
     virtual void draw();

    /**
     * Called to change shape data when the shape settings are changed
     */
    virtual void changeShapeSettings(int param1, int param2, float param3);

protected:
    /**
     * Appends the vector toAppend to vec
     * @param vec - the vector being appended to
     * @param toAppend - the vector being appended to vec
     */
    void appendToVector(std::vector<float> &vec, std::vector<float> toAppend);

    /**
     * Appends the vec3 toAppend to vec
     * @param vec - the vector being appended to
     * @param toAppend - the vector being appended to vec
     */
    void appendVec3ToVector(std::vector<float> &vec, glm::vec3 toAppend);

    /**
     * Does everything needed to prepare the shape for drawing, including
     * filling up the m_vertData matrix with the correct data, setting the
     * vertex attributes, and building the VAO.
     */
    void prepareShape();

    void setVerticesAndBuildVAO();

    /**
     * Fills up m_vertData with correct data
     */
    virtual void createVertVector();

    /**
     * Calculates the number of vertices in the shape
     */
    virtual int calcNumVerts();

    std::vector<float> m_vertData;              /// vector of floats containing the vertex data.
    GLsizeiptr m_size;                          /// size of the data array, in bytes.
    VBO::GEOMETRY_LAYOUT m_drawMode;            /// drawing mode
    int m_numVertices;                          /// number of vertices to be rendered
    std::vector<VBOAttribMarker> m_markers;     /// list of VBOAttribMarkers that describe how the data is laid out.
    std::unique_ptr<CS123::GL::VAO> m_VAO;      /// a wrapper for the vertex array object (VAO)
    const int m_floatsPerVertex;
    bool m_uv;

};

#endif // OPENGLSHAPE_H
