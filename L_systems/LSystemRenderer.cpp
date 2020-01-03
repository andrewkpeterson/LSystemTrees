#include "LSystemRenderer.h"
#include "glm/gtx/transform.hpp"

LSystemRenderer::LSystemRenderer(std::shared_ptr<CS123::GL::Shader> shader, glm::vec3 tropism) :
    m_shader(shader),
    cylinder(std::make_unique<Cylinder>(1,20,1,true)),
    tropism_vec(tropism),
    branch(std::make_unique<Branch>(1,20,1,true,.8,.6)),
    leaf(std::make_unique<Leaf>(10,10,1,true,.8,.6))
{
    symbols = {"!", "F", "+", "-", "&", "^", "#", "/", "|", "[", "]", "$"};
    current_state.position = glm::vec3(0,0,0);
    current_state.orientation.H = glm::vec3(0,1,0);
    current_state.orientation.L = glm::vec3(0,0,1);
    current_state.orientation.U = glm::vec3(1,0,0);
}

LSystemRenderer::~LSystemRenderer() {

}

/**
 * Renders the tree using treestring. The symbols in treestring that can
 * actually affect the structure of the tree are (see pgs. 7, 19, 24, 46, and 57
 * of The Algorithmic Beatuy of Plants:
 * -> !(x), which tells us to set the cylinder width to x
 * -> F(x), which tells us to draw a cylinder of length x
 * -> +(x), which tells us to turn left (about up vector) by angle x
 * -> -(x), which tells us to turn right (about up vector) by angle x
 * -> &(x), which tells us to pitch down (about left vector) by angle x
 * -> ^(x), which tells us to pitch up (about left vector) by angle x
 * -> #(x), which tells us to roll left (about heading vector) by angle x
 * -> /(x), which tells us to roll right (about heading vector) by angle x
 * -> |, which tells us to turn around 180 degrees about up vector
 * -> [, push the current state of the turtle on the stack
 * -> ], pop off the top state of the stack and set it to the current state
 * -> $, roll the turtle about the heading so that the the left vector is horizontal
 */
void LSystemRenderer::renderTree(std::string treestring) {
    //cylinder = std::make_unique<Cylinder>(1,20,1,true);
    current_state.position = glm::vec3(0,0,0);
    current_state.orientation.H = glm::vec3(0,1,0);
    current_state.orientation.L = glm::vec3(0,0,1);
    current_state.orientation.U = glm::vec3(1,0,0);
    std::string curr_str = treestring;
    bool symbols_to_process = true;
    while (symbols_to_process) {
        int symbol_idx = findFirstOccurence(curr_str);
        if (symbol_idx != -1) {
            std::string symbol = curr_str.substr(symbol_idx, 1);
            if (symbol.compare("|") == 0  || symbol.compare("[") == 0 || symbol.compare("]") == 0 || symbol.compare("$") == 0) {
                curr_str = curr_str.substr(symbol_idx + 1);
                processSymbol(symbol, -1, false);
            } else {
                curr_str = curr_str.substr(symbol_idx + 2);
                int length_of_num = curr_str.find(")");
                std::string float_string = curr_str.substr(0, length_of_num);
                float num = std::strtof(float_string.c_str(), nullptr);
                curr_str = curr_str.substr(length_of_num + 1);
                bool terminal_node = checkTerminalNode(curr_str);
                processSymbol(symbol, num, terminal_node);
            }
        } else {
            symbols_to_process = false;
        }
    }
}

bool LSystemRenderer::checkTerminalNode(std::string str) {
    int next_branch = str.find("F");
    for (int i = next_branch - 1; i >= 0; i--) {
        if (str.substr(i, 1).compare("]") == 0) {
            return true;
        } else if (str.substr(i, 1).compare("[") == 0) {
            return false;
        }
    }
    return false;
}

void LSystemRenderer::processSymbol(std::string symbol, float arg, bool terminal_node) {
    if (symbol.compare("!") == 0) {
        updateCylinderWidth(arg);
    } else if (symbol.compare("F") == 0) {
        drawBranch(arg, terminal_node);
    } else if (symbol.compare("+") == 0) {
        rotate(symbol, arg);
    } else if (symbol.compare("-") == 0) {
        rotate(symbol, arg);
    } else if (symbol.compare("&") == 0) {
        rotate(symbol, arg);
    } else if (symbol.compare("^") == 0) {
        rotate(symbol, arg);
    } else if (symbol.compare("#") == 0) {
        rotate(symbol, arg);
    } else if (symbol.compare("/") == 0) {
        rotate(symbol, arg);
    } else if (symbol.compare("|") == 0) {
        rotate("+", 180);
    } else if (symbol.compare("[") == 0) {
        pushState();
    } else if (symbol.compare("]") == 0) {
        popState();
    } else if (symbol.compare("$") == 0) {
        alignLeftVectorWidthHorizontal();
    }
}

void LSystemRenderer::updateCylinderWidth(float width) {
    current_state.cylinder_width = width;
}

