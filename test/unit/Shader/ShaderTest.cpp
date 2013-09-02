// cpp

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include <BIL/Interface.hpp>
#include <BIL/Widget.hpp>
#include "ShaderTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(ShaderTest);

static const char* vs_source =
#ifdef GL_ES_VERSION_2_0
        "#version 100\n"  // OpenGL ES 2.0
#else
        "#version 120\n"  // OpenGL 2.1
#endif
		        "attribute vec2 coord2d;                  "
		        "void main(void) {                        "
		        "  gl_Position = vec4(coord2d, 0.0, 1.0); "
		        "}";

static const char* fs_source =
#ifdef GL_ES_VERSION_2_0
        "#version 100\n"  // OpenGL ES 2.0
#else
        "#version 120\n"  // OpenGL 2.1
#endif
		        "void main(void) {        "
		        "  gl_FragColor[0] = 0.0; "
		        "  gl_FragColor[1] = 0.0; "
		        "  gl_FragColor[2] = 1.0; "
		        "}";

GLint attribute_coord2d;

ShaderWidget::ShaderWidget ()
		: Widget(NULL)
{
	program_.addShader(vs_source, GL_VERTEX_SHADER);
	program_.addShader(fs_source, GL_FRAGMENT_SHADER);
	program_.link();

	const char* attribute_name = "coord2d";
	  attribute_coord2d = glGetAttribLocation(program_.id(), attribute_name);
	  if (attribute_coord2d == -1) {
	    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
	  }
}


void ShaderWidget::render ()
{
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program_.id());

	glEnableVertexAttribArray(attribute_coord2d);
	GLfloat triangle_vertices[] = { 0.0, 0.8, -0.8, -0.8, 0.8, -0.8, };
	/* Describe our vertices array to OpenGL (it can't guess its format automatically) */
	glVertexAttribPointer(attribute_coord2d, // attribute
	        2,                 // number of elements per vertex, here (x,y)
	        GL_FLOAT,          // the type of each element
	        GL_FALSE,          // take our values as-is
	        0,                 // no extra data between each position
	        triangle_vertices  // pointer to the C array
	        );

	/* Push each element in buffer_vertices to the vertex shader */
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(attribute_coord2d);
}

ShaderTest::ShaderTest ()
{

}

ShaderTest::~ShaderTest ()
{

}

void ShaderTest::setUp ()
{

}

void ShaderTest::tearDown ()
{

}

void ShaderTest::shader_load1 ()
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

	ShaderWidget widget;

	// widget.set_round_box_type(RoundBoxAll);
	widget.set_pos(100, 100);
	widget.resize(200, 200);

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

void ShaderTest::cbError (int error, const char* description)
{
	std::cerr << "Error: " << description
			<< " (error code: " << error << ")"
			<< std::endl;
}

void ShaderTest::cbWindowSize (GLFWwindow* window, int w, int h)
{
	BIL::Interface::instance()->resizeEvent(w, h);
}

void ShaderTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BIL::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void ShaderTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BIL::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void ShaderTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BIL::Interface::instance()->cursorPosEvent(xpos, ypos);
}
