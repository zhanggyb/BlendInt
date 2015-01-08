#include "HBlockTest1.hpp"
#include <BlendInt/Gui/HBlockLayout.hpp>
#include <BlendInt/Gui/VBlockLayout.hpp>
#include <BlendInt/Gui/ToolButton.hpp>
#include <BlendInt/Gui/Button.hpp>

#include <BlendInt/Stock/Icons.hpp>

using namespace BlendInt;
using Shaders::Icons;

HBlockTest1::HBlockTest1()
: testing::Test()
{
	// TODO: add constructor code
}

HBlockTest1::~HBlockTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(HBlockTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("HBlock - Foo1", 640, 480);

    // TODO: add test code here
	Context* context = Manage (new Context);
	DBG_SET_NAME(context, "Context");
    Interface::instance->SetCurrentContext(context);

	Button* btn1 = Manage(new Button(Icons::instance->icon_16x16(0)));
	DBG_SET_NAME(btn1, "button 1");
	Button* btn2 = Manage(new Button(Icons::instance->icon_16x16(0)));
	DBG_SET_NAME(btn2, "button 2");
	Button* btn3 = Manage(new Button(Icons::instance->icon_16x16(0)));
	DBG_SET_NAME(btn3, "button 3");

	HBlockLayout* hblock = Manage(new HBlockLayout);
	
	hblock->SetPosition(200, 200);
	hblock->Append(btn1);
	hblock->Append(btn2);
	hblock->Append(btn3);
	
	hblock->Resize(hblock->GetPreferredSize());

	context->Append(hblock);
	
	ToolButton* tbtn1 = Manage(new ToolButton);
	ToolButton* tbtn2 = Manage(new ToolButton);
	
	HBlockLayout* hblock2 = Manage(new HBlockLayout);
	
	hblock2->SetPosition(400, 200);
	hblock2->Append(tbtn1);
	hblock2->Append(tbtn2);

	hblock2->Resize(hblock2->GetPreferredSize());

	context->Append (hblock2);
	
    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
