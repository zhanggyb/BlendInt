/* Test case for FontFace */

#ifndef _THEME_TEST_H
#define _THEME_TEST_H

#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class ThemeTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ThemeTest);

    CPPUNIT_TEST(initialize1);
    CPPUNIT_TEST(initialize2);

    CPPUNIT_TEST_SUITE_END();

 public:

    ThemeTest();

    virtual ~ThemeTest();

    void setUp();

    void tearDown();

    void runTest ();

 private:

    void initialize1 ();

    void initialize2 ();

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);


};

#endif  /* _THEME_TEST_H */
