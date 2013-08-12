/* Test case for FontFace */

#ifndef _WINDOW_TEST_H
#define _WINDOW_TEST_H

#include <GLFW/glfw3.h>
#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

#include <BIL/Window.h>

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

class WindowTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(WindowTest);

	CPPUNIT_TEST(event1);
	CPPUNIT_TEST(event2);

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

};

#endif  /* _WINDOW_TEST_H */
