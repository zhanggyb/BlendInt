/**
 * BIL demo
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Cpp/Events.hpp>
#include <BIL/Interface.hpp>

#include <BIL/ToggleButton.hpp>

static void cbWindowSize (GLFWwindow* window, int w, int h)
{
	BIL::Interface::instance()->resizeEvent (w, h);
}

int main (int argc, char* argv[])
{
	using namespace BIL;

	Cpp::Events::ProcessInit processInit;

    GLFWwindow* window;

    Interface* app = Interface::instance();

    /* Initialize the library */
    if(!glfwInit())
        return -1;

    window = glfwCreateWindow(1200, 800, "Demo Window for BIL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

	glfwSetWindowSizeCallback(window, &cbWindowSize);
	
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* initialize BIL after OpenGL content is created */
    if(!Interface::initialize()) {
        glfwTerminate();
    }

    app->resize (1200, 800);

	ToggleButton button ("Hello World!");
	button.set_pos(100, 100);
	button.set_font(Font("Droid Sans"));
	button.set_round_box_type (RoundBoxAll);

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

    /* release BIL */
    Interface::release();

    glfwTerminate();
	return 0;
}

