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

testWidget::testWidget(int type, Traceable* parent)
	: Widget (parent)
{
	round_box_type_ = RoundBoxAll;
	set_pos (Point(100, 100));
	resize (200, 200);

	switch (type)
	{
		case 0:
			CalculateRoundBoxEdges (round_box_type_, Rect(100, 100, 200, 200), 1.0, &base_);
			break;
		case 1:
			CalculateCheckTriangle (Rect(100, 100, 200, 200), &(base_.tria1));
			break;
		case 2:
			CalculateCheckTriangle (Rect(100, 100, 200, 200), &(base_.tria2));
			break;
		case 3:
			CalculateMenuTriangle (Rect(100, 100, 200, 200), &(base_.tria1));
			break;
		default:
			break;
	}

}

void testWidget::Render(void)
{
	DrawWidgetBase(&base_);
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

void WidgetTest::widget_default_show ()
{
    Application app;

    Window win(640, 480, "640 x 480 Window", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    Widget widget;
    widget.set_parent (&win);

    // widget.set_round_box_type(RoundBoxAll);
    widget.set_pos(50, 50);
    widget.resize(200, 100);

    app.run();

    CPPUNIT_ASSERT(true);
}

void WidgetTest::testwidget_show0()
{
    Application app;

    Window win(640, 480, "640 x 480 Window", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    testWidget widget(0);
    widget.set_parent (&win);
    widget.set_pos(Point(50, 50));
    widget.resize(80, 40);

    app.run();

    CPPUNIT_ASSERT(true);
}

void WidgetTest::testwidget_show1()
{
    Application app;

    Window win(640, 480, "640 x 480 Window", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    testWidget widget(1);
    widget.set_parent (&win);

    app.run();

    CPPUNIT_ASSERT(true);
}

void WidgetTest::testwidget_show2()
{
    Application app;

    Window win(640, 480, "640 x 480 Window", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    testWidget widget(2);
    widget.set_parent (&win);

    app.run();

    CPPUNIT_ASSERT(true);
}

void WidgetTest::testwidget_show3()
{
    Application app;

    Window win(640, 480, "640 x 480 Window", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    testWidget widget(3);
    widget.set_parent (&win);

    app.run();

    CPPUNIT_ASSERT(true);
}
