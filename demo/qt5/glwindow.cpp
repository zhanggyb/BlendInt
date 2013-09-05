#include <GL/glew.h>

#include <iostream>

#include "glwindow.h"
#include <BIL/Interface.hpp>
#include <QMouseEvent>

GLWindow::GLWindow(QWidget *parent) :
    QGLWidget(parent), button_(0)
{
}

GLWindow::~GLWindow()
{
    if(button_)
        delete button_;

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

    button_ = new ToggleButton("Hello World!");
    button_->set_pos(200, 200);
    button_->set_font(Font("Droid Sans"));
    button_->set_round_box_type(RoundBoxAll);
}

void GLWindow::resizeGL(int w, int h)
{
    BIL::Interface::instance()->resizeEvent(w, h);
}

void GLWindow::paintGL()
{
    using namespace BIL;
    BIL::Interface* app = BIL::Interface::instance();

    app->render();
}

void GLWindow::mouseMoveEvent (QMouseEvent* event)
{
    BIL::Interface::instance()->cursorPosEvent(event->windowPos().toPoint().x(),
            event->windowPos().toPoint().y());
}

void GLWindow::mousePressEvent (QMouseEvent* event)
{
	
}

void GLWindow::mouseReleaseEvent (QMouseEvent* event)
{

}
