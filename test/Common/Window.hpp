/**
 * Common Window for Test, based on GLFW
 */

// load GLEW first
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <BlendInt/Interface.hpp>

namespace BlendInt {

    typedef void (*RenderCallback)(void* param);

	extern void Init ();

	extern GLFWwindow* CreateWindow (const char* name);

	extern void RunLoop (GLFWwindow* window, RenderCallback callback = 0, void* param = 0);

	extern void Terminate ();

	extern bool CheckAllocatedObjects ();

}

