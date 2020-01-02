#include "view.h"
#include "lib/FileLoader.h"
#include "shapes/Cylinder.h"
#include "shapes/SpecialShape1.h"
#include "gl/GLDebug.h"
#include "glm.hpp"
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

#include "viewformat.h"
#include <QApplication>
#include <QKeyEvent>
#include <iostream>
#include <Settings.h>

View::View(QWidget *parent) : QGLWidget(ViewFormat(), parent),
    m_time(), m_timer(), time_elapsed(0), m_captureMouse(false),
    m_shape(nullptr), phong_shader(nullptr), quad_shader(nullptr),
    W_pressed(false), A_pressed(false), S_pressed(false), D_pressed(false),
    J_pressed(false), K_pressed(false), I_pressed(false), L_pressed(false)
{
    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(true);

    // Hide the cursor
    if (m_captureMouse) {
        QApplication::setOverrideCursor(Qt::BlankCursor);
    }

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The update loop is implemented using a timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));

    //set up camera parameters
    position = glm::vec3(10,4,0);
    look = glm::vec3(-1,0,0);
    up = glm::vec3(0,1,0);
    right = glm::vec3(0,0,1);
}

View::~View()
{
}

void View::initializeGL() {
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.
    //initialize glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit(); //Causes invalid enum error
    checkError();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cerr << "Something is very wrong, glew initialization failed." << std::endl;
    }
    std::cout << "Using GLEW " <<  glewGetString( GLEW_VERSION ) << std::endl;
    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    m_time.start();
    m_timer.start(1000 / 60);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    std::string phong_vertexSource = FileLoader::loadResourceFileToString(":/shaders/phong.vert");
    std::string phong_fragmentSource = FileLoader::loadResourceFileToString(":/shaders/phong.frag");
    phong_shader = std::make_shared<Shader>(phong_vertexSource, phong_fragmentSource);

    std::string quad_vertexSource = FileLoader::loadResourceFileToString(":/shaders/quad.vert");
    std::string quad_fragmentSource = FileLoader::loadResourceFileToString(":/shaders/quad.frag");
    quad_shader = std::make_unique<Shader>(quad_vertexSource, quad_fragmentSource);

    CS123::GL::checkError();
    CS123::GL::checkError();
    m_shape = std::make_unique<SpecialShape1>(100,100,10, true);
    CS123::GL::checkError();

    std::vector<GLfloat> quadData = {-1.0f,1.0f, 0.0f,
                                     0.0f, 0.0f,
                                     -1.0f,-1.0f,0.0f,
                                     0.0, 1.0f,
                                     1.0f, 1.0f, 0.0f,
                                     1.0f, 0.0f,
                                     1.0f, -1.0f, 0.0f,
                                     1.0f, 1.0f};
    m_quad = std::make_unique<OpenGLShape>(true);
    m_quad->setVertexData(quadData, quadData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 4);
    m_quad->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->setAttribute(ShaderAttrib::TEXCOORD0, 2, 3*sizeof(GLfloat), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->buildVAO();

    /*
    tree_builder = std::make_unique<LSystemBuilder>(2);
    tree_builder->setAxiom("B(x)");
    tree_builder->addProduction("A","F(x)A(%0,%1)X(12)",true, 2);
    tree_builder->addProduction("B","F(1)A(%0,%0)Z(6)B(x)Y(27)",true, 1);
    std::cout << tree_builder->buildString() << std::endl;
    std::cout << "check" << std::endl;
    */

    tree_builder = std::make_unique<LSystemBuilder>(10);
    tree_builder->setAxiom("A(1.0,0.1)");
    tree_builder->addProduction("A","!(%1)F(%0)[&(c)B(%0*b,%1*f)]/(e)A(%0*a,%1*f)",true, 2);
    tree_builder->addProduction("B","!(%1)F(%0)[-(d)$C(%0*b,%1*f)]C(%0*a,%1*f)",true, 2);
    tree_builder->addProduction("C","!(%1)F(%0)[+(d)$B(%0*b,%1*f)]B(%0*a,%1*f)",true, 2);
    tree_builder->addParameter("a", .9);
    tree_builder->addParameter("b", .7);
    tree_builder->addParameter("c", 30);
    tree_builder->addParameter("d", -30);
    tree_builder->addParameter("e", 137.5);
    tree_builder->addParameter("f", .707);
    treestring = tree_builder->buildString();
    std::cout << treestring << std::endl;
    std::cout << treestring.size() << std::endl;
    std::cout << treestring.max_size() << std::endl;
    std::cout << "check" << std::endl;

    /*
    tree_builder = std::make_unique<LSystemBuilder>(10);
    tree_builder->setAxiom("A(1.0,0.1)");
    tree_builder->addProduction("A","!(%1)F(%0)[&(c)B(%0*a,%1*e)]/(180)[&(d)B(%0*b,%1*e)]",true, 2);
    tree_builder->addProduction("B","!(%1)F(%0)[+(c)$B(%0*a,%1*e)][-(d)$B(%0*b,%1*e)]",true, 2);
    tree_builder->addParameter("a", .9);
    tree_builder->addParameter("b", .8);
    tree_builder->addParameter("c", 35);
    tree_builder->addParameter("d", 35);
    tree_builder->addParameter("e", .707);
    treestring = tree_builder->buildString();
    std::cout << treestring << std::endl;
    std::cout << treestring.size() << std::endl;
    std::cout << treestring.max_size() << std::endl;
    std::cout << "check" << std::endl;
    */
    /*
    tree_builder = std::make_unique<LSystemBuilder>(6);
    tree_builder->setAxiom("!(.01)F(2)/(45)A");
    tree_builder->addProduction("A","!(.03)F(.5)[&(c)F(.5)A]/(a)[&(c)F(.5)A]/(b)[&(c)F(.5)A]",false, 0);
    tree_builder->addProduction("F","F(%0*d)",true, 1);
    tree_builder->addProduction("!","!(%0*e)",true, 1);
    tree_builder->addParameter("a", 94.74);
    tree_builder->addParameter("b", 132.63);
    tree_builder->addParameter("c", 18.95);
    tree_builder->addParameter("d", 1.109);
    tree_builder->addParameter("e", 1.732);
    treestring = tree_builder->buildString();
    std::cout << treestring << std::endl;
    std::cout << treestring.size() << std::endl;
    std::cout << treestring.max_size() << std::endl;
    std::cout << "check" << std::endl;
    */

    tree_renderer = std::make_unique<LSystemRenderer>(phong_shader, glm::vec3(.03,-.3,.02));
}

