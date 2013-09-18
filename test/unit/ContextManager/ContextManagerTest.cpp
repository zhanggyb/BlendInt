// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "ContextManagerTest.h"

#include <BIL/Interface.hpp>
#include <BIL/Label.hpp>
#include <BIL/FontConfig.hpp>
#include <BIL/Rect.hpp>

#include <BIL/ContextManager.hpp>

using namespace BILO;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(ContextManagerTest);

ContextManagerTest::ContextManagerTest ()
{

}

ContextManagerTest::~ContextManagerTest ()
{

}

void ContextManagerTest::setUp ()
{
}

void ContextManagerTest::tearDown ()
{
}

void ContextManagerTest::check_layer_0_0 ()
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
	CPPUNIT_ASSERT(cm->index_size() == 4 && cm->layer_size() == 2);

	widget1.add_child(&widget2);
	widget1.add_child(&widget4);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 3 && cm->layer_size() == 2);

	widget3.add_child(&widget4);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 2 && cm->layer_size() == 2);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	widget1.remove_child(&widget2);
	widget3.remove_child(&widget4);

	cm->print();

	/* release BIL */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void ContextManagerTest::check_layer_0_1 ()
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

	ContextManager* cm = ContextManager::instance();

	Widget* widget1 = new Widget;
	widget1->set_pos(50, 50);
	widget1->resize (50, 50);
	widget1->set_name("widget1");

	Widget* widget2 = new Widget;
	widget2->set_pos(100, 100);
	widget2->resize (50, 50);
	widget2->set_name("widget2");

	Widget* widget3 = new Widget;
	widget3->set_pos(150, 150);
	widget3->resize (50, 50);
	widget3->set_name("widget3");
	widget3->set_z(1);

	Widget* widget4 = new Widget;
	widget4->set_pos(200, 200);
	widget4->resize (50, 50);
	widget4->set_name("widget4");
	widget4->set_z(1);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 4 && cm->layer_size() == 2);

	widget1->add_child(widget2);
	widget1->add_child(widget4);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 3 && cm->layer_size() == 2);

	widget3->add_child(widget4);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 2 && cm->layer_size() == 2);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	delete widget1;
	delete widget3;

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 0 && cm->layer_size() == 0);

	/* release BIL */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void ContextManagerTest::check_layer_0_2 ()
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

	ContextManager* cm = ContextManager::instance();

	Widget* widget1 = new Widget;
	widget1->set_pos(50, 50);
	widget1->resize (50, 50);
	widget1->set_name("widget1");

	Widget* widget2 = new Widget(widget1);
	widget2->set_pos(100, 100);
	widget2->resize (50, 50);
	widget2->set_name("widget2");

	Widget* widget3 = new Widget;
	widget3->set_pos(150, 150);
	widget3->resize (50, 50);
	widget3->set_name("widget3");
	widget3->set_z(1);

	Widget* widget4 = new Widget(widget3);
	widget4->set_pos(200, 200);
	widget4->resize (50, 50);
	widget4->set_name("widget4");
	widget4->set_z(1);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 2 && cm->layer_size() == 2);

	widget1->add_child(widget2);
	widget1->add_child(widget4);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 3 && cm->layer_size() == 2);


	widget3->add_child(widget4);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 2 && cm->layer_size() == 2);

	widget2->set_parent(0);
	widget4->set_parent(0);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 4 && cm->layer_size() == 2);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	delete widget1;
	delete widget3;

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 2 && cm->layer_size() == 2);

	delete widget2; delete widget4;
	//delete widget5;

	/* release BIL */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void ContextManagerTest::check_layer_0_3 ()
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

	ContextManager* cm = ContextManager::instance();

	Widget* widget1 = new Widget;
	widget1->set_pos(50, 50);
	widget1->resize (50, 50);
	widget1->set_name("widget1");

	Widget* widget2 = new Widget(widget1);
	widget2->set_pos(100, 100);
	widget2->resize (50, 50);
	widget2->set_name("widget2");

	Widget* widget3 = new Widget;
	widget3->set_pos(150, 150);
	widget3->resize (50, 50);
	widget3->set_name("widget3");
	widget3->set_z(1);

	Widget* widget4 = new Widget(widget3);
	widget4->set_pos(200, 200);
	widget4->resize (50, 50);
	widget4->set_name("widget4");
	widget4->set_z(1);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 2 && cm->layer_size() == 2);

	widget1->add_child(widget2);
	widget1->add_child(widget3);
	widget1->add_child(widget4);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 3 && cm->layer_size() == 2);

	widget1->remove_child(widget2);
	widget1->remove_child(widget3);
	widget1->remove_child(widget4);
	widget1->remove_child(widget2);	// try again

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 4 && cm->layer_size() == 2);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	delete widget1;
	delete widget3;

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 2 && cm->layer_size() == 2);

	delete widget2; delete widget4;
	//delete widget5;

	/* release BIL */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void ContextManagerTest::check_layer_0_4 ()
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

	ContextManager* cm = ContextManager::instance();

	Widget* widget1 = new Widget;
	widget1->set_pos(50, 50);
	widget1->resize (50, 50);
	widget1->set_name("widget1");

	Widget* widget2 = new Widget(widget1);
	widget2->set_pos(100, 100);
	widget2->resize (50, 50);
	widget2->set_name("widget2");

	Widget* widget3 = new Widget(widget2);
	widget3->set_pos(150, 150);
	widget3->resize (50, 50);
	widget3->set_name("widget3");

	Widget* widget4 = new Widget(widget3);
	widget4->set_pos(200, 200);
	widget4->resize (50, 50);
	widget4->set_name("widget4");

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 1 && cm->layer_size() == 1);

	widget1->add_child(widget2);
	widget1->add_child(widget3);
	widget1->add_child(widget4);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 1 && cm->layer_size() == 1);

	widget1->remove_child(widget2);
	widget1->remove_child(widget3);
	widget1->remove_child(widget4);
	widget1->remove_child(widget2);	// try again

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 4 && cm->layer_size() == 1);

	widget1->add_child(widget2);
	widget1->add_child(widget3);
	widget1->add_child(widget4);

	widget2->set_z (1);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 2 && cm->layer_size() == 2);

	widget3->set_z (2);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 3 && cm->layer_size() == 3);

	widget4->set_z (3);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 4 && cm->layer_size() == 4);

	widget3->set_z (1);
	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 4 && cm->layer_size() == 3);

	widget1->add_child(widget2);
	widget3->set_parent(widget2);
	widget3->add_child(widget4);
	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 3 && cm->layer_size() == 3);

	widget2->set_z(0);
	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 1 && cm->layer_size() == 1);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	delete widget1;

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 0 && cm->layer_size() == 0);

	/* release BIL */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}


void ContextManagerTest::cbError (int error, const char* description)
{
	std::cerr << "Error: " << description
			<< " (error code: " << error << ")"
			<< std::endl;
}

void ContextManagerTest::cbWindowSize (GLFWwindow* window, int w, int h)
{
	BILO::Interface::instance()->resizeEvent(w, h);
}

void ContextManagerTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BILO::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void ContextManagerTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BILO::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void ContextManagerTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BILO::Interface::instance()->cursorPosEvent(xpos, ypos);
}
