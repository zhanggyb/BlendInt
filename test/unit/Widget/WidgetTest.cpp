// cpp

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Widget.hpp>
#include <BlendInt/AbstractForm.hpp>
#include <BlendInt/ContextManager.hpp>
#include "WidgetTest.h"

using namespace BlendInt;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(WidgetTest);

testWidget::testWidget (int type, AbstractForm* parent)
		: Widget(parent)
{
	m_round_type = RoundCornerAll;
	set_position(Point(100, 100));
	resize(200, 200);

	switch (type) {
		case 0:
			break;
		case 1:

			break;
		case 2:

			break;
		case 3:
			break;
		default:
			break;
	}

}

void testWidget::render (void)
{
}

WidgetTest::WidgetTest ()
{

}

WidgetTest::~WidgetTest ()
{

}

void WidgetTest::setUp ()
{

}

void WidgetTest::tearDown ()
{
#ifdef DEBUG
	int mapsize = AbstractForm::map_size();

	if (mapsize > 0) {
		map<uint64_t, AbstractForm*>::const_iterator it;
		for (it = AbstractForm::get_map().begin(); it != AbstractForm::get_map().end();
		        it++) {
			cout << "id: " << it->first << " was not deleted!" << endl;
		}
	}

	CPPUNIT_ASSERT(mapsize == 0);
#endif
}

void WidgetTest::widget_default_show ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BlendInt",
	        NULL, NULL);
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

	Widget widget;

	// widget.set_round_box_type(RoundBoxAll);
	widget.set_position(50, 50);
	widget.resize(200, 100);

	ContextManager::instance()->bind(&widget);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	widget.unbind();

	/* release BlendInt */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void WidgetTest::testwidget_show0 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BlendInt",
	        NULL, NULL);
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

	testWidget widget(0);
	widget.set_position(Point(50, 50));
	widget.resize(80, 40);

	ContextManager::instance()->bind(&widget);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	widget.unbind();
	/* release BlendInt */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void WidgetTest::testwidget_show1 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BlendInt",
	        NULL, NULL);
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

	testWidget widget(1);

	ContextManager::instance()->bind(&widget);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	widget.unbind();

	/* release BlendInt */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void WidgetTest::testwidget_show2 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BlendInt",
	        NULL, NULL);
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

	testWidget widget(2);
	ContextManager::instance()->bind(&widget);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	widget.unbind();
	/* release BlendInt */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void WidgetTest::testwidget_show3 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BlendInt",
	        NULL, NULL);
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

	testWidget widget(3);
	ContextManager::instance()->bind(&widget);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	widget.unbind();
	/* release BlendInt */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void WidgetTest::cbError (int error, const char* description)
{
	std::cerr << "Error: " << description << " (error code: " << error << ")"
	        << std::endl;
}

void WidgetTest::cbWindowSize (GLFWwindow* window, int w, int h)
{
	BlendInt::Interface::instance()->resizeEvent(w, h);
}

void WidgetTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BlendInt::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void WidgetTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BlendInt::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void WidgetTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BlendInt::Interface::instance()->cursorPosEvent(xpos, ypos);
}
