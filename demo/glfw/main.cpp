/**
 * BlendInt demo
 */

#include <BlendInt/Interface.hpp>

#include "GLFWDemoContext.hpp"
#include "Window.hpp"

#include <BlendInt/Gui/Splitter.hpp>

int main(int argc, char* argv[])
{
	using namespace BlendInt;

	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("GLFW3 Demo", 1280, 800);

	GLFWDemoContext* context = Manage (new GLFWDemoContext);
	DBG_SET_NAME(context, "Context");
	Interface::instance->SetCurrentContext(context);
	context->Resize(1280, 800);

    ToolBox* tb = Manage(new ToolBox);
    tb->SetPosition(100, 100);

	Expander* expander1 = Manage(new Expander("System"));
	DBG_SET_NAME(expander1, "System Expander");
	Button* btn1 = Manage(new Button);
	expander1->Setup(btn1);

	Expander* expander2 = Manage(new Expander("System Bookmarks"));
	DBG_SET_NAME(expander2, "System Bookmarks Expander");
	Button* btn2 = Manage(new Button);

	expander2->Setup(btn2);

    tb->PushBack(expander1);
    tb->PushBack(expander2);

    //tb->Resize(200, 400);

    context->PushBack(tb);

	RunLoop(win);
	Interface::Release();
	Terminate();

	return 0;
}

