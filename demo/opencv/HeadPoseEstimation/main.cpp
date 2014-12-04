/**
 * BlendInt demo
 */

#include <Cpp/Events.hpp>
#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Stock/Cursor.hpp>

#include <CVWindow/Window.hpp>
#include <CVWindow/CVCursor.hpp>
#include "HPEContext.hpp"

using namespace BlendInt;
using namespace std;

int main(int argc, char* argv[])
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("OpenCV Demo", 1280, 800);
	Cursor::instance->RegisterCursorType (new CVCursor(win));

	HPEContext* context = Manage(new HPEContext);
	DBG_SET_NAME(context, "Context");
	SetContext(context);
	context->Resize(1280, 800);

	RunLoop (win);
	Terminate();

	return 0;
}
