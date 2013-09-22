/**
 * BILO demo
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Cpp/Events.hpp>
#include <BILO/Interface.hpp>
#include <BILO/ContextManager.hpp>

#include <BILO/ToggleButton.hpp>
#include <BILO/VerticalLayout.hpp>
#include <BILO/HorizontalLayout.hpp>

#include <BILO/Types.hpp>
#include <BILO/Button.hpp>
#include <BILO/Label.hpp>
#include <BILO/ScrollWidget.hpp>
#include <BILO/Slider.hpp>

using namespace BILO;

static void cbError (int error, const char* description)
{
	std::cerr << "Error: " << description
			<< " (error code: " << error << ")"
			<< std::endl;
}

static void cbWindowSize(GLFWwindow* window, int w, int h) {
	BILO::Interface::instance()->resizeEvent(w, h);
}

static void cbKey(GLFWwindow* window, int key, int scancode, int action,
		int mods) {
	BILO::Interface::instance()->keyEvent(key, scancode, action, mods);
}

static void cbMouseButton(GLFWwindow* window, int button, int action,
		int mods) {
	BILO::Interface::instance()->mouseButtonEvent(button, action, mods);
}

static void cbCursorPos(GLFWwindow* window, double xpos, double ypos) {
	BILO::Interface::instance()->cursorPosEvent(xpos, ypos);
}

int main(int argc, char* argv[]) {
	using namespace BILO;

	Cpp::Events::ProcessInit processInit;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(window, &cbWindowSize);
	glfwSetKeyCallback(window, &cbKey);
	glfwSetMouseButtonCallback(window, &cbMouseButton);
	glfwSetCursorPosCallback(window, &cbCursorPos);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		return -1;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	ContextManager* cm = ContextManager::instance();

	Widget widget1;
	widget1.set_pos(50, 50);
	widget1.resize (50, 50);
	widget1.set_name("widget1");

	Widget widget2;
	widget2.set_pos(100, 100);
	widget2.resize (50, 50);
	widget2.set_name("widget2");

	Widget widget3;
	widget3.set_pos(150, 150);
	widget3.resize (50, 50);
	widget3.set_name("widget3");
	widget3.set_z(1);

	Widget widget4;
	widget4.set_pos(200, 200);
	widget4.resize (50, 50);
	widget4.set_name("widget4");
	widget4.set_z(1);

	cm->print();
	cm->bind(&widget1);

	widget1.bind(&widget2);
	//widget1.bind(&widget4);

	cm->print();

	widget3.bind(&widget4);

	cm->print();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	widget1.unbind();
	widget2.unbind();
	widget3.unbind();
	widget4.unbind();

	cm->print();

	/* release BILO */
	Interface::release();

	glfwTerminate();
	return 0;
}

