/**
 * Common Window for Test, based on GLFW
 */

#include <GLFW/glfw3.h>

namespace BlendInt {

	extern void Init ();

	extern GLFWwindow* CreateWindow (const char* name);

	extern void RunLoop (GLFWwindow* window);

	extern void Terminate ();

	extern bool CheckAllocatedObjects ();

}

