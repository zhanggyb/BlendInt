/**
 * Common Window for Test, based on GLFW
 */

#include <GLFW/glfw3.h>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

    typedef void (*RenderCallback)(void* param);

	extern void Init ();

	extern GLFWwindow* CreateWindow (const char* name, int width = 1280, int height = 800);

	extern void RunLoop (GLFWwindow* window, RenderCallback callback = 0, void* param = 0);

	extern void Terminate ();

	extern void SetContext (Context* context);
}