void LSystemRenderer::drawBranch(float length, bool terminal_node) {
    float width = current_state.cylinder_width;
    //glm::mat4x4 shrink_mat = glm::scale(glm::vec3(.2,.2,.2));
    glm::mat4x4 scale_mat = glm::scale(glm::vec3(width, length, width));
    glm::vec3 U = current_state.orientation.U;
    glm::vec3 H = current_state.orientation.H;
    glm::vec3 L = current_state.orientation.L;
    //To rotate the cylinder in the correct way, we use the fact that the UHL
    //coordinate frame is defined by 3 orthonormal vectors. We know that taking the
    //dot products of a point p in XYZ space with the vectors U, H, and L gives us the
    //coordinates of p in UHL space (i.e. p_UHL = ([U H L]^T)p). We know that the
    //vertices of the rotated cylinder in UHL space are the same as the unrotated
    //vertices the the cylinder in XYZ space (i.e. Rotate(p_UHL) = p_XYZ). So, we can
    //multiply the vertices of the rotated cylinder in UHL space by ([U H L]^T)^(-1) =
    //([U H L])^T)^T (because [U H L] is a orthogonal matrix). Therefore, we know that
    //[U H L] is the rotation matrix. Finally, we take the transpose of the matrix
    //below because in OpenGL, matrices are stored in column-major order.
    glm::mat4x4 rotate_mat = glm::transpose(glm::mat4x4({U.x, H.x, L.x, 0,
                                                         U.y, H.y, L.y, 0,
                                                         U.z, H.z, L.z, 0,
                                                         0,0,0,1}));
    glm::vec3 cylinder_center = current_state.position + float(.5) * length * current_state.orientation.H;
    glm::mat4x4 translate_mat =  glm::translate(glm::vec3(cylinder_center));
    glm::mat4x4 model = translate_mat * rotate_mat * scale_mat;
    m_shader->setUniform("model", model);
    branch->draw();

    //move position forward along branch
    current_state.position += length * current_state.orientation.H;

    //draw leaf
    //rotate_mat = glm::rotate(float(M_PI/2.0), glm::vec3(0,0,1));
    if (terminal_node) {
        scale_mat = glm::scale(glm::vec3(.01, .5, .05));
        translate_mat = glm::translate(glm::vec3(current_state.position));
        model = translate_mat * rotate_mat * scale_mat;
        m_shader->setUniform("model", model);
        leaf->draw();
    }

    //rotate current_state.orientation.H in direction of tropism vector
    float e = .1;
    glm::vec3 axis_of_rotation = glm::normalize(glm::cross(current_state.orientation.H, glm::normalize(tropism_vec)));
    if (!(std::isnan(axis_of_rotation.x) || std::isnan(axis_of_rotation.y) || std::isnan(axis_of_rotation.z))) {
        //only perform the tropism rotation if it is defined
        float angle = e*glm::length(axis_of_rotation);
        glm::mat4x4 Hrotation = glm::rotate(angle, axis_of_rotation);
        current_state.orientation.H = glm::normalize(glm::vec3(Hrotation * glm::vec4(current_state.orientation.H, 0)));
        current_state.orientation.L = glm::normalize(glm::vec3(Hrotation * glm::vec4(current_state.orientation.L, 0)));
        current_state.orientation.U = glm::normalize(glm::vec3(Hrotation * glm::vec4(current_state.orientation.U, 0)));
    }
}

void LSystemRenderer::rotate(std::string symbol, float angle) {
    float rads = glm::radians(angle);
    if (symbol.compare("+") == 0 || symbol.compare("-") == 0 ) {
        rads = 1 * (symbol.compare("+") == 0) * rads + -1 * (symbol.compare("-") == 0) * rads;
        glm::vec3 savedH = current_state.orientation.H;
        current_state.orientation.H = current_state.orientation.H * glm::cos(rads) - current_state.orientation.L * glm::sin(rads);
        current_state.orientation.L = savedH * glm::sin(rads) + current_state.orientation.L * glm::cos(rads);
    } else if (symbol.compare("&") == 0 || symbol.compare("^") == 0) {
        rads = 1 * (symbol.compare("&") == 0) * rads + -1 * (symbol.compare("^") == 0) * rads;
        glm::vec3 savedH = current_state.orientation.H;
        current_state.orientation.H = current_state.orientation.H * glm::cos(rads) + current_state.orientation.U * glm::sin(rads);
        current_state.orientation.U = -savedH * glm::sin(rads) + current_state.orientation.U * glm::cos(rads);
    } else {
        rads = 1 * (symbol.compare("#") == 0) * rads + -1 * (symbol.compare("/") == 0) * rads;
        glm::vec3 savedL = current_state.orientation.L;
        current_state.orientation.L = current_state.orientation.L * glm::cos(rads) + current_state.orientation.U * glm::sin(rads);
        current_state.orientation.U = -savedL * glm::sin(rads) + current_state.orientation.U * glm::cos(rads);
    }
}

void LSystemRenderer::pushState() {
    state_stack.push(current_state);
}

void LSystemRenderer::popState() {
    current_state = state_stack.top();
    state_stack.pop();
}

void LSystemRenderer::alignLeftVectorWidthHorizontal() {
    current_state.orientation.L = glm::normalize(glm::cross(glm::vec3(0,1,0), current_state.orientation.H));
    current_state.orientation.U = glm::normalize(glm::cross(current_state.orientation.H, current_state.orientation.L));
}

/**
 * Finds the first occurrence of one of the symbols that affect the
 * structure of the tree.
 */
int LSystemRenderer::findFirstOccurence(std::string str) {
    //go through every index in str until we find an occurrence of a symbol
    for (int i = 0; i < str.size(); i++) {
        std::string curr_symbol = str.substr(i, 1);
        for (int j = 0; j < symbols.size(); j++) {
            if (curr_symbol.compare(symbols[j]) == 0) {
                return i;
            }
        }
    }
    return -1;
}
