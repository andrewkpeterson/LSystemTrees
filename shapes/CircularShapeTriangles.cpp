#include "CircularShapeTriangles.h"
#include <memory>
#include <vector>
#include "Disc.h"
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"

CircularShapeTriangles::CircularShapeTriangles(int param1, int param2, float param3) :
    OpenGLShape(true),
    m_slices(glm::max(3, param2)),
    m_rings(glm::max(1, param1))
{

}

CircularShapeTriangles::~CircularShapeTriangles()
{

}

void CircularShapeTriangles::makeCircularStrip(float lower_height, float lower_rad, float rise, float run) {
    //place strips around
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    glm::vec3 normal;
    glm::vec3 lower_vert;
    glm::vec3 upper_vert;
    float upper_height = lower_height + rise;
    float upper_rad = lower_rad - run;
    for (int slice_idx = 0; slice_idx <= m_slices; slice_idx++) {
        float theta = -slice_idx * (2 * M_PI / m_slices);
        float x, z;
        x = lower_rad * cos(theta);
        z = lower_rad * sin(theta);

        float lower_y_norm = -radiusDeriv(lower_height)*glm::sqrt(std::pow(x,2) + std::pow(z,2));
        normal = {x,lower_y_norm, z};
        normal = glm::normalize(normal);
        lower_vert = {lower_rad * cos(theta), lower_height, lower_rad * sin(theta)};
        positions.push_back(lower_vert);
        normals.push_back(normal);
        uvs.push_back(glm::vec2(1 - (lower_height + .5f), slice_idx / float(m_slices)));

        x = upper_rad * cos(theta);
        z = upper_rad * sin(theta);
        float upper_y_norm = -radiusDeriv(upper_height)*glm::sqrt(std::pow(x,2) + std::pow(z,2));
        normal = {x,upper_y_norm, z};
        normal = glm::normalize(normal);
        upper_vert = {upper_rad * cos(theta), upper_height, upper_rad * sin(theta)};
        positions.push_back(upper_vert);
        normals.push_back(normal);
        uvs.push_back(glm::vec2(1 - (upper_height + .5f), slice_idx / float(m_slices)));
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
