#include "ListViewTest1.hpp"
#include <BlendInt/Gui/ListView.hpp>
#include <BlendInt/Gui/FileSystemModel.hpp>

using namespace BlendInt;

ListViewTest1::ListViewTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ListViewTest1::~ListViewTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ListViewTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ListView - Foo1", 640, 480);

    // TODO: add test code here
	Context* context = Manage(new Context);
	DBG_SET_NAME(context, "Context");
	Interface::instance->SetCurrentContext(context);

	ListView* list = Manage(new ListView);
	list->SetPosition(100, 100);

	RefPtr<FileSystemModel> model(new FileSystemModel);
	model->Load(".");
	list->SetModel(model);

	context->Append(list);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
