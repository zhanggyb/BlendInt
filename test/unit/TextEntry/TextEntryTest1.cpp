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
    
    context->Append(lineedit);
    lineedit->SetText("Hello World!");

	RunLoop(window);
	Terminate();
	ASSERT_TRUE(true);
}
