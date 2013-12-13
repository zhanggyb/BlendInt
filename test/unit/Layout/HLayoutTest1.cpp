#include "HLayoutTest1.hpp"

#include <BlendInt/Interface.hpp>
#include <BlendInt/HorizontalLayout.hpp>
#include <BlendInt/VerticalLayout.hpp>
#include <BlendInt/Widget.hpp>

#include <boost/smart_ptr.hpp>

using namespace BlendInt;

// test the default Add() method
TEST_F(HLayoutTest1, Mix1)
{
	Init();
	GLFWwindow* window = CreateWindow("Mix Layout Test 1");

	// add test code here

	HorizontalLayout* hlayout = new HorizontalLayout;
	hlayout->SetPosition(200, 200);

	Widget* widget1 = new Widget;
	Widget* widget2 = new Widget;
	Widget* widget3 = new Widget;

	VerticalLayout* vlayout = new VerticalLayout;
	vlayout->Add(widget2);
	vlayout->Add(widget3);

	hlayout->Add(widget1);
	hlayout->Add(vlayout);

	Interface::Instance()->Bind(hlayout);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
