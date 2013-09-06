/* Test case for FontFace */

#ifndef _SHADER_TEST_H
#define _SHADER_TEST_H

#include <BIL/Widget.hpp>
#include <BIL/GLSLProgram.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace BIL;
using namespace std;

class ShaderWidget1: public Widget
{
public:

	ShaderWidget1 ();

	virtual ~ShaderWidget1 ();

protected:

	virtual void render ();

private:

	struct attributes {
		GLfloat coord2d[2];
		GLfloat v_color[3];
	};

	bool init_resources ();

	GLSLProgram program_;
	
	GLuint vbo_triangle;
	GLuint vbo_triangle_colors;
	GLint attribute_coord2d;
	GLint attribute_v_color;
	GLint uniform_fade;
	
	static const char* vs_source;

	static const char* fs_source;
};

class ShaderWidget2: public Widget
{
public:

	ShaderWidget2 ();

	virtual ~ShaderWidget2 ();

protected:

	virtual void render ();

private:

	struct attributes {
		GLfloat coord2d[2];
		GLfloat v_color[3];
	};

	bool init_resources ();

	GLSLProgram program_;

	GLuint vao_triangle;
	GLuint vbo_triangle[2];
	GLint attribute_VertexPosition;
	GLint attribute_VertexColor;

	static const char* vs_source;

	static const char* fs_source;
};

class ShaderWidget3: public Widget
{
public:

	ShaderWidget3 ();

	virtual ~ShaderWidget3 ();

protected:

	virtual void render ();

private:

	struct attributes {
		GLfloat coord2d[2];
		GLfloat v_color[3];
	};

	bool init_resources ();

	GLSLProgram program_;

	GLuint vertex_array_id_;

	GLuint vertex_buffer_;

	static const char* vs_source;

	static const char* fs_source;
};


class ShaderTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(ShaderTest);

	CPPUNIT_TEST(shader_load1);
	CPPUNIT_TEST(shader_load2);
	CPPUNIT_TEST(shader_load3);

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

	void shader_load2 ();

	void shader_load3 ();

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);

};

#endif  /* _SHADER_TEST_H */
