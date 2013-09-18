/**
 * BILO demo
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Cpp/Events.hpp>
#include <BILO/Interface.hpp>

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

	VerticalLayout* layout1 = new VerticalLayout;
	layout1->set_pos(100, 100);
	layout1->set_alignment(BILO::AlignVerticalCenter);

	Label* label1 = new Label(L"la1");
	label1->set_pos(100, 100);

	layout1->add_widget(label1);

	HorizontalLayout layout2;

	Label* label2 = new Label(L"Hello Blender");
	label2->set_pos(500, 500);

	layout2.add_widget(label2);
	layout2.add_layout(layout1);

	layout2.set_pos (400, 500);
	layout2.set_z(2);

	//ScrollWidget scroll_widget;
	//scroll_widget.set_pos(500, 200);

	ScrollBar hbar(Horizontal);
	hbar.set_pos (500, 100);

	ScrollBar vbar(Vertical);
	vbar.set_pos (600, 200);

	Slider hslider (Horizontal);
	hslider.set_pos (200, 200);
	hslider.set_value(80);

	Slider vslider (Vertical);
	vslider.set_pos (200, 300);
	vslider.set_value(20);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* release BILO */
	Interface::release();

	glfwTerminate();
	return 0;
}

