#include "TextEntryTest1.hpp"
#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/Label.hpp>
#include <BlendInt/Gui/Dialog.hpp>
#include <BlendInt/Gui/Context.hpp>

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
	SetContext(context);
	context->Resize(640, 480);

	Dialog* dialog = Manage(new Dialog);

    TextEntry* lineedit = Manage(new TextEntry);
 	lineedit->MoveTo(100, 100);
    
    lineedit->SetText("Hello World!");

    dialog->AddWidget(lineedit);

    context->AddFrame(dialog);

	RunLoop(window);
	Terminate();

	ASSERT_TRUE(true);
}
