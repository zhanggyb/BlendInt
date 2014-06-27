/**
 * BlendInt demo
 */

#include <BlendInt/Interface.hpp>
#include <BlendInt/Core/Types.hpp>

#include "EditorContext.hpp"
#include "Window.hpp"

using namespace BlendInt;
using namespace std;

namespace BI = BlendInt;

int main(int argc, char* argv[])
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("GLFW3 Demo", 640, 480);

	EditorContext* context = Manage (new EditorContext);
	Interface::instance->SetCurrentContext(context);

	RunLoop(win);

	Interface::Release();

	Terminate();

	return 0;
}

