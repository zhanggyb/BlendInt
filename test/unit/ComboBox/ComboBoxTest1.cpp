#include "ComboBoxTest1.hpp"
#include <BlendInt/Gui/ComboBox.hpp>
#include <BlendInt/Gui/FileSystemModel.hpp>

using namespace BlendInt;

ComboBoxTest1::ComboBoxTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ComboBoxTest1::~ComboBoxTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ComboBoxTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ComboBox - Foo1", 640, 480);

	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    ComboBox* combo = Manage(new ComboBox);

    combo->SetPosition(200, 20);

    FileSystemModel* fs = new FileSystemModel;
    fs->Load(getenv("PWD"));

    RefPtr<AbstractItemModel> model(fs);

    combo->SetModel(model);

    context->Append(combo);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
