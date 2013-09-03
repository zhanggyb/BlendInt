/* Test case for FontFace */

#ifndef _SHADER_TEST_H
#define _SHADER_TEST_H

#include <BIL/Widget.hpp>
#include <BIL/Program.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace BIL;
using namespace BIL::GL;
using namespace std;

class ShaderWidget: public Widget
{
public:

	ShaderWidget ();

	virtual ~ShaderWidget ();

protected:

	virtual void render ();

private:

	struct attributes {
		GLfloat coord2d[2];
		GLfloat v_color[3];
	};

	bool init_resources ();

	Program program_;
	
	GLuint vbo_triangle;
	GLuint vbo_triangle_colors;
	GLint attribute_coord2d;
	GLint attribute_v_color;
	GLint uniform_fade;
	
};

class ShaderTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(ShaderTest);

	CPPUNIT_TEST(shader_load1);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	ShaderTest ();

	virtual ~ShaderTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void shader_load1 ();

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);

};

#endif  /* _SHADER_TEST_H */
