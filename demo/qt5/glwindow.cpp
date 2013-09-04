#include <GL/glew.h>

#include <iostream>

#include "glwindow.h"
#include <BIL/Interface.hpp>
#include <BIL/ToggleButton.hpp>

GLWindow::GLWindow(QWidget *parent) :
    QGLWidget(parent)
{
}

GLWindow::~GLWindow()
{
  std::cout << "release BIL resources" << std::endl;
    BIL::Interface::release ();
}

void GLWindow::initializeGL()
{
    using namespace BIL;

    if(!Interface::initialize()) {
      exit(1);
    }
    
    Interface::instance()->resize(size().width(), size().height());

    ToggleButton button("Hello World!");
    button.set_pos(200, 200);
    button.set_font(Font("Droid Sans"));
    button.set_round_box_type(RoundBoxAll);
}

void GLWindow::resizeGL(int w, int h)
{
    BIL::Interface::instance()->resizeEvent(w, h);
}

void GLWindow::paintGL()
{
    BIL::Interface* app = BIL::Interface::instance();
    app->render();

    glColor3f(1.0, 0.2, 0.2);
    glBegin(GL_LINE_LOOP);
        glVertex3i(100, 100, 0);
        glVertex3i(600, 100, 0);
        glVertex3i(600, 400, 0);
        glVertex3i(100, 400, 0);
    glEnd();
}
