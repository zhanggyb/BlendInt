/**
 * BlendInt demo
 */

#include <iostream>

#include <Cpp/Events.hpp>
#include <BlendInt/Interface.hpp>
#include <BlendInt/Core/Types.hpp>

#include <BlendInt/Gui/CVImageView.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/ToolBar.hpp>

#include "CVWindow.hpp"
#include "CVContext.hpp"

using namespace BlendInt;
using namespace std;

namespace BI = BlendInt;

int main(int argc, char* argv[])
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("OpenCV Demo", 1280, 800);

	CVContext* context = Manage(new CVContext);
	DBG_SET_NAME(context, "CVContext");
	Interface::instance->SetCurrentContext(context);
	Interface::instance->Resize(1280, 800);

	RunLoop (win);

	Interface::Release();

	Terminate();

	return 0;
}
