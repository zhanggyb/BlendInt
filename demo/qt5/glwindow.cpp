#include <GL/glew.h>

#include <iostream>

#include "glwindow.h"
#include <BIL/Interface.hpp>
#include <QMouseEvent>

GLWindow::GLWindow(QWidget *parent) :
    QGLWidget(parent), button_(0)
{
    setMouseTracking(true);
}

GLWindow::~GLWindow()
{
    if(button_)
        delete button_;

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

    button_ = new ToggleButton("Hello World!");
    button_->set_pos(200, 200);
    button_->set_font(Font("Droid Sans"));
    button_->set_round_box_type(RoundCornerAll);
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

    glFlush ();
}

void GLWindow::mouseMoveEvent (QMouseEvent* event)
{
    BILO::Interface::instance()->cursorPosEvent(event->pos().x(),
            event->pos().y());
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
