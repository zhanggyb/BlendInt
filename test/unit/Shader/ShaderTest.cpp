// cpp

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <string>
#include <stdio.h>

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
	Application app;

	Window win(640, 480, "640 x 480 Shader Test Window", NULL, NULL);

	app.setMainWindow(&win);
	app.initialize();

	ShaderWidget widget;
	widget.setParent(&win);

	// widget.set_round_box_type(RoundBoxAll);
	widget.set_pos(100, 100);
	widget.resize(200, 200);

	app.run();

	CPPUNIT_ASSERT(true);
}
