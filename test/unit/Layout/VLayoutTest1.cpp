/*
 * VLayoutTest1.cpp
 *
 *  Created on: 2013��12��7��
 *      Author: zhanggy
 */

#include "VLayoutTest1.hpp"
#include <BlendInt/Interface.hpp>
#include <BlendInt/Gui/VBox.hpp>
#include <BlendInt/Gui/HBox.hpp>
#include <BlendInt/Gui/Widget.hpp>

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
	GLFWwindow* window = CreateWindow("Vertical Layout Test", 640, 480);

	// add test code here
	Context* context = Manage(new Context);
#ifdef DEBUG
	context->set_name("Context");
#endif
	Interface::instance->SetCurrentContext(context);

	VBox* vlayout = Manage(new VBox);

	vlayout->SetPosition(100, 100);

	Widget* widget1 = Manage(new Widget);
	Widget* widget2 = Manage(new Widget);
	Widget* widget3 = Manage(new Widget);

	HBox* hlayout = Manage(new HBox);
	hlayout->Add(widget2);
	hlayout->Add(widget3);

	vlayout->Add(widget1);
	vlayout->Add(hlayout);

	context->Add(vlayout);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
