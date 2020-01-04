#ifndef LSYSTEMPARSER_H
#define LSYSTEMPARSER_H

#include <memory>
#include <stack>
#include "gl/shaders/Shader.h"
#include "shapes/Cylinder.h"
#include "BranchTriangles.h"
#include "LeafTriangles.h"


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
    unsigned int square_bracket_depth;
};


class LSystemRenderer
{
public:
    LSystemRenderer(std::shared_ptr<CS123::GL::Shader> shader);
    ~LSystemRenderer();
    std::shared_ptr<OpenGLShape> renderTree(std::string treestring);

private:
    std::shared_ptr<CS123::GL::Shader> m_shader;
    TurtleState current_state;
    std::stack<TurtleState> state_stack;
    std::vector<std::string> symbols;
    std::unique_ptr<Cylinder> cylinder;
    std::unique_ptr<BranchTriangles> branch;
    std::unique_ptr<LeafTriangles> leaf;
    std::shared_ptr<OpenGLShape> tree_mesh;
    std::vector<float> mesh_data;

    int findFirstOccurence(std::string str);
    void processSymbol(std::string symbol, float arg, bool terminal_node, float nextWidth);
    void updateCylinderWidth(float width);
    void drawBranch(float length, bool terminal_node, float next_width);
    void rotate(std::string symbol, float arg);
    void pushState();
    void popState();
    void alignLeftVectorWidthHorizontal();
    bool checkTerminalNode(std::string str);
    void appendToVertVector(const std::vector<float> &vec, glm::mat4x4 model_mat, glm::mat3x3 inverse_transpose_model_mat);
    float findNextWidth(std::string curr_str);

};

#endif // LSYSTEMPARSER_H
