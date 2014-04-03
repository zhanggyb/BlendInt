/**
 * BlendInt demo
 */

#include <iostream>

#include <Cpp/Events.hpp>
#include <BlendInt/Interface.hpp>
#include <BlendInt/Types.hpp>

#include <BlendInt/Gui/CVImageView.hpp>

#include "Window.hpp"

using namespace BlendInt;
using namespace std;

namespace BI = BlendInt;

int main(int argc, char* argv[])
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("GLFW3 Demo", 800, 600);

	CVImageView* img = Manage(new CVImageView);
	img->set_name("CVImageView");
	img->SetPosition(100, 100);
	img->Open("rendering.jpg");

	RunLoop (win);

	Interface::Release();

	Terminate();

	return 0;
}
