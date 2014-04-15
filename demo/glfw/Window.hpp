/**
 * Common Window for Test, based on GLFW
 */

#include <GLFW/glfw3.h>

#include <BlendInt/Interface.hpp>

namespace BlendInt {

    typedef void (*RenderCallback)(void* param);

	extern void Init ();

	extern GLFWwindow* CreateWindow (const char* name, int width = 1280, int height = 800);

	extern void RunLoop (GLFWwindow* window, const RedrawEvent& event, RenderCallback callback = 0, void* param = 0);

	extern void Terminate ();

	extern bool CheckAllocatedObjects ();

}

