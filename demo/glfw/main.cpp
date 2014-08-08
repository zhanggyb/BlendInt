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

	Splitter* splitter = Manage(new Splitter);
#ifdef DEBUG
	splitter->set_name("Splitter");
#endif
	splitter->SetPosition(50, 50);
	splitter->Resize(450, 200);

	Button* btn1 = Manage(new Button("Button1"));
	//Button* btn2 = Manage(new Button("Button2"));
	//Button* btn3 = Manage(new Button("Button3"));

	splitter->PushBack(btn1);
	//splitter->PushBack(btn2);
	//splitter->PushBack(btn3);

	context->PushBack(splitter);

	splitter->Resize(400, 200);
	splitter->SetPosition(100, 100);

	RunLoop(win);
	Interface::Release();
	Terminate();

	return 0;
}

