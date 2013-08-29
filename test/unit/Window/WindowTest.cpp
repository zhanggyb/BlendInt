// cpp

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include <BIL/Application.hpp>

#include <BIL/Window.hpp>
#include <BIL/Label.hpp>
#include <BIL/Widget.hpp>

#include "WindowTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (WindowTest);

myWindow::myWindow(int width, int height, const char *title, GLFWmonitor *monitor,
        GLFWwindow *share, Traceable* parent)
: BIL::Window( width, height, title, monitor, share, parent)
{

}

void myWindow::cursorEnterEvent(int entered)
{
	if (entered == GL_TRUE) {
		cout << "This time show message in derived class" << endl;
	}
}

myWidget::myWidget()
	: Widget (NULL)
{
	resize (200, 100);
}

myWidget::~myWidget()
{

}

void myWidget::render (void)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor4f(1.0f, 0.9f, 1.0f, 0.95f);

	drawRoundBox(pos_.x(),
				 pos_.y(),
				 pos_.x() + size_.width(),
				 pos_.y() + size_.height(),
				 5.0);
	DrawShadowBox(pos_.x(),
				  pos_.y(),
				  pos_.x() + size_.width(),
				  pos_.y() + size_.height(),
				  5.0,
				  64);

	glDisable(GL_BLEND);
}

void myWidget::mousePressEvent (MouseEvent* event)
{
	cout << "Click at" << event->pos().x() << " "
		 << event->pos().y() << endl;

	cout << "and window position: " << event->window_pos().coord.x << " "
		 << event->window_pos().coord.y << endl;

	event->Accept();
}

void myWidget::mouseMoveEvent (MouseEvent* event)
{
	if(size_.Contains(event->pos())) {
		cout << "Hover on this widget" << endl;
	}

	Widget::mouseMoveEvent(event);
}

WindowTest::WindowTest ()
{

}

WindowTest::~WindowTest ()
{

}

void WindowTest::setUp ()
{

}

void WindowTest::tearDown ()
{

}

void WindowTest::event1 ()
{
    Application app;

    Window win(640, 480, "640 x 480 WindowTest Event1", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    Label label(L"Test Event1");
    label.set_pos (Point(200, 200));
    label.setParent (&win);

    app.run();

    CPPUNIT_ASSERT(true);
}

void WindowTest::event2 ()
{
    Application app;

    myWindow win(640, 480, "640 x 480 WindowTest Event2", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    Label label(L"Test Event2");
    label.set_pos (Point(300, 200));
    label.setParent (&win);

    app.run();

    CPPUNIT_ASSERT(true);
}

void WindowTest::solorender1 ()
{
    Application app;

    myWindow win(640, 480, "640 x 480 WindowTest Event2", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    Label label(L"Test Event2");
    label.set_pos (Point(100, 100));
    label.setParent (&win);

	Label solo (L"This label should also been rendered");
	solo.set_pos(400, 300);

    app.run();

    CPPUNIT_ASSERT(true);
}

void WindowTest::check_mouse_press1 ()
{
    Application app;

    myWindow win(640, 480, "640 x 480 check mouse press", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();
	
		myWidget widget;
		widget.setParent (&win);
		widget.set_pos(300, 300);

    app.run();

    CPPUNIT_ASSERT(true);
}
