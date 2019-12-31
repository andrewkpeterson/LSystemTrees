#ifndef CUBE_H
#define CUBE_H

#include "OpenGLShape.h"
#include <memory>
#include <vector>
#include <GL/glew.h>
#include "GL/glew.h"

enum Side {XPOS, YPOS, ZPOS, XNEG, YNEG, ZNEG};
enum Direction {CUBE_X, CUBE_Y, CUBE_Z};

class Cube : public OpenGLShape {
public:
    Cube(int param1, int param2, float param3);
    ~Cube();
    void changeShapeSettings(int param1, int param2, float param3) override;

private:
    int calcNumVerts() override;

    /**
     * Creates a triangle strip along a specific face of the cube
     * @param top_left - the top-left vertex of the face being created
     * @param normal - normal vector to current face
     * @param row_inc - distance increment moving from row to row
     * @param col_inc - distance increment moving from column to column
     * @param vert - "vertical" direction of the face
     * @param horz - "horizontal" direction of the face
     * @return the coordinates of the last vertex in the strip
     */
    std::vector<float> makeStrip(std::vector<float> top_left, std::vector<float> normal, float row_inc,
                                 float col_inc, Direction vert, Direction horz, int row);

    /**
     * Creates a triangle strip along a specific face of the cube
     * @param top_left - the top-left vertex of the face being created
     * @param normal - normal vector to current face
     * @param vert - "vertical" direction of the face
     * @param horz - "horizontal" direction of the face
     */
    void makeSide(Side side, std::vector<float> top_left, std::vector<float> normal, Direction vert, Direction horz);

    /**
     * Calls makeSide() with the correct parameters for the particular
     * side being created
     * @param side - the side of the cube being created
     */
    void chooseSide(Side side);
    void createVertVector() override;

    const float m_halfside;
    const int m_numsides;
    bool m_createdface;
    int m_param1;
};

#endif // CUBE_H
