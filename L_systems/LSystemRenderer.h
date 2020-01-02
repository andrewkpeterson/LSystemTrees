#ifndef LSYSTEMPARSER_H
#define LSYSTEMPARSER_H

#include <memory>
#include <stack>
#include "gl/shaders/Shader.h"
#include "shapes/Cylinder.h"
#include "Branch.h"
#include "Leaf.h"


struct Orientation {
    //three orthonormal vectors describing the orientation of the turtle satisfying H x L = U
    glm::vec3 H; //the heading of the turtle
    glm::vec3 L; //the direction to the left of the turtle
    glm::vec3 U; //the direction up
};

struct TurtleState {
    Orientation orientation;
    glm::vec3 position;
    float cylinder_width;
};


class LSystemRenderer
{
public:
    LSystemRenderer(std::shared_ptr<CS123::GL::Shader> shader, glm::vec3 tropism);
    ~LSystemRenderer();
    void renderTree(std::string treestring);

private:
    std::shared_ptr<CS123::GL::Shader> m_shader;
    TurtleState current_state;
    std::stack<TurtleState> state_stack;
    std::vector<std::string> symbols;
    std::unique_ptr<Cylinder> cylinder;
    std::unique_ptr<Branch> branch;
    std::unique_ptr<Leaf> leaf;
    glm::vec3 tropism_vec;

    int findFirstOccurence(std::string str);
    void processSymbol(std::string symbol, float arg);
    void updateCylinderWidth(float width);
    void drawCylinder(float length);
    void rotate(std::string symbol, float arg);
    void pushState();
    void popState();
    void alignLeftVectorWidthHorizontal();

};

#endif // LSYSTEMPARSER_H
