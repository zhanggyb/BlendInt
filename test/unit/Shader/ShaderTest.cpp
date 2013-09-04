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
	"attribute vec2 coord2d;"
	"attribute vec3 v_color;"
	"varying vec3 f_color;"
	""
	"void main(void) {"
	"	gl_Position = vec4(coord2d, 0.0, 1.0);"
	"	f_color = v_color;"
	"}";

static const char* fs_source =
	"uniform float fade;"
	"varying vec3 f_color;"
	""
	"void main(void) {"
	"	gl_FragColor = vec4(f_color.x, f_color.y, f_color.z, fade);"
	"}";

ShaderWidget::ShaderWidget ()
		: Widget(NULL)
{
	init_resources();
}

ShaderWidget::~ShaderWidget()
{
	glDeleteBuffers(1, &vbo_triangle);
}

void ShaderWidget::render ()
{
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear (GL_COLOR_BUFFER_BIT);

	glUseProgram(program_.id());

	glUniform1f(uniform_fade, 1.0);

	glEnableVertexAttribArray(attribute_coord2d);
	glEnableVertexAttribArray(attribute_v_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glVertexAttribPointer(
						  attribute_coord2d,   // attribute
						  2,                   // number of elements per vertex, here (x,y)
						  GL_FLOAT,            // the type of each element
						  GL_FALSE,            // take our values as-is
						  sizeof(struct attributes),  // next coord2d appears every 5 floats
						  0                    // offset of first element
  );
	glVertexAttribPointer(
						  attribute_v_color,      // attribute
						  3,                      // number of elements per vertex, here (r,g,b)
						  GL_FLOAT,               // the type of each element
						  GL_FALSE,               // take our values as-is
						  sizeof(struct attributes),  // stride
						  //(GLvoid*) (2 * sizeof(GLfloat))     // offset of first element
						  (GLvoid*) offsetof(struct attributes, v_color)  // offset
  );

	/* Push each element in buffer_vertices to the vertex shader */
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(attribute_coord2d);
	glDisableVertexAttribArray(attribute_v_color);

}

bool ShaderWidget::init_resources()
{
	if(!program_.isValid()) {
		return false;
	}

	struct attributes triangle_attributes[] = {
		{{ 0.0,  0.8}, {1.0, 1.0, 0.0}},
		{{-0.8, -0.8}, {0.0, 0.0, 1.0}},
		{{ 0.8, -0.8}, {1.0, 0.0, 0.0}}
	};
	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

	program_.attachShader(vs_source, GL_VERTEX_SHADER);
	program_.attachShader(fs_source, GL_FRAGMENT_SHADER);
	
	if(!program_.link()) {
		return false;
	}

	const char* attribute_name;
	attribute_name = "coord2d";
	attribute_coord2d = glGetAttribLocation(program_.id(), attribute_name);
	if (attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return false;
	}
	attribute_name = "v_color";
	attribute_v_color = glGetAttribLocation(program_.id(), attribute_name);
	if (attribute_v_color == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return false;
	}

	const char* uniform_name;
	uniform_name = "fade";
	uniform_fade = glGetUniformLocation(program_.id(), uniform_name);
	if (uniform_fade == -1) {
		fprintf(stderr, "Could not bind uniform_fade %s\n", uniform_name);
		return false;
	}


	return true;
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
	// widget.set_pos(100, 100);
	// widget.resize(200, 200);

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
