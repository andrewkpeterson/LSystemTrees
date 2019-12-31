#ifndef CIRCULARSHAPE_H
#define CIRCULARSHAPE_H

#include "OpenGLShape.h"
#include <memory>
#include <vector>
#include "Disc.h"
#include <GL/glew.h>
#include "GL/glew.h"
#include "glm/glm.hpp"

/**
 * Class representing arbitrary solid of revolution about the y-axis
 */
class CircularShape : public OpenGLShape
{
public:
    CircularShape(int param1, int param2, float param3, bool sceneview, bool uv);
    virtual ~CircularShape();
    virtual void changeShapeSettings(int param1, int param2, float param3) override;

protected:
    /**
     * Creates a strip with circular symmetry about the y-axis
     * @param lower_height - the height of the lower edge of the strip
     * @param lower_rad - the radius of the lower edge of the strip
     * @param rise - the vertical height of the strip
     * @param run - the change in radius over the strip
     */
    virtual void makeCircularStrip(float lower_rad, float curr_rad, float rise, float run);

    /**
     * Function relating the y_position of a vertex to its radius from the y axis
     * @param y_pos - the y position
     */
    virtual float radiusFunc(float y_pos) = 0;

    /**
     * Derivative of function relating the y_position of a vertex to its radius from the y axis
     * @param y_pos - the y position
     */
    virtual float radiusDeriv(float y_pos) = 0;

    bool made_first_ring;
    int m_slices;
    int m_rings;
    bool m_sceneview;
};

#endif // CIRCULARSHAPE_H
