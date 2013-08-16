/* Test case for FontFace */

#ifndef _WINDOW_TEST_H
#define _WINDOW_TEST_H

#include <GLFW/glfw3.h>
#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

#include <BIL/Window.hpp>

using namespace BIL;
using namespace std;

class myWindow: public BIL::Window
{
public:
	myWindow(int width, int height, const char *title, GLFWmonitor *monitor,
	        GLFWwindow *share, Traceable* parent = NULL);

protected:

	virtual void cursorEnterEvent (int entered);
};

class myWidget: public BIL::Widget
{
 public:

	myWidget();

	virtual ~myWidget();

	virtual void Render (void);

 protected:

	virtual void MousePressEvent (MouseEvent* event);
	
	virtual void MouseMoveEvent (MouseEvent* event);

};

class WindowTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(WindowTest);

	CPPUNIT_TEST(event1);
	CPPUNIT_TEST(event2);

	CPPUNIT_TEST(solorender1);

	CPPUNIT_TEST(check_mouse_press1);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	WindowTest ();

	virtual ~WindowTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void event1 ();
	void event2 ();

	void check_mouse_press1 ();

	void solorender1 ();

};

#endif  /* _WINDOW_TEST_H */
