#include "SectionTest1.hpp"
#include <BlendInt/Gui/Section.hpp>
#include <BlendInt/Gui/FileSelector.hpp>

using namespace BlendInt;

SectionTest1::SectionTest1()
: testing::Test()
{
	// TODO: add constructor code
}

SectionTest1::~SectionTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(SectionTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("FileButton - Foo1", 1280, 800);

    // TODO: add test code here
	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);
    context->Resize(1280, 800);

    Button* btn = Manage(new Button("Click Me"));
    DBG_SET_NAME(btn, "Button");
    btn->SetPosition(160, 200);

    FileSelector* fs = Manage(new FileSelector);
    DBG_SET_NAME(fs, "File Selector");

    Section* section = Manage(new Section);
    DBG_SET_NAME(section, "Section");
    section->Append(fs);
    section->set_mode(Section::Modal);

    fs->SetPosition(200, 100);
    fs->Resize(800, 600);

    context->Append(btn);

	context->Append(section);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
