// cpp

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include <BILO/Interface.hpp>
#include <BILO/Drawable.hpp>
#include <BILO/Widget.hpp>
#include <BILO/ContextManager.hpp>
#include "DrawableTest.h"

using namespace BILO;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(DrawableTest);

myTexture::myTexture (Drawable *parent)
		: Drawable(parent)
{
	resize(400, 400);
	set_pos(Point(20, 20));

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &_texName);
	glBindTexture(GL_TEXTURE_2D, _texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight,
	        0, GL_RGBA, GL_UNSIGNED_BYTE, _checkImage);

}

myTexture::~myTexture ()
{

}

void myTexture::render (void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, _texName);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(100.0, 100, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(400.0, 100, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(400.0, 400.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(100.0, 400.0, 0.0);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void myTexture::press_key (KeyEvent* event)
{
}

void myTexture::press_context_menu (ContextMenuEvent* event)
{

}

void myTexture::release_context_menu (ContextMenuEvent* event)
{

}

void myTexture::press_mouse (MouseEvent* event)
{

}

void myTexture::release_mouse (MouseEvent* event)
{

}

void myTexture::move_mouse (MouseEvent* event)
{

}

void myTexture::makeCheckImage (void)
{
	int i, j, c;
	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = (((i & 0x8) == 0) ^ ((j & 0x8)) == 0) * 255;
			_checkImage[i][j][0] = (GLubyte) c;
			_checkImage[i][j][1] = (GLubyte) c;
			_checkImage[i][j][2] = (GLubyte) c;
			_checkImage[i][j][3] = (GLubyte) 255;
		}
	}
}

myDrawable1::myDrawable1 (Drawable* parent)
		: Drawable(parent)
{
	round_box_type_ = RoundCornerAll;
}

myDrawable1::~myDrawable1 ()
{

}

void myDrawable1::render (void)
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 0.9f, 1.0f, 0.95f);

	DrawBox(GL_POLYGON, pos_.x(), pos_.y(), pos_.x() + size_.width(),
	        pos_.y() + size_.height(), 5.0);

	glDisable(GL_BLEND);
}

void myDrawable1::press_key (KeyEvent* event)
{
}

void myDrawable1::press_context_menu (ContextMenuEvent* event)
{

}

void myDrawable1::release_context_menu (ContextMenuEvent* event)
{

}

void myDrawable1::press_mouse (MouseEvent* event)
{
}

void myDrawable1::release_mouse (MouseEvent* event)
{

}

void myDrawable1::move_mouse (MouseEvent* event)
{

}

myDrawable2::myDrawable2 (Drawable* parent)
		: Drawable(parent)
{
	round_box_type_ = RoundCornerAll;
}

myDrawable2::~myDrawable2 ()
{

}

void myDrawable2::render (void)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor4f(1.0f, 0.9f, 1.0f, 0.95f);

	DrawBoxShade(GL_POLYGON, pos_.x(), pos_.y(), pos_.x() + size_.width(),
	        pos_.y() + size_.height(), 5.0, 0.5, 0.1);

	glDisable(GL_BLEND);
}

void myDrawable2::press_key (KeyEvent* event)
{
}

void myDrawable2::press_context_menu (ContextMenuEvent* event)
{

}

void myDrawable2::release_context_menu (ContextMenuEvent* event)
{

}

void myDrawable2::press_mouse (MouseEvent* event)
{
}

void myDrawable2::release_mouse (MouseEvent* event)
{
}

void myDrawable2::move_mouse (MouseEvent* event)
{

}

myDrawable3::myDrawable3 (Drawable* parent)
		: Drawable(parent)
{
	round_box_type_ = RoundCornerAll;
}

myDrawable3::~myDrawable3 ()
{

}

void myDrawable3::render (void)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor4f(1.0f, 0.9f, 1.0f, 0.95f);

	drawRoundBox(pos_.x(), pos_.y(), pos_.x() + size_.width(),
	        pos_.y() + size_.height(), 5.0);
	DrawShadowBox(pos_.x(), pos_.y(), pos_.x() + size_.width(),
	        pos_.y() + size_.height(), 5.0, 64);

	glDisable(GL_BLEND);
}

void myDrawable3::press_key (KeyEvent* event)
{
}

void myDrawable3::press_context_menu (ContextMenuEvent* event)
{

}

void myDrawable3::release_context_menu (ContextMenuEvent* event)
{

}

void myDrawable3::press_mouse (MouseEvent* event)
{

}

void myDrawable3::release_mouse (MouseEvent* event)
{

}

void myDrawable3::move_mouse (MouseEvent* event)
{

}

DrawableTest::DrawableTest ()
{

}

DrawableTest::~DrawableTest ()
{

}

void DrawableTest::setUp ()
{

}

void DrawableTest::tearDown ()
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

void DrawableTest::texture1 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO",
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	myTexture widget;

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
	CPPUNIT_ASSERT(true);
}

