#include "HBlockTest1.hpp"
#include <BlendInt/Gui/HBlockLayout.hpp>
#include <BlendInt/Gui/VBlockLayout.hpp>
#include <BlendInt/Gui/ToolButton.hpp>
#include <BlendInt/Gui/Button.hpp>

using namespace BlendInt;

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

	Button* btn1 = Manage(new Button);
	Button* btn2 = Manage(new Button);
	
	HBlock* hblock = Manage(new HBlock);
	
	hblock->SetPosition(200, 200);
	hblock->PushBack(btn1);
	hblock->PushBack(btn2);

	hblock->Resize(hblock->GetPreferredSize());
	
	context->PushBack(hblock);
	
	ToolButton* tbtn1 = Manage(new ToolButton);
	ToolButton* tbtn2 = Manage(new ToolButton);
	
	HBlock* hblock2 = Manage(new HBlock);
	
	hblock2->SetPosition(400, 200);
	hblock2->PushBack(tbtn1);
	hblock2->PushBack(tbtn2);

	hblock2->Resize(hblock2->GetPreferredSize());

	DBG_PRINT_MSG("preferred size: %d, %d", hblock2->size().width(), hblock2->size().height());
	
	context->PushBack (hblock2);
	
    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
