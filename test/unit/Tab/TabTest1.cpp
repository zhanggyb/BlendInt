#include "TabTest1.hpp"
#include <BlendInt/Gui/Tab.hpp>
#include <BlendInt/Gui/Label.hpp>
#include <BlendInt/Gui/Button.hpp>

using namespace BlendInt;

TabTest1::TabTest1()
: testing::Test()
{
	// TODO: add constructor code
}

TabTest1::~TabTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(TabTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("Tab - Foo1", 640, 480);

    // TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Tab* tab = Manage (new Tab);
    tab->SetPosition(100, 100);

    Label* label = Manage(new Label("Test in Stack"));
    label->SetFont(Font("Doird Sans Mono", 24));
    label->SetBackgroundColor(Color(0x4F0F7F8F));

    Button* btn = Manage (new Button);
    btn->SetRoundType(RoundNone);

    tab->Add(String("Title1"), btn);
    tab->Resize(400, 300);

    tab->Add(String("Label"), label);

    tab->Resize(500, 300);

    context->Append(tab);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
