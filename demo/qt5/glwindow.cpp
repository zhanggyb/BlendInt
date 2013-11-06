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
    BlendInt::Interface::release ();
}

void GLWindow::initializeGL()
{
    using namespace BlendInt;

    if(!Interface::initialize()) {
      exit(1);
    }
    
  Interface* app = Interface::instance();
  
  app->resize(size().width(), size().height());

	Button* add_button = new Button;
	add_button->set_text("Add Button");
	add_button->set_position(600, 700);

	Button* remove_button = new Button;
	remove_button->set_text("Remove Button");
	remove_button->set_position(600, 650);

	app->bind(add_button);
	app->bind(remove_button);

	// -----------------------

	Button* reset_button = new Button;
	reset_button->set_text("Reset");
	reset_button->move(640, 300);

	app->bind(reset_button);

	ScrollView* scroll_view = new ScrollView;

	scroll_view->set_position(200, 200);
	scroll_view->set_orientation(2);
	scroll_view->resize(400, 400);

	Button* button = new Button;
	button->set_text("Hello World!");
//	button->resize(80, 600);
	button->move(205, 205);

	scroll_view->set_viewport(button);

	app->bind(scroll_view);

	Label* label = new Label("Hello World!");

	label->set_position(50, 50);

	label->set_text ("alsdkjflasdjflasfnvlkasefage");
	label->resize(80, 40);
	
	app->bind(label);
}

void GLWindow::resizeGL(int w, int h)
{
    BlendInt::Interface::instance()->resizeEvent(w, h);
}

void GLWindow::paintGL()
{
    using namespace BlendInt;
    BlendInt::Interface* app = BlendInt::Interface::instance();

    app->render();
}

void GLWindow::mouseMoveEvent (QMouseEvent* event)
{
    //std::cout << event->x() << " " << event->y() << std::endl;
    //std::cout << event->pos().x() << " " << event->pos().y() << std::endl;

    BlendInt::Interface::instance()->cursorPosEvent((double)event->x(),
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

    BlendInt::Interface::instance()->mouseButtonEvent(button, action, mods);
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

    BlendInt::Interface::instance()->mouseButtonEvent(button, action, mods);
}
