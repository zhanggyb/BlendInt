/**
 * BIL demo
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Cpp/Events.hpp>
#include <BIL/Interface.hpp>

int main (int argc, char* argv[])
{
	using namespace BIL;

	Cpp::Events::ProcessInit processInit;

    GLFWwindow* window;

    Interface* app = Interface::create();

    /* Initialize the library */
    if(!glfwInit())
        return -1;


    window = glfwCreateWindow(1200, 800, "Demo Window for BIL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* initialize BIL after OpenGL content is created */
    app->initialize();

    app->resizeEvent (1200, 800);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        app->render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* terminate BIL */
    app->terminate();

    glfwTerminate();
	return 0;
}

