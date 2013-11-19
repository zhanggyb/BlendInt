// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Label.hpp>
#include <BlendInt/Theme.hpp>
#include <BlendInt/AbstractWidget.hpp>

#include "ThemeTest.h"

using namespace BlendInt;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(ThemeTest);

ThemeTest::ThemeTest ()
{

}

ThemeTest::~ThemeTest ()
{

}

void ThemeTest::setUp ()
{
}

void ThemeTest::tearDown ()
{
#ifdef DEBUG
	int mapsize = AbstractWidget::map_size();

	if(mapsize > 0) {
		map<uint64_t, AbstractWidget*>::const_iterator it;
		for (it = AbstractWidget::get_map().begin(); it != AbstractWidget::get_map().end(); it++)
		{
			cout << "id: " << it->first << " was not deleted!" << endl;
		}
	}

	CPPUNIT_ASSERT(mapsize == 0);
#endif
}

void ThemeTest::initialize1 ()
{
	if(!ThemeManager::initialize()) {
		CPPUNIT_ASSERT(false);
		return;
	}

	unsigned char r = 0x00;

	ThemeManager* theme = ThemeManager::instance();

	if (theme != NULL)
		theme->initialize();

	//_themeUI.wcol_tool.outline = RGBAf(0.098, 0.098, 0.098);
	r = theme->themes()->tool.outline.r();

	if (theme != NULL) {
		ThemeManager::release();
	}

	cout << "Red: " << r << endl;
	ThemeManager::release();
	CPPUNIT_ASSERT(r == 0x19);
}

void ThemeTest::initialize2 ()
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
	app->Resize(1200, 800);

    ThemeManager* theme = ThemeManager::instance();
	//_themeUI.wcol_tool.outline = RGBAf(0.098, 0.098, 0.098);
	Color bg_color = theme->themes()->menu_item.item;
	Color textcolor = theme->themes()->menu_item.text;

	Label label(L"Text in Label");
	label.SetPosition(Point(50, 50));
	label.set_background(bg_color);
	label.set_font(Font("Droid Sans", 12));
	label.set_foreground(textcolor);

	app->bind(&label);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->Render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	app->unbind(&label);

	/* release BlendInt */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void ThemeTest::cbError (int error, const char* description)
{
	std::cerr << "Error: " << description
			<< " (error code: " << error << ")"
			<< std::endl;
}

void ThemeTest::cbWindowSize (GLFWwindow* window, int w, int h)
{
	BlendInt::Interface::instance()->resizeEvent(w, h);
}

void ThemeTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BlendInt::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void ThemeTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BlendInt::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void ThemeTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BlendInt::Interface::instance()->cursorPosEvent(xpos, ypos);
}
