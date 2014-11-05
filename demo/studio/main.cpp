/**
 * BlendInt demo
 */

#include <BlendInt/Core/Types.hpp>

#include "StudioContext.hpp"
#include "StudioWindow.hpp"

#include <BlendInt/Gui/Splitter.hpp>
#include <BlendInt/Gui/FileButton.hpp>

int main(int argc, char* argv[])
{
	using namespace BlendInt;

	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("GLFW3 Demo", 1280, 800);

	StudioContext* context = Manage (new StudioContext);
	DBG_SET_NAME(context, "Context");
	SetContext(context);
	context->Resize(1280, 800);

	RunLoop(win);
	Terminate();

	return 0;
}

