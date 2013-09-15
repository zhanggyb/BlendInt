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
#include <BIL/Types.hpp>

using namespace BIL;
using namespace std;

//CPPUNIT_TEST_SUITE_REGISTRATION(LayoutTest);

LayoutTest::LayoutTest ()
{

}

LayoutTest::~LayoutTest ()
{

}

void LayoutTest::setUp ()
{
	int mapsize = Traceable::mapSize();

	if(mapsize > 0) {
		map<uint64_t, Traceable*>::const_iterator it;
    for (it = Traceable::getMap().begin(); it != Traceable::getMap().end(); it++)
		{
			cout << "id: " << it->first << " was not deleted!" << endl;
		}
	}

	CPPUNIT_ASSERT(mapsize == 0);
}

void LayoutTest::tearDown ()
{
	int mapsize = Traceable::mapSize();

	if(mapsize > 0) {
		map<uint64_t, Traceable*>::const_iterator it;
		for (it = Traceable::getMap().begin(); it != Traceable::getMap().end(); it++)
		{
			cout << "id: " << it->first << " was not deleted!" << endl;
		}
	}

	CPPUNIT_ASSERT(mapsize == 0);
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

	HorizontalLayout layout1;
	layout1.set_pos(100, 100);
	layout1.set_alignment(BIL::AlignHorizontalCenter);

	Label label1(L"Hello");
	label1.set_pos(100, 100);

	Button button1(L"Sample Button");
	button1.set_pos(200, 200);

	layout1.addWidget(&label1);
	layout1.addWidget(&button1);

	HorizontalLayout layout2;
	layout2.set_pos(300, 200);
	layout2.set_alignment(BIL::AlignTop);

	Label label2(L"Hello");
	label2.set_pos(100, 100);

	Button button2(L"Sample Button");
	button2.set_pos(200, 200);

	layout2.addWidget(&label2);
	layout2.addWidget(&button2);

	HorizontalLayout layout3;
	layout3.set_pos(500, 300);
	layout3.set_alignment(BIL::AlignBottom);

	Label label3(L"Hello");
	label3.set_pos(100, 100);

	Button button3(L"Sample Button");
	button3.set_pos(200, 200);

	layout3.addWidget(&label3);
	layout3.addWidget(&button3);


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

	VerticalLayout layout1;
	layout1.set_pos(100, 100);
	layout1.set_alignment(BIL::AlignVerticalCenter);

	Label label1(L"Hello");
	label1.set_pos(100, 100);

	Button button1(L"Sample Button");
	button1.set_pos(200, 200);

	layout1.addWidget(&label1);
	layout1.addWidget(&button1);

	VerticalLayout layout2;
	layout2.set_pos(250, 100);
	layout2.set_alignment(BIL::AlignLeft);

	Label label2(L"Hello");
	label2.set_pos(100, 100);

	Button button2(L"Sample Button");
	button2.set_pos(200, 200);

	layout2.addWidget(&label2);
	layout2.addWidget(&button2);

	VerticalLayout layout3;
	layout3.set_pos(500, 100);
	layout3.set_alignment(BIL::AlignRight);

	Label label3(L"Hello");
	label3.set_pos(100, 100);

	Button button3(L"Sample Button");
	button3.set_pos(200, 200);

	layout3.addWidget(&label3);
	layout3.addWidget(&button3);

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

void LayoutTest::layout_mix1()
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

	VerticalLayout* layout1 = new VerticalLayout;
	layout1->set_pos(100, 100);
	layout1->set_alignment(BIL::AlignVerticalCenter);

	Label* label1 = new Label(L"la1");
	label1->set_pos(100, 100);

	Button* button1 = new Button(L"Button1");
	button1->set_pos(200, 200);

	layout1->addWidget(label1);
	layout1->addWidget(button1);

	HorizontalLayout* layout2 = new HorizontalLayout;

	Label* label2 = new Label(L"Hello Blender");
	label2->set_pos(500, 500);

	Button* button2 = new Button(L"Sample Button");
	button2->set_pos(200, 200);

	layout2->addWidget(label2);
	layout2->addLayout(layout1);
	layout2->addWidget(button2);

	layout2->set_pos (400, 500);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	delete layout2;
	/* release BIL */
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

	VerticalLayout layout1;
	layout1.set_pos(100, 100);
	layout1.set_alignment(BIL::AlignVerticalCenter);

	Label label1(L"Hello");
	label1.set_pos(100, 100);

	Button button1(L"Sample Button");
	button1.set_pos(200, 200);

	layout1.addWidget(&label1);
	layout1.addWidget(&button1);

	VerticalLayout layout2;
	layout2.set_pos(250, 100);
	layout2.set_alignment(BIL::AlignLeft);

	Label label2(L"Hello");
	label2.set_pos(100, 100);

	Button button2(L"Sample Button");
	button2.set_pos(200, 200);

	layout2.addWidget(&label2);
	layout2.addWidget(&button2);

	VerticalLayout layout3;
	layout3.set_pos(500, 100);
	layout3.set_alignment(BIL::AlignRight);

	Label label3(L"Hello");
	label3.set_pos(100, 100);

	Button button3(L"Sample Button");
	button3.set_pos(200, 200);

	layout3.addWidget(&label3);
	layout3.addWidget(&button3);

	HorizontalLayout layout4;
	Button button4(L"Button4");

	layout4.addWidget(&button4);
	layout4.addLayout(&layout1);
	layout4.addLayout(&layout2);
	layout4.addLayout(&layout3);
	layout4.set_pos(400, 200);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	layout3.set_parent(0);
	layout2.set_parent(0);
	layout1.set_parent(0);

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

