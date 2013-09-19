#include <GL/glew.h>

#include <iostream>

#include "glwindow.h"
#include <BILO/Interface.hpp>
#include <QMouseEvent>

GLWindow::GLWindow(QWidget *parent) :
    QGLWidget(parent), m_slider(0)
{
    setMouseTracking(true);
}

GLWindow::~GLWindow()
{
    if(m_slider)
        delete m_slider;

  std::cout << "release BILO resources" << std::endl;
    BILO::Interface::release ();
}

void GLWindow::initializeGL()
{
    using namespace BILO;

    if(!Interface::initialize()) {
      exit(1);
    }
    
    Interface::instance()->resize(size().width(), size().height());

    m_slider = new Slider(BILO::Horizontal);
    m_slider->set_pos(200, 200);
}

void GLWindow::resizeGL(int w, int h)
{
    BILO::Interface::instance()->resizeEvent(w, h);
}

void GLWindow::paintGL()
{
    using namespace BILO;
    BILO::Interface* app = BILO::Interface::instance();

    app->render();
    
    swapBuffers();
}

void GLWindow::mouseMoveEvent (QMouseEvent* event)
{
   //std::cout << event->x() << " " << event->y() << std::endl;
   // std::cout << event->pos().x() << " " << event->pos().y() << std::endl;

    BILO::Interface::instance()->cursorPosEvent((double)event->x(),
           (double) event->y());
}

void GLWindow::mousePressEvent (QMouseEvent* event)
{
    int button = BILO::MouseButtonNone;
    int action = BILO::MousePress;
    int mods = BILO::ModifierNone;

    switch(event->button()) {
        case Qt::LeftButton:
            button = BILO::MouseButtonLeft;
            break;
        case Qt::RightButton:
            button = BILO::MouseButtonRight;
        default:
            break;

    };

    int qt_mods = event->modifiers();
    if(qt_mods & Qt::ShiftModifier) {
        mods = mods | BILO::ModifierShift;
    }
    if(qt_mods & Qt::ControlModifier) {
        mods = mods | BILO::ModifierControl;
    }
    if(qt_mods & Qt::AltModifier) {
        mods = mods | BILO::ModifierAlt;
    }

    BILO::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void GLWindow::mouseReleaseEvent (QMouseEvent* event)
{
    int button = BILO::MouseButtonNone;
    int action = BILO::MouseRelease;
    int mods = BILO::ModifierNone;

    switch(event->button()) {
        case Qt::LeftButton:
            button = BILO::MouseButtonLeft;
            break;
        case Qt::RightButton:
            button = BILO::MouseButtonRight;
        default:
            break;

    };

    int qt_mods = event->modifiers();
    if(qt_mods & Qt::ShiftModifier) {
        mods = mods | BILO::ModifierShift;
    }
    if(qt_mods & Qt::ControlModifier) {
        mods = mods | BILO::ModifierControl;
    }
    if(qt_mods & Qt::AltModifier) {
        mods = mods | BILO::ModifierAlt;
    }

    BILO::Interface::instance()->mouseButtonEvent(button, action, mods);
}
