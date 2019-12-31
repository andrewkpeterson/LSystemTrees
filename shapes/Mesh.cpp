#include "Mesh.h"
#include <fstream>
#include <iostream>
#include "gl/datatype/VBOAttribMarker.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include <cstring>
#include <QString>
#include <QFile>
#include <QTextStream>

Mesh::Mesh(std::string filename) :
    OpenGLShape (true)
{
    m_uv = loadMeshFile(filename);
}

Mesh::~Mesh() {

}

bool Mesh::loadMeshFile(std::string filename) {
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;

    QFile file((QString::fromStdString(filename)));

    QTextStream stream;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        printf("Mesh file opened successfully!\n");
    } else {
        printf("Could not open the obj file!\n");
        return false;
    }

    bool has_uv = false;
    bool has_normal = false;
    while(!file.atEnd()){
        char buf[2048];
        file.readLine(buf, 2048);
        std::string line  = std::string(buf);

        if (strcmp(line.substr(0, line.find(" ", 0)).c_str(), "v") == 0) {
            glm::vec3 vertex;
            std::sscanf(line.substr(2, line.size()).c_str(), "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        } else if (strcmp(line.substr(0, line.find(" ", 0)).c_str(), "vt") == 0) {
            has_uv = true;
            glm::vec2 uv;
            sscanf(line.substr(3, line.size()).c_str(), "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        } else if (strcmp(line.substr(0, line.find(" ", 0)).c_str(), "vn" ) == 0) {
            has_normal = true;
            glm::vec3 normal;
            sscanf(line.substr(3, line.size()).c_str(), "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        } else if (strcmp(line.substr(0, line.find(" ", 0)).c_str(), "f") == 0) {
            bool double_slash = line.find("//") != std::string::npos;
            if (!has_uv && !has_normal) {
                unsigned int vertexIndex[3];
                sscanf(line.substr(2, line.size()).c_str(), "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
            } else if (!has_uv) {
                unsigned int vertexIndex[3], normalIndex[3];
                if (double_slash) {
                    sscanf(line.substr(2, line.size()).c_str(), "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0],
                                                           &vertexIndex[1], &normalIndex[1],
                                                           &vertexIndex[2], &normalIndex[2]);
                } else {
                    sscanf(line.substr(2, line.size()).c_str(), "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &normalIndex[0],
                                                        &vertexIndex[1], &normalIndex[1],
                                                        &vertexIndex[2], &normalIndex[2]);
                }
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            } else {
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                if (double_slash) {
                    sscanf(line.substr(2, line.size()).c_str(), "%d//%d//%d %d//%d//%d %d//%d//%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                                                       &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                                                       &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                } else {
                    sscanf(line.substr(2, line.size()).c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                                                 &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                }
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
        }
    }

    std::vector<float> data;
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        data.push_back(vertex[0]);
        data.push_back(vertex[1]);
        data.push_back(vertex[2]);
        if (has_normal) {
            unsigned int normalIndex = normalIndices[i];
            glm::vec3 normal = temp_normals[normalIndex - 1];
            data.push_back(normal[0]);
            data.push_back(normal[1]);
            data.push_back(normal[2]);
        }
        if (has_uv) {
            unsigned int uvIndex = uvIndices[i];
            glm::vec2 uv = temp_uvs[uvIndex - 1];
            data.push_back(uv[0]);
            data.push_back(uv[1]);
        }
    }
    num_verts = vertexIndices.size();

    setVertexData(data, data.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, calcNumVerts());
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    if (has_normal) {
        setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    }
    if (has_uv) {
        setAttribute(ShaderAttrib::TEXCOORD0, 2, 24, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    }

    buildVAO();
    return has_uv;
}

int Mesh::calcNumVerts() {
    return num_verts;
}

void Mesh::createVertVector() {

}
