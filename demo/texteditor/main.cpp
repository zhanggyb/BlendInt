/**
 * BlendInt demo
 */

#include <BlendInt/Interface.hpp>
#include <BlendInt/Types.hpp>

#include "EditorContext.hpp"
#include "Window.hpp"

#include <BlendInt/Gui/TextureAtlas.hpp>

using namespace BlendInt;
using namespace std;

namespace BI = BlendInt;

int main(int argc, char* argv[])
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("GLFW3 Demo", 640, 480);

	EditorContext* context = Manage (new EditorContext);
	context->set_name("Context");
	Interface::instance->SetCurrentContext(context);

	TextureAtlasExt atlas;
	atlas.Generate(64, 64, 20, 20, 1);
	atlas.Push(1);
	atlas.Push(2);
	atlas.Push(3);
	atlas.Push(4);
	atlas.Push(5);
	atlas.Push(6);
	atlas.Push(7);
	atlas.Push(8);
	atlas.Push(9);
	atlas.Push(10);
	atlas.Clear();

	int num = atlas.GetMaxNumber();
	std::cout << "max num: " << num << std::endl;

	RunLoop(win);

	Interface::Release();

	Terminate();

	return 0;
}

