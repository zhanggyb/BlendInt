/**
 * BlendInt demo
 */

#include <BlendInt/Interface.hpp>

#include "GLFWDemoContext.hpp"
#include "Window.hpp"

#include <BlendInt/Gui/Widget.hpp>

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

    Container* container = Manage(new Container);
    DBG_SET_NAME(container, "Container");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container->InsertSubWidgetExt(0, widget1);
    container->InsertSubWidgetExt(1, widget2);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    delete container;

	RunLoop(win);
	Interface::Release();
	Terminate();

	return 0;
}

