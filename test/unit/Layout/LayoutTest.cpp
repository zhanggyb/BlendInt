// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "LayoutTest.h"

#include <BILO/Interface.hpp>
#include <BILO/Label.hpp>
#include <BILO/Button.hpp>
#include <BILO/FontConfig.hpp>
#include <BILO/Rect.hpp>
#include <BILO/HorizontalLayout.hpp>
#include <BILO/VerticalLayout.hpp>
#include <BILO/Types.hpp>

using namespace BILO;
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

void LayoutTest::tearDown ()
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

void LayoutTest::horizontal_layout1()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO", NULL, NULL);
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	HorizontalLayout layout1;
	layout1.set_pos(100, 100);
	layout1.set_alignment(BILO::AlignHorizontalCenter);

	Label label1(L"Hello");
	label1.set_pos(100, 100);

	Button button1(L"Sample Button");
	button1.set_pos(200, 200);

	layout1.add_widget(&label1);
	layout1.add_widget(&button1);

	HorizontalLayout layout2;
	layout2.set_pos(300, 200);
	layout2.set_alignment(BILO::AlignTop);

	Label label2(L"Hello");
	label2.set_pos(100, 100);

	Button button2(L"Sample Button");
	button2.set_pos(200, 200);

	layout2.add_widget(&label2);
	layout2.add_widget(&button2);

	HorizontalLayout layout3;
	layout3.set_pos(500, 300);
	layout3.set_alignment(BILO::AlignBottom);

	Label label3(L"Hello");
	label3.set_pos(100, 100);

	Button button3(L"Sample Button");
	button3.set_pos(200, 200);

	layout3.add_widget(&label3);
	layout3.add_widget(&button3);

	app->bind(&layout1);
	app->bind(&layout2);
	app->bind(&layout3);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	layout1.unbind();
	layout2.unbind();
	layout3.unbind();

	/* release BILO */
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

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO", NULL, NULL);
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	VerticalLayout layout1;
	layout1.set_pos(100, 100);
	layout1.set_alignment(BILO::AlignVerticalCenter);

	Label label1(L"Hello");
	label1.set_pos(100, 100);

	Button button1(L"Sample Button");
	button1.set_pos(200, 200);

	layout1.add_widget(&label1);
	layout1.add_widget(&button1);

	VerticalLayout layout2;
	layout2.set_pos(250, 100);
	layout2.set_alignment(BILO::AlignLeft);

	Label label2(L"Hello");
	label2.set_pos(100, 100);

	Button button2(L"Sample Button");
	button2.set_pos(200, 200);

	layout2.add_widget(&label2);
	layout2.add_widget(&button2);

	VerticalLayout layout3;
	layout3.set_pos(500, 100);
	layout3.set_alignment(BILO::AlignRight);

	Label label3(L"Hello");
	label3.set_pos(100, 100);

	Button button3(L"Sample Button");
	button3.set_pos(200, 200);

	layout3.add_widget(&label3);
	layout3.add_widget(&button3);

	app->bind(&layout1);
	app->bind(&layout2);
	app->bind(&layout3);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	layout1.unbind();
	layout2.unbind();
	layout3.unbind();

	/* release BILO */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void LayoutTest::layout_mix1()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO", NULL, NULL);
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	VerticalLayout* layout1 = new VerticalLayout;
	layout1->set_pos(100, 100);
	layout1->set_alignment(BILO::AlignVerticalCenter);

	Label* label1 = new Label(L"la1");
	label1->set_pos(100, 100);

	Button* button1 = new Button(L"Button1");
	button1->set_pos(200, 200);

	layout1->add_widget(label1);
	layout1->add_widget(button1);

	HorizontalLayout* layout2 = new HorizontalLayout;

	Label* label2 = new Label(L"Hello Blender");
	label2->set_pos(500, 500);

	Button* button2 = new Button(L"Sample Button");
	button2->set_pos(200, 200);

	layout2->add_widget(label2);
	layout2->add_layout(layout1);
	layout2->add_widget(button2);

	layout2->set_pos (400, 500);

	app->bind(layout2);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	//delete layout2;
	/* release BILO */
	Interface::release();

	glfwTerminate();

	CPPUNIT_ASSERT(true);
}

void LayoutTest::layout_mix2()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO", NULL, NULL);
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	VerticalLayout layout1;
	layout1.set_pos(100, 100);
	layout1.set_alignment(BILO::AlignVerticalCenter);

	Label label1(L"Hello");
	label1.set_pos(100, 100);

	Button button1(L"Sample Button");
	button1.set_pos(200, 200);

	layout1.add_widget(&label1);
	layout1.add_widget(&button1);

	VerticalLayout layout2;
	layout2.set_pos(250, 100);
	layout2.set_alignment(BILO::AlignLeft);

	Label label2(L"Hello");
	label2.set_pos(100, 100);

	Button button2(L"Sample Button");
	button2.set_pos(200, 200);

	layout2.add_widget(&label2);
	layout2.add_widget(&button2);

	VerticalLayout layout3;
	layout3.set_pos(500, 100);
	layout3.set_alignment(BILO::AlignRight);

	Label label3(L"Hello");
	label3.set_pos(100, 100);

	Button button3(L"Sample Button");
	button3.set_pos(200, 200);

	layout3.add_widget(&label3);
	layout3.add_widget(&button3);

	HorizontalLayout layout4;
	Button button4(L"Button4");

	layout4.add_widget(&button4);
	layout4.add_layout(&layout1);
	layout4.add_layout(&layout2);
	layout4.add_layout(&layout3);
	layout4.set_pos(400, 200);

	app->bind(&layout4);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	layout4.unbind();
	layout3.unbind();
	layout2.unbind();
	layout1.unbind();

	/* release BILO */
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
	BILO::Interface::instance()->resizeEvent(w, h);
}

void LayoutTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BILO::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void LayoutTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BILO::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void LayoutTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BILO::Interface::instance()->cursorPosEvent(xpos, ypos);
}

