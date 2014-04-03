#include "HLayoutTest1.hpp"

#include <BlendInt/Interface.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/Widget.hpp>

#include <boost/smart_ptr.hpp>

using namespace BlendInt;

// test the default Add() method
TEST_F(HLayoutTest1, Mix1)
{
	Init();
	GLFWwindow* window = CreateWindow("Mix Layout Test 1", 640, 480);

	// add test code here

	HLayout* hlayout = new HLayout;
	hlayout->SetPosition(100, 100);

	Widget* widget1 = new Widget;
	Widget* widget2 = new Widget;
	Widget* widget3 = new Widget;

	VLayout* vlayout = new VLayout;
	vlayout->Add(Manage(widget2));
	vlayout->Add(Manage(widget3));

	hlayout->Add(Manage(widget1));
	hlayout->Add(Manage(vlayout));

	Manage(hlayout);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
