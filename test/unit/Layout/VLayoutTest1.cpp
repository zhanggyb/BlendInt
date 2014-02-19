/*
 * VLayoutTest1.cpp
 *
 *  Created on: 2013��12��7��
 *      Author: zhanggy
 */

#include "VLayoutTest1.hpp"
#include <BlendInt/Interface.hpp>
#include <BlendInt/Gui/VerticalLayout.hpp>
#include <BlendInt/Gui/HorizontalLayout.hpp>

using namespace BlendInt;

VLayoutTest1::VLayoutTest1()
: testing::Test()
{
	// TODO Auto-generated constructor stub

}

VLayoutTest1::~VLayoutTest1() {
	// TODO Auto-generated destructor stub
}

// test default Add() method
TEST_F(VLayoutTest1, Add1)
{
	Init ();
	GLFWwindow* window = CreateWindow("Vertical Layout Test");

	// add test code here

	VerticalLayout* vlayout = new VerticalLayout;

	vlayout->SetPosition(200, 200);

	Widget* widget1 = new Widget;
	Widget* widget2 = new Widget;
	Widget* widget3 = new Widget;

	HorizontalLayout* hlayout = new HorizontalLayout;
	hlayout->Add(widget2);
	hlayout->Add(widget3);

	vlayout->Add(widget1);
	vlayout->Add(hlayout);

    vlayout->Register();

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
