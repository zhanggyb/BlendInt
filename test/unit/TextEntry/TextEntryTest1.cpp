#include "TextEntryTest1.hpp"
#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/Label.hpp>

using namespace BlendInt;

TextEntryTest1::TextEntryTest1()
: testing::Test()
{

}

TextEntryTest1::~TextEntryTest1()
{

}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(TextEntryTest1, Foo1)
{
	Init ();
	GLFWwindow* window = CreateWindow("TextEntry - Foo1", 640, 480);

	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    TextEntry* lineedit = Manage(new TextEntry);
 	lineedit->SetPosition(200, 200);
    
    Label* label = Manage(new Label("Label"));
    label->SetPosition(420, 200);

    context->PushBack(lineedit);
    context->PushBack(label);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
