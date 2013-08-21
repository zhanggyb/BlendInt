// cpp

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include <BIL/Widget.hpp>
#include "WidgetTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (WidgetTest);

myWidget1::myWidget1(Traceable* parent)
	: Widget (parent)
{
	round_box_type_ = RoundBoxAll;
}

myWidget1::~myWidget1()
{

}

void myWidget1::Render(void)
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 0.9f, 1.0f, 0.95f);

		DrawAntiRoundbox(GL_POLYGON,
				pos_.x(),
				pos_.y(),
				pos_.x() + size_.width(),
				pos_.y() + size_.height(),
				5.0, false);

		glColor4f (0.1f, 0.1f, 0.1f, 1.0f);
		DrawAntiTriangle (100.0f, 100.0f, 120.0f, 100.0f, 110.0f, 120.0f);

	glDisable(GL_BLEND);
}

myWidget2::myWidget2(Traceable* parent)
	: Widget (parent)
{
	round_box_type_ = RoundBoxAll;
	set_pos (Point(100, 100));
	resize (200, 200);
	CalculateRoundBoxEdges (round_box_type_, Rect(100, 100, 200, 200), 1.0, &base_);
	CalculateCheckTriangle (Rect(100, 100, 200, 200), &(base_.tria1));
	CalculateCheckTriangle (Rect(100, 100, 200, 200), &(base_.tria2));

}

myWidget2::~myWidget2()
{

}

void myWidget2::Render(void)
{
	DrawWidgetBase(&base_, &colors_);
}


WidgetTest::WidgetTest ()
{

}

WidgetTest::~WidgetTest ()
{

}

void WidgetTest::setUp ()
{

}

void WidgetTest::tearDown ()
{

}

void WidgetTest::mywidget1_show1()
{
    Application app;

    Window win(640, 480, "640 x 480 Window", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    myWidget1 widget;
    widget.set_parent (&win);
    widget.set_pos(Point(50, 50));
    widget.resize(80, 40);

    app.run();

    CPPUNIT_ASSERT(true);
}

void WidgetTest::mywidget2_show1()
{
    Application app;

    Window win(640, 480, "640 x 480 Window", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    myWidget2 widget;
    widget.set_parent (&win);

    app.run();

    CPPUNIT_ASSERT(true);
}
