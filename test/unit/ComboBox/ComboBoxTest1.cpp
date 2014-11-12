#include "ComboBoxTest1.hpp"
#include <BlendInt/Gui/ComboBox.hpp>
#include <BlendInt/Gui/FileSystemModel.hpp>
#include <Common/UnitTestContext.hpp>
#include <BlendInt/Gui/Dialog.hpp>

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

    UnitTestContext* context = Manage (new UnitTestContext);
	DBG_SET_NAME(context, "Context");
	SetContext(context);
	context->Resize(640, 480);

    ComboBox* combo = Manage(new ComboBox);
    combo->MoveTo(20, 20);

    FileSystemModel* fs = new FileSystemModel;
    fs->Load(getenv("PWD"));

    RefPtr<AbstractItemModel> model(fs);
    combo->SetModel(model);

	Dialog* dialog = Manage(new Dialog);
	dialog->Resize(400, 300);

	dialog->AddWidget(combo);

	context->AddFrame(dialog);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
