// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "LayoutTest.h"

#include <BIL/Interface.hpp>
#include <BIL/Label.hpp>
#include <BIL/Button.hpp>
#include <BIL/FontConfig.hpp>
#include <BIL/Rect.hpp>
#include <BIL/HorizontalLayout.hpp>
#include <BIL/VerticalLayout.hpp>

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(LayoutTest);

LayoutTest::LayoutTest ()
{

}

LayoutTest::~LayoutTest ()
{

}

void LayoutTest::setUp ()
{
	int mapsize = Traceable::mapSize();
    int listsize = Traceable::getSolos().size();

	if(listsize > 0) {
		list<Traceable*>::const_iterator it;
        for (it = Traceable::getSolos().begin(); it != Traceable::getSolos().end(); it++)
		{
			cout << "id: " << (*it)->id() << " was not deleted!" << endl;
		}
	}

	CPPUNIT_ASSERT(mapsize == 0 && listsize == 0);
}

void LayoutTest::tearDown ()
{
	int mapsize = Traceable::mapSize();
    int listsize = Traceable::getSolos().size();

	if(mapsize > 0) {
		map<uint64_t, Traceable*>::const_iterator it;
		for (it = Traceable::getMap().begin(); it != Traceable::getMap().end(); it++)
		{
			cout << "id: " << it->first << " was not deleted!" << endl;
		}
	}

	if(listsize > 0) {
		list<Traceable*>::const_iterator it;
        for (it = Traceable::getSolos().begin(); it != Traceable::getSolos().end(); it++)
		{
			cout << "id: " << (*it)->id() << " was not deleted!" << endl;
		}
	}

	CPPUNIT_ASSERT(mapsize == 0 && listsize == 0);
}

void LayoutTest::horizontal_layout1()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BIL", NULL, NULL);
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

	/* initialize BIL after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	HorizontalLayout layout;
	layout.set_pos(100, 100);

	Label label(L"Hello World!");
	label.set_pos(100, 100);

	Button button(L"Sample Button");
	button.set_pos(100, 200);

	layout.addWidget(&label);
	layout.addWidget(&button);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
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
	CPPUNIT_ASSERT(true);
}

void LayoutTest::vertical_layout1()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BIL", NULL, NULL);
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

	/* initialize BIL after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	VerticalLayout layout;
	layout.set_pos(100, 100);

	Label label(L"Hello World!");
	label.set_pos(100, 100);

	Button button(L"Sample Button");
	button.set_pos(200, 200);

	layout.addWidget(&label);
	layout.addWidget(&button);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
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
	CPPUNIT_ASSERT(true);
}

void LayoutTest::cbError (int error, const char* description)
{
	std::cerr << "Error: " << description
			<< " (error code: " << error << ")"
			<< std::endl;
}

void LayoutTest::cbWindowSize (GLFWwindow* window, int w, int h)
{
	BIL::Interface::instance()->resizeEvent(w, h);
}

void LayoutTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BIL::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void LayoutTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BIL::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void LayoutTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BIL::Interface::instance()->cursorPosEvent(xpos, ypos);
}

