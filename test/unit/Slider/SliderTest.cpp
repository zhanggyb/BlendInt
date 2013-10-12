// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "SliderTest.h"

#include <BlendInt/Interface.hpp>
#include <BlendInt/Slider.hpp>

#include <stdlib.h>

using namespace BlendInt;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(SliderTest);

SliderTest::SliderTest ()
{

}

SliderTest::~SliderTest ()
{

}

void SliderTest::setUp ()
{
}

void SliderTest::tearDown ()
{
#ifdef DEBUG
	int mapsize = Drawable::map_size();

	if(mapsize > 0) {
		map<uint64_t, Drawable*>::const_iterator it;
		for (it = Drawable::get_map().begin(); it != Drawable::get_map().end(); it++)
		{
			cout << "id: " << it->first << " was not deleted!" << endl;
		}
	}

	CPPUNIT_ASSERT(mapsize == 0);
#endif
}

void SliderTest::slider_move_test01()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BlendInt", NULL, NULL);
	if (!window) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	glfwSetWindowSizeCallback(window, &cbWindowSize);
	glfwSetKeyCallback(window, &cbKey);
	glfwSetMouseButtonCallback(window, &cbMouseButton);
	glfwSetCursorPosCallback(window, &cbCursorPos);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* initialize BlendInt after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	Slider* hslider = new Slider;
	hslider->set_pos(200, 200);
	hslider->set_value(50);

	Slider* vslider = new Slider(Vertical);
	vslider->set_pos(200, 250);
	vslider->set_value(50);

	app->bind(hslider);
	app->bind(vslider);

	app->events().connect(hslider->slider_moved(), vslider, &AbstractSlider::set_value);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* release BlendInt */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void SliderTest::cbError (int error, const char* description)
{
	std::cerr << "Error: " << description
			<< " (error code: " << error << ")"
			<< std::endl;
}

void SliderTest::cbWindowSize (GLFWwindow* window, int w, int h)
{
	BlendInt::Interface::instance()->resizeEvent(w, h);
}

void SliderTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BlendInt::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void SliderTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BlendInt::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void SliderTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BlendInt::Interface::instance()->cursorPosEvent(xpos, ypos);
}
