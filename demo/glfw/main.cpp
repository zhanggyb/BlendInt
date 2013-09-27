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
#include <BILO/Frame.hpp>
#include <BILO/VertexIcon.hpp>

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

	/*
	Widget* widget = new Widget;

	widget->set_pos(200, 500);
	widget->set_corner_radius(7.0);
	widget->set_roundcorner(RoundCornerAll);
	widget->resize(200, 100);
	widget->set_name("widget");

	Button * button = new Button;

	button->set_pos (400, 300);
	//button->resize (100, 30);
	button->set_text("This is a normal button");
	button->set_font(Font("Droid Sans"));
	button->set_name("button");

	ToggleButton* toggle = new ToggleButton;
	toggle->set_pos (400, 350);
	toggle->set_text("This is a toggle button");
	toggle->set_font(Font("Droid Sans"));
	//toggle->resize (100, 30);
	toggle->set_name("toggle button");
	*/

	Slider* hslider1 = new Slider;
	hslider1->set_pos(100, 30);
	hslider1->set_name("hslider1");

	Slider* hslider2 = new Slider;
	hslider2->set_pos(100, 60);
	hslider2->set_name("hslider2");

	Slider* hslider3 = new Slider;
	hslider3->set_pos(100, 90);
	hslider3->set_name("hslider3");

	Slider* hslider4 = new Slider;
	hslider4->set_pos(100, 120);
	hslider4->set_name("hslider4");

	Slider* hslider5 = new Slider;
	hslider5->set_pos(100, 150);
	hslider5->set_name("hslider4");

	Slider* hslider6 = new Slider;
	hslider6->set_pos(100, 180);
	hslider6->set_name("hslider6");

	Slider* hslider7 = new Slider;
	hslider7->set_pos(100, 210);
	hslider7->set_name("hslider7");

	Slider* hslider8 = new Slider;
	hslider8->set_pos(100, 240);
	hslider8->set_name("hslider8");

	Slider* hslider9 = new Slider;
	hslider9->set_pos(100, 270);
	hslider9->set_name("hslider9");

	Slider* hslider10 = new Slider;
	hslider10->set_pos(100, 300);
	hslider10->set_name("hslider10");

	Slider* hslider11 = new Slider;
	hslider11->set_pos(100, 330);
	hslider11->set_name("hslider11");

	Slider* hslider12 = new Slider;
	hslider12->set_pos(100, 360);
	hslider12->set_name("hslider12");

	Slider* hslider13 = new Slider;
	hslider13->set_pos(100, 390);
	hslider13->set_name("hslider13");

	Slider* hslider14 = new Slider;
	hslider14->set_pos(100, 420);
	hslider14->set_name("hslider14");

	Slider* hslider15 = new Slider;
	hslider15->set_pos(100, 450);
	hslider15->set_name("hslider15");

	Slider* hslider16 = new Slider;
	hslider16->set_pos(100, 480);
	hslider16->set_name("hslider16");

	Slider* vslider = new Slider(Vertical);
	vslider->set_pos(50, 250);
	vslider->set_name("vslider");

//	app->bind(widget);
//	app->bind(button);
//	app->bind(toggle);

	app->bind(hslider1);
	app->bind(hslider2);
	app->bind(hslider3);
	app->bind(hslider4);
	app->bind(hslider5);
	app->bind(hslider6);
	app->bind(hslider7);
	app->bind(hslider8);
	app->bind(hslider9);
	app->bind(hslider10);
	app->bind(hslider11);
	app->bind(hslider12);
	app->bind(hslider13);
	app->bind(hslider14);
	app->bind(hslider15);
	app->bind(hslider16);

	app->bind(vslider);

	app->events().connect(hslider1->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider2->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider3->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider4->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider5->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider6->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider7->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider8->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider9->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider10->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider11->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider12->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider13->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider14->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider15->slider_moved(), vslider, &AbstractSlider::set_value);
	app->events().connect(hslider16->slider_moved(), vslider, &AbstractSlider::set_value);

	Frame* frame = new Frame;

	frame->resize(400, 400);
	frame->set_pos(400, 200);

	VertexIcon icon;

	app->bind(frame);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();


		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(500,
					 200,
					 0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor3ub(255, 200, 133);

		icon.display();
		glBegin(GL_TRIANGLES);
		glVertex2f (200, 200);
		glVertex2f (400, 200);
		glVertex2f (300, 300);
		glEnd();

		glDisable(GL_BLEND);

		glPopMatrix();

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