void View::paintGL() {

    // TODO: Implement the demo rendering here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_view = glm::lookAt(position, position + look, up);
    phong_shader->bind();
    checkError();
    phong_shader->setUniform("view", m_view);
    phong_shader->setUniform("projection", m_projection);
    tree_renderer->renderTree(treestring);
    phong_shader->unbind();


}

void View::resizeGL(int w, int h) {
    float ratio = static_cast<QGuiApplication *>(QCoreApplication::instance())->devicePixelRatio();
    w = static_cast<int>(w / ratio);
    h = static_cast<int>(h / ratio);
    m_screenwidth = w;
    m_screenheight = h;
    m_view = glm::lookAt(position, position + look, up);
    m_projection = glm::perspective(0.8f, (float)width()/height(), 0.1f, 100.f);
    glViewport(0, 0, w, h);
}

void View::mousePressEvent(QMouseEvent *event) {

}

void View::mouseMoveEvent(QMouseEvent *event) {
    // This starter code implements mouse capture, which gives the change in
    // mouse position since the last mouse movement. The mouse needs to be
    // recentered after every movement because it might otherwise run into
    // the edge of the screen, which would stop the user from moving further
    // in that direction. Note that it is important to check that deltaX and
    // deltaY are not zero before recentering the mouse, otherwise there will
    // be an infinite loop of mouse move events.
    if(m_captureMouse) {
        int deltaX = event->x() - width() / 2;
        int deltaY = event->y() - height() / 2;
        if (!deltaX && !deltaY) return;
        QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

        // TODO: Handle mouse movements here
        look = glm::normalize((glm::rotate(-float(.0025*deltaX), up)*glm::vec4(look,0)).xyz());
        right = glm::normalize(glm::cross(up, look));
        look = glm::normalize((glm::rotate(float(.0025*deltaY), right)*glm::vec4(look,0)).xyz());
        std::cout << deltaX << std::endl;
    }
}

