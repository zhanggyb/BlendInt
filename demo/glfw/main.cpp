/**
 * BlendInt demo
 */

#include <BlendInt/Interface.hpp>

#include "GLFWDemoContext.hpp"
#include "Window.hpp"

#include <BlendInt/Gui/Splitter.hpp>
#include <BlendInt/Gui/FileButton.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Cursor.hpp>

#include "GLFWCursor.hpp"

int main(int argc, char* argv[])
{
	using namespace BlendInt;

	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("GLFW3 Demo", 1280, 800);

	Cursor::instance->RegisterCursorType (new GLFWCursor(win));

	GLFWDemoContext* context = Manage (new GLFWDemoContext);
	DBG_SET_NAME(context, "Context");
	Interface::instance->SetCurrentContext(context);
	context->Resize(1280, 800);

	RunLoop(win);
	Interface::Release();
	Terminate();

	return 0;
}

