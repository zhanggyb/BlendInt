/**
 * BlendInt demo
 */

#include <iostream>

#include <Cpp/Events.hpp>
#include <BlendInt/Interface.hpp>
#include <BlendInt/Types.hpp>

#include "Window.hpp"
#include "GLFWContext.hpp"

using namespace BlendInt;
using namespace std;

namespace BI = BlendInt;

int main(int argc, char* argv[])
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("GLFW3 Demo", 1024, 728);

	GLFWContext* context = Manage(new GLFWContext);
#ifdef DEBUG
	context->set_name("GLFWContext");
#endif
	Interface::instance->SetCurrentContext(context);
	context->Resize(1024, 728);

	//Interface::instance->Resize(800, 600);

	RunLoop (win);

	Interface::Release();

	Terminate();

	return 0;
}
