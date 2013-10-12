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

	Slider* hslider1 = new Slider;
	hslider1->set_pos(100, 30);
	hslider1->set_name("hslider1");

	Slider* hslider2 = new Slider;
	hslider2->set_pos(100, 60);
	hslider2->set_name("hslider2");

	Slider* hslider3 = new Slider;
	hslider3->set_pos(100, 90);
	hslider3->set_name("hslider3");

	Slider* hslider4 = new Slider;
	hslider4->set_pos(100, 120);
	hslider4->set_name("hslider4");

	Slider* hslider5 = new Slider;
	hslider5->set_pos(100, 150);
	hslider5->set_name("hslider4");

	Slider* hslider6 = new Slider;
	hslider6->set_pos(100, 180);
	hslider6->set_name("hslider6");

	Slider* hslider7 = new Slider;
	hslider7->set_pos(100, 210);
	hslider7->set_name("hslider7");

	Slider* hslider8 = new Slider;
	hslider8->set_pos(100, 240);
	hslider8->set_name("hslider8");

	Slider* hslider9 = new Slider;
	hslider9->set_pos(100, 270);
	hslider9->set_name("hslider9");

	Slider* hslider10 = new Slider;
	hslider10->set_pos(100, 300);
	hslider10->set_name("hslider10");

	Slider* hslider11 = new Slider;
	hslider11->set_pos(100, 330);
	hslider11->set_name("hslider11");

	Slider* hslider12 = new Slider;
	hslider12->set_pos(100, 360);
	hslider12->set_name("hslider12");

	Slider* hslider13 = new Slider;
	hslider13->set_pos(100, 390);
	hslider13->set_name("hslider13");

	Slider* hslider14 = new Slider;
	hslider14->set_pos(100, 420);
	hslider14->set_name("hslider14");

	Slider* hslider15 = new Slider;
	hslider15->set_pos(100, 450);
	hslider15->set_name("hslider15");

	Slider* hslider16 = new Slider;
	hslider16->set_pos(100, 480);
	hslider16->set_name("hslider16");

	Slider* vslider = new Slider(Vertical);
	vslider->set_pos(50, 250);
	vslider->set_name("vslider");

//	app->bind(widget);
//	app->bind(button);
//	app->bind(toggle);

	app->bind(hslider1);
	app->bind(hslider2);
	app->bind(hslider3);
	app->bind(hslider4);
	app->bind(hslider5);
	app->bind(hslider6);
	app->bind(hslider7);
	app->bind(hslider8);
	app->bind(hslider9);
	app->bind(hslider10);
	app->bind(hslider11);
	app->bind(hslider12);
	app->bind(hslider13);
	app->bind(hslider14);
	app->bind(hslider15);
	app->bind(hslider16);

	app->bind(vslider);

	app->events().connect(hslider1->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider2->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider3->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider4->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider5->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider6->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider7->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider8->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider9->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider10->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider11->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider12->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider13->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider14->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider15->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider16->slider_moved(), vslider, &AbstractSlider::set_value);

	Frame* frame = new Frame;

	frame->resize(400, 400);
	frame->set_pos(400, 200);

	app->bind(frame);
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
   // std::cout << event->pos().x() << " " << event->pos().y() << std::endl;

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
