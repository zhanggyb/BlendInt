/**
 * BlendInt demo
 */

#include <BlendInt/Core/Types.hpp>

#include "GLFWDemoContext.hpp"
#include "Window.hpp"

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
	SetContext(context);
	context->Resize(1280, 800);

	DBG_PRINT_MSG("sizeof AbstractWidget: %ld", sizeof(AbstractWidget));

	RunLoop(win);
	Terminate();

	return 0;
}