void View::mouseReleaseEvent(QMouseEvent *event) {

}

void View::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) QApplication::quit();

    // TODO: Handle keyboard presses here
    if (event->key() == Qt::Key_W) {
        W_pressed = true;
        S_pressed = false;
    }
    if (event->key() == Qt::Key_S) {
        S_pressed = true;
        W_pressed = false;
    }
    if (event->key() == Qt::Key_A) {
        A_pressed = true;
        D_pressed = false;
    }
    if (event->key() == Qt::Key_D) {
        D_pressed = true;
        A_pressed = false;
    }
    if (event->key() == Qt::Key_I) {
        I_pressed = true;
        K_pressed = false;
    }
    if (event->key() == Qt::Key_K) {
        K_pressed = true;
        I_pressed = false;
    }
    if (event->key() == Qt::Key_J) {
        J_pressed = true;
        L_pressed = false;
    }
    if (event->key() == Qt::Key_L) {
        L_pressed = true;
        J_pressed = false;
    }
}

void View::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_W)
        W_pressed = false;
    if (event->key() == Qt::Key_S)
        S_pressed = false;
    if (event->key() == Qt::Key_A)
        A_pressed = false;
    if (event->key() == Qt::Key_D)
        D_pressed = false;
    if (event->key() == Qt::Key_I)
        I_pressed = false;
    if (event->key() == Qt::Key_J)
        J_pressed = false;
    if (event->key() == Qt::Key_K)
        K_pressed = false;
    if (event->key() == Qt::Key_L)
        L_pressed = false;
}

void View::tick() {
    // Get the number of seconds since the last tick (variable update rate)
    float seconds = m_time.restart() * 0.001f;
    time_elapsed = seconds;

    if (J_pressed) {
        look = glm::normalize((glm::rotate(float(.06), up)*glm::vec4(look,0)).xyz());
        right = glm::normalize(glm::cross(up, look));
    } else if (L_pressed) {
        look = glm::normalize((glm::rotate(-float(.06), up)*glm::vec4(look,0)).xyz());
        right = glm::normalize(glm::cross(up, look));
    }
    if (I_pressed) {
        look = glm::normalize((glm::rotate(-float(.06), right)*glm::vec4(look,0)).xyz());
    } else if (K_pressed) {
        look = glm::normalize((glm::rotate(float(.06), right)*glm::vec4(look,0)).xyz());
    }
    // TODO: Implement the demo update here
    if (W_pressed) {
         //position = position + float(.1)*glm::normalize(glm::vec3(look.xy(),0));

             position = position + float(.2)*look;
             //position.y = 0.1f;

         //printf("%f, %f, %f\n", position.x, position.y, position.z);
     } else if (S_pressed) {
         //position = position + float(.1)*glm::normalize(glm::vec3(look.xy(),0));

             position = position - float(.2)*look;

             //position.y = 0.1f;

     }
     if (A_pressed) {
         //position = position + float(.1)*glm::normalize(glm::vec3(right.xy(),0));
         position = position + float(.2)*right;
         //position.y = 0.1f;

     } else if (D_pressed) {
         //position = position + float(.1)*glm::normalize(glm::vec3(right.xy(),0));
         position = position - float(.2)*right;
         //position.y = 0.1f;

     }
    // Flag this view for repainting (Qt will call paintGL() soon after)
    char buffer [50];
    sprintf(buffer, "pos: (%f, %f, %f)",position.x, position.y, position.z);
    std::cout << buffer << std::endl;
    sprintf(buffer, "look: (%f, %f, %f)",look.x, look.y, look.z);
    std::cout << buffer << std::endl;

    update();
}
