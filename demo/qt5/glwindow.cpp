#include <GL/glew.h>

#include <iostream>

#include "glwindow.h"
#include <BlendInt/Interface.hpp>
#include <QMouseEvent>

#include <Cpp/Events.hpp>
#include <BlendInt/Interface.hpp>
#include <BlendInt/ContextManager.hpp>

#include <BlendInt/ToggleButton.hpp>
#include <BlendInt/VerticalLayout.hpp>
#include <BlendInt/HorizontalLayout.hpp>

#include <BlendInt/Types.hpp>
#include <BlendInt/Button.hpp>
#include <BlendInt/Label.hpp>
#include <BlendInt/ScrollWidget.hpp>
#include <BlendInt/Slider.hpp>
#include <BlendInt/Frame.hpp>
#include <BlendInt/ScrollView.hpp>

GLWindow::GLWindow(QWidget *parent) :
    QGLWidget(parent)
{
    setMouseTracking(true);
}

GLWindow::~GLWindow()
{
    BlendInt::Interface::Release ();
}

void GLWindow::initializeGL()
{
    using namespace BlendInt;

    if(!Interface::Initialize()) {
      exit(1);
    }
    
  Interface* app = Interface::Instance();
  
  app->Resize(size().width(), size().height());

  ToggleButton* button = new ToggleButton;

  button->SetPosition(200, 200);
  button->Register();
}

void GLWindow::resizeGL(int w, int h)
{
    BlendInt::Interface::Instance()->Resize(w, h);
}

void GLWindow::paintGL()
{
    using namespace BlendInt;
    BlendInt::Interface* app = BlendInt::Interface::Instance();

    app->Draw();
}

void GLWindow::mouseMoveEvent (QMouseEvent* event)
{
    //std::cout << event->x() << " " << event->y() << std::endl;
    //std::cout << event->pos().x() << " " << event->pos().y() << std::endl;

    BlendInt::Interface::Instance()->GLFWCursorPosEvent((double)event->x(),
           (double) event->y());
}

void GLWindow::mousePressEvent (QMouseEvent* event)
{
    int button = BlendInt::MouseButtonNone;
    int action = BlendInt::MousePress;
    int mods = BlendInt::ModifierNone;

    switch(event->button()) {
        case Qt::LeftButton:
            button = BlendInt::MouseButtonLeft;
            break;
        case Qt::RightButton:
            button = BlendInt::MouseButtonRight;
            break;
        default:
            break;
    };

    int qt_mods = event->modifiers();
    if(qt_mods & Qt::ShiftModifier) {
        mods = mods | BlendInt::ModifierShift;
    }
    if(qt_mods & Qt::ControlModifier) {
        mods = mods | BlendInt::ModifierControl;
    }
    if(qt_mods & Qt::AltModifier) {
        mods = mods | BlendInt::ModifierAlt;
    }

    BlendInt::Interface::Instance()->GLFWMouseButtonEvent(button, action, mods);
}

void GLWindow::mouseReleaseEvent (QMouseEvent* event)
{
    int button = BlendInt::MouseButtonNone;
    int action = BlendInt::MouseRelease;
    int mods = BlendInt::ModifierNone;

    switch(event->button()) {
        case Qt::LeftButton:
            button = BlendInt::MouseButtonLeft;
            break;
        case Qt::RightButton:
            button = BlendInt::MouseButtonRight;
            break;
        default:
            break;

    };

    int qt_mods = event->modifiers();
    if(qt_mods & Qt::ShiftModifier) {
        mods = mods | BlendInt::ModifierShift;
    }
    if(qt_mods & Qt::ControlModifier) {
        mods = mods | BlendInt::ModifierControl;
    }
    if(qt_mods & Qt::AltModifier) {
        mods = mods | BlendInt::ModifierAlt;
    }

    BlendInt::Interface::Instance()->GLFWMouseButtonEvent(button, action, mods);
}