void DrawableTest::mydrawable1 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO",
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	myDrawable1 widget;
	widget.set_pos(Point(50, 50));
	widget.resize(80, 40);

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
	CPPUNIT_ASSERT(true);
}

void DrawableTest::mydrawable2 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO",
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	myDrawable2 widget;
	widget.set_pos(Point(50, 50));
	widget.resize(80, 40);

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
	CPPUNIT_ASSERT(true);
}

void DrawableTest::mydrawable3 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO",
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	myDrawable3 widget;
	widget.set_pos(Point(50, 50));
	widget.resize(80, 40);

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
	CPPUNIT_ASSERT(true);
}

void DrawableTest::bind_test1()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Drawable bind test 01",
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	Widget widget1;
	widget1.set_pos(0, 0);
	widget1.resize (50, 50);
	widget1.bind_to (ContextManager::instance());

	Widget widget2;
	widget2.set_pos(50, 50);
	widget2.resize (50, 50);
	widget2.bind_to(&widget1);

	Widget widget3;
	widget3.set_pos(100, 100);
	widget3.resize(50, 50);
	widget2.bind(&widget3);

	Widget widget4;
	widget4.set_pos(150, 150);
	widget4.resize(50, 50);
	ContextManager::instance()->bind(&widget4);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	widget1.unbind();
	widget2.unbind();
	widget3.unbind();
	widget4.unbind();

	/* release BILO */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void DrawableTest::bind_test2()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Drawable bind test 02",
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	Widget* widget1 = new Widget;
	widget1->set_pos(0, 0);
	widget1->resize (50, 50);
	widget1->set_name("widget1");
	widget1->bind_to (ContextManager::instance());

	Widget* widget2 = new Widget;
	widget2->set_pos(50, 50);
	widget2->resize (50, 50);
	widget2->set_name("widget2");
	widget2->bind_to(widget1);

	Widget* widget3 = new Widget;
	widget3->set_pos(100, 100);
	widget3->resize(50, 50);
	widget3->set_name("widget3");
	widget2->bind(widget3);

	Widget* widget4 = new Widget;
	widget4->set_pos(150, 150);
	widget4->resize(50, 50);
	widget4->set_name("widget4");
	ContextManager::instance()->bind(widget4);

	Drawable::print();
	ContextManager::instance()->print();

	CPPUNIT_ASSERT(Drawable::map_size() == 4);

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

	Drawable::print();
	ContextManager::instance()->print();

	CPPUNIT_ASSERT(Drawable::map_size() == 1);

	/* release BILO */
	Interface::release();

	CPPUNIT_ASSERT(Drawable::map_size() == 0);

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void DrawableTest::bind_test3()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Drawable bind test 03",
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	Widget* widget1 = new Widget;
	widget1->set_pos(0, 0);
	widget1->resize (50, 50);
	widget1->set_name("widget1");
	widget1->bind_to (ContextManager::instance());

	Widget* widget2 = new Widget;
	widget2->set_pos(50, 50);
	widget2->resize (50, 50);
	widget2->set_name("widget2");
	widget2->bind_to(ContextManager::instance());

	Widget* widget3 = new Widget;
	widget3->set_pos(100, 100);
	widget3->resize(50, 50);
	widget3->set_name("widget3");
	widget2->bind(widget3);

	Widget* widget4 = new Widget;
	widget4->set_pos(150, 150);
	widget4->resize(50, 50);
	widget4->set_name("widget4");
	ContextManager::instance()->bind(widget4);

	delete widget4; widget4 = 0;

	CPPUNIT_ASSERT(Drawable::map_size() == 3);

	widget2->bind_to(widget1);

	CPPUNIT_ASSERT(ContextManager::instance()->index_size() == 1);

	widget1->unbind();
	CPPUNIT_ASSERT(ContextManager::instance()->index_size() == 0);

	widget1->unbind(widget2);
	widget1->bind_to(ContextManager::instance());

	CPPUNIT_ASSERT(ContextManager::instance()->index_size() == 1);

	widget1->bind(widget2);

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
	CPPUNIT_ASSERT(Drawable::map_size() == 0);

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void DrawableTest::bind_test4()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Drawable bind test 04",
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	Widget* widget1 = new Widget;
	widget1->set_pos(0, 0);
	widget1->resize (50, 50);
	widget1->set_name("widget1");

	Widget* widget2 = new Widget;
	widget2->set_pos(50, 50);
	widget2->resize (50, 50);
	widget2->set_name("widget2");

	Widget* widget3 = new Widget;
	widget3->set_pos(100, 100);
	widget3->resize(50, 50);
	widget3->set_name("widget3");

	Widget* widget4 = new Widget;
	widget4->set_pos(150, 150);
	widget4->resize(50, 50);
	widget4->set_name("widget4");

	CPPUNIT_ASSERT(Drawable::map_size() == 4);

	CPPUNIT_ASSERT(!widget1->bounded());

	widget1->bind(widget2);
	widget2->bind(widget3);
	widget3->bind(widget4);

	CPPUNIT_ASSERT(!widget1->bounded());

	widget1->bind_to(ContextManager::instance());

	CPPUNIT_ASSERT(widget4->bounded());

	widget3->bind_to(ContextManager::instance());

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
	CPPUNIT_ASSERT(Drawable::map_size() == 0);

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void DrawableTest::bind_test5()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Drawable bind test 05",
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	Widget* widget1 = new Widget;
	widget1->set_pos(0, 0);
	widget1->resize (50, 50);
	widget1->set_name("widget1");

	Widget* widget2 = new Widget;
	widget2->set_pos(50, 50);
	widget2->resize (50, 50);
	widget2->set_name("widget2");

	Widget* widget3 = new Widget;
	widget3->set_pos(100, 100);
	widget3->resize(50, 50);
	widget3->set_name("widget3");

	Widget* widget4 = new Widget;
	widget4->set_pos(150, 150);
	widget4->resize(50, 50);
	widget4->set_name("widget4");

	CPPUNIT_ASSERT(Drawable::map_size() == 4);

	CPPUNIT_ASSERT(!widget1->bounded());

	widget1->bind(widget2);
	widget2->bind(widget3);
	widget3->bind(widget4);
	widget1->bind_to(ContextManager::instance());

	//widget1->set_z(1);

