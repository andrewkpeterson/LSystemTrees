#ifndef VIEW_H
#define VIEW_H

#include "GL/glew.h"
#include <qgl.h>
#include <QTime>
#include <QTimer>
#include <QSettings>
#include "shapes/OpenGLShape.h"
#include "shapes/Mesh.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include "gl/shaders/Shader.h"
#include "L_systems/LSystemBuilder.h"
#include "L_systems/LSystemRenderer.h"

class View : public QGLWidget {
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

private:
    QTime m_time;
    QTimer m_timer;
    float time_elapsed;
    bool m_captureMouse;

    std::unique_ptr<OpenGLShape> m_shape;
    std::unique_ptr<OpenGLShape> m_quad;

    std::shared_ptr<Shader> phong_shader;
    std::unique_ptr<Shader> quad_shader;

    std::unique_ptr<LSystemBuilder> tree_builder;
    std::unique_ptr<LSystemRenderer> tree_parser;

    glm::vec3 position;
    glm::vec3 look;
    glm::vec3 up;
    glm::vec3 right;
    glm::mat4x4 m_projection;
    glm::mat4x4 m_view;

    int m_screenwidth;
    int m_screenheight;

    bool W_pressed;
    bool A_pressed;
    bool S_pressed;
    bool D_pressed;
    bool I_pressed;
    bool J_pressed;
    bool K_pressed;
    bool L_pressed;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void tick();
};

#endif // VIEW_H
