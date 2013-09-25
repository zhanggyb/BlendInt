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

int main(int argc, char* argv[])
{
	using namespace BILO;

	BILO_EVENTS_INIT_ONCE_IN_MAIN;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, __func__, NULL, NULL);
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

	FontCache* cache1 = FontCache::create(Font("Droid Sans", 24));

	FontCache* cache2 = FontCache::create(Font("Sans", 24));

	Widget* widget = new Widget;

	widget->set_pos(300, 300);
	widget->set_corner_radius(50.0);
	widget->set_roundcorner(RoundCornerAll);
	widget->resize(400, 400);

	Slider* hslider = new Slider;
	hslider->set_pos(200, 200);
	hslider->set_value(50);

	Slider* vslider = new Slider(Vertical);
	vslider->set_pos(200, 250);
	vslider->set_value(50);

	app->bind(widget);
	app->bind(hslider);
	app->bind(vslider);

	app->events().connect(hslider->slider_moved(), vslider, &AbstractSlider::set_value);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		cache1->print(100, 150, "Hello World! (Droid Sans)");

		cache2->print(100, 100, "Hello World! (Sans)");

		cache2->print(600, 100, "Hello World! (Sans)", 5);

		cache2->print("Hello World! (Sans)", 5);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	FontCache::list();

	/* release BILO */
	Interface::release();

	glfwTerminate();
	return 0;
}