//	CPPUNIT_ASSERT(ContextManager::instance()->index_size() == 1);

//	CPPUNIT_ASSERT(ContextManager::instance()->layer_size() == 1);

	widget3->bind_to(ContextManager::instance());

	//widget3->set_z(0);

//	CPPUNIT_ASSERT(ContextManager::instance()->index_size() == 2);
//	CPPUNIT_ASSERT(ContextManager::instance()->layer_size() == 2);

	ContextManager::instance()->print();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	delete widget1; widget1 = 0;
	delete widget3; widget3 = 0;

	CPPUNIT_ASSERT(ContextManager::instance()->index_size() == 0);
	CPPUNIT_ASSERT(ContextManager::instance()->layer_size() == 0);

	/* release BILO */
	Interface::release();
	CPPUNIT_ASSERT(Drawable::map_size() == 0);
}

void DrawableTest::bind_test6()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Drawable bind test 06",
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	Widget* widget1 = new Widget;
	widget1->set_pos(0, 0);
	widget1->resize (50, 50);
	widget1->set_name("widget1");
	widget1->bind_to (ContextManager::instance());

	Widget* widget2 = new Widget;
	widget2->set_pos(50, 50);
	widget2->resize (50, 50);
	widget2->set_name("widget2");
	widget2->bind_to(ContextManager::instance());

	Widget* widget3 = new Widget;
	widget3->set_pos(100, 100);
	widget3->resize(50, 50);
	widget3->set_name("widget3");
	widget2->bind(widget3);

	Widget* widget4 = new Widget;
	widget4->set_pos(150, 150);
	widget4->resize(50, 50);
	widget4->set_name("widget4");
	ContextManager::instance()->bind(widget4);

	delete widget4; widget4 = 0;

	CPPUNIT_ASSERT(Drawable::map_size() == 3);

	widget2->bind_to(widget1);

	CPPUNIT_ASSERT(ContextManager::instance()->index_size() == 1);

	widget1->unbind();
	CPPUNIT_ASSERT(ContextManager::instance()->index_size() == 0);

	widget1->unbind(widget2);
	widget1->bind_to(ContextManager::instance());

	CPPUNIT_ASSERT(ContextManager::instance()->index_size() == 1);

	widget1->bind(widget2);

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
	CPPUNIT_ASSERT(Drawable::map_size() == 0);

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void DrawableTest::bind_test7()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO",
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	Widget widget1;
	widget1.set_pos(200, 200);
	widget1.bind_to (ContextManager::instance());

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	widget1.unbind();

	/* release BILO */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void DrawableTest::bind_test8()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO",
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	Widget widget1;
	widget1.set_pos(200, 200);
	widget1.bind_to (ContextManager::instance());

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	widget1.unbind();

	/* release BILO */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void DrawableTest::bind_test9()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO",
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	Widget widget1;
	widget1.set_pos(200, 200);
	widget1.bind_to (ContextManager::instance());

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	widget1.unbind();

	/* release BILO */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void DrawableTest::bind_test10()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO",
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

	/* initialize BILO after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	Widget widget1;
	widget1.set_pos(200, 200);
	widget1.bind_to (ContextManager::instance());

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	widget1.unbind();

	/* release BILO */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void DrawableTest::cbError (int error, const char* description)
{
	std::cerr << "Error: " << description << " (error code: " << error << ")"
	        << std::endl;
}

void DrawableTest::cbWindowSize (GLFWwindow* window, int w, int h)
{
	BILO::Interface::instance()->resizeEvent(w, h);
}

void DrawableTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BILO::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void DrawableTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BILO::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void DrawableTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BILO::Interface::instance()->cursorPosEvent(xpos, ypos);
}

