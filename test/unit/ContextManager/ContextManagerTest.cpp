// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "ContextManagerTest.h"

#include <BlendInt/Interface.hpp>
#include <BlendInt/Label.hpp>
#include <BlendInt/FontConfig.hpp>
#include <BlendInt/Rect.hpp>

#include <BlendInt/ContextManager.hpp>

using namespace BlendInt;
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
#ifdef DEBUG
	int mapsize = AbstractForm::map_size();

	if(mapsize > 0) {
		map<uint64_t, AbstractForm*>::const_iterator it;
		for (it = AbstractForm::get_map().begin(); it != AbstractForm::get_map().end(); it++)
		{
			cout << "id: " << it->first << " was not deleted!" << endl;
		}
	}

	CPPUNIT_ASSERT(mapsize == 0);
#endif
}

void ContextManagerTest::check_layer_0_0 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, __func__, NULL, NULL);
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
	widget3.reset_z(1);

	Widget widget4;
	widget4.set_pos(200, 200);
	widget4.resize (50, 50);
	widget4.set_name("widget4");
	widget4.reset_z(1);

	cm->bind(&widget1);
	cm->bind(&widget3);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 2 && cm->layer_size() == 2);

	widget1.bind(&widget2);
	widget1.bind(&widget4);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 2 && cm->layer_size() == 2);

	widget3.bind(&widget4);

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

	widget1.unbind(&widget2);
	widget3.unbind(&widget4);

	cm->print();

	widget1.unbind();
	widget3.unbind();

	/* release BlendInt */
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

	GLFWwindow* window = glfwCreateWindow(1200, 800, __func__, NULL, NULL);
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
	widget3->reset_z(1);

	Widget* widget4 = new Widget;
	widget4->set_pos(200, 200);
	widget4->resize (50, 50);
	widget4->set_name("widget4");
	widget4->reset_z(1);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 0 && cm->layer_size() == 0);

	cm->bind(widget1);
	widget1->bind(widget2);
	widget1->bind(widget4);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 1 && cm->layer_size() == 1);

	widget3->bind(widget4);
	widget3->bind_to(cm);

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

	/* release BlendInt */
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

	GLFWwindow* window = glfwCreateWindow(1200, 800, __func__, NULL, NULL);
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
	widget3->reset_z(1);

	Widget* widget4 = new Widget(widget3);
	widget4->set_pos(200, 200);
	widget4->resize (50, 50);
	widget4->set_name("widget4");
	widget4->reset_z(1);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 0 && cm->layer_size() == 0);

	widget1->bind(widget2);
	widget1->bind(widget4);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 0 && cm->layer_size() == 0);

	widget3->bind(widget4);

	cm->bind(widget1);
	cm->bind(widget3);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 2 && cm->layer_size() == 2);

	widget2->unbind();
	widget4->unbind();

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

	delete widget2; delete widget4;
	//delete widget5;

	/* release BlendInt */
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

	GLFWwindow* window = glfwCreateWindow(1200, 800, __func__, NULL, NULL);
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
	widget3->reset_z(1);

	Widget* widget4 = new Widget(widget3);
	widget4->set_pos(200, 200);
	widget4->resize (50, 50);
	widget4->set_name("widget4");
	widget4->reset_z(1);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 0 && cm->layer_size() == 0);

	cm->bind(widget1);
	cm->bind(widget3);
	widget1->bind(widget2);
	widget3->bind(widget4);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 2 && cm->layer_size() == 2);

	widget4->reset_z(0);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 2 && cm->layer_size() == 1);

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

	/* release BlendInt */
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

	GLFWwindow* window = glfwCreateWindow(1200, 800, __func__, NULL, NULL);
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
	CPPUNIT_ASSERT(cm->index_size() == 0 && cm->layer_size() == 0);

	widget1->bind(widget2);
	widget1->bind(widget3);
	widget1->bind(widget4);

	cm->bind(widget1);
	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 1 && cm->layer_size() == 1);

	widget1->unbind(widget2);
	widget1->unbind(widget3);
	widget1->unbind(widget4);
	widget1->unbind(widget2);	// try again

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 1 && cm->layer_size() == 1);

	widget1->bind(widget2);
	widget1->bind(widget3);
	widget1->bind(widget4);

	widget2->reset_z (1);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 1 && cm->layer_size() == 1);

	widget3->reset_z (2);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 1 && cm->layer_size() == 1);

	widget4->reset_z (3);

	cm->print();
	CPPUNIT_ASSERT(cm->index_size() == 1 && cm->layer_size() == 1);

	widget3->reset_z (1);
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

	/* release BlendInt */
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
	BlendInt::Interface::instance()->resizeEvent(w, h);
}

void ContextManagerTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BlendInt::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void ContextManagerTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BlendInt::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void ContextManagerTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BlendInt::Interface::instance()->cursorPosEvent(xpos, ypos);
}
