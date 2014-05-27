/**
 * BlendInt demo
 */

#include <Cpp/Events.hpp>
#include <BlendInt/Interface.hpp>
#include <BlendInt/Types.hpp>

#include "Window.hpp"

using namespace BlendInt;
using namespace std;

namespace BI = BlendInt;

int main(int argc, char* argv[])
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("Theme Editor Demo", 640, 480);

	Context* context = Manage(new Context);
	Interface::instance->SetCurrentContext(context);

	RunLoop(win);

	Interface::Release();

	Terminate();

	return 0;
}

