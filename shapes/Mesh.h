#ifndef MESH_H
#define MESH_H

#include "OpenGLShape.h"

class Mesh : public OpenGLShape
{
public:
    Mesh(std::string filename);
    ~Mesh();

protected:
    int calcNumVerts() override;
    bool loadMeshFile(std::string filename);
    void createVertVector() override;
    int num_verts;
};

#endif // MESH_H
