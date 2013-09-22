// cpp

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include <BILO/Interface.hpp>
#include <BILO/Widget.hpp>
#include "ShaderTest.h"

using namespace BILO;
using namespace std;

//CPPUNIT_TEST_SUITE_REGISTRATION(ShaderTest);

const char* ShaderWidget1::vs_source =
		"attribute vec2 coord2d;"
		"attribute vec3 v_color;"
		"varying vec3 f_color;"
		""
		"void main(void) {"
		"	vec4 pos = gl_ModelViewProjectionMatrix * vec4(coord2d, 0.0, 1.0);"
		"	gl_Position = pos;"
		"	f_color = v_color;"
		"}";

const char* ShaderWidget1::fs_source =
	"uniform float fade;"
	"varying vec3 f_color;"
	""
	"void main(void) {"
	"	gl_FragColor = vec4(f_color.x, f_color.y, f_color.z, fade);"
	"}";

ShaderWidget1::ShaderWidget1 ()
		: Widget(NULL)
{
	init_resources();
}

ShaderWidget1::~ShaderWidget1()
{
	glDeleteBuffers(1, &vbo_triangle);
}

void ShaderWidget1::render ()
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

    glUseProgram(0);

}

bool ShaderWidget1::init_resources()
{
	if(!program_.isValid()) {
		return false;
	}

	struct attributes triangle_attributes[] = {
		{{ 100,  100}, {1.0, 1.0, 0.0}},
		{{ 500, 100}, {0.0, 0.0, 1.0}},
		{{ 300, 300}, {1.0, 0.0, 0.0}}
	};
	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

	program_.attachShaderPair(vs_source, fs_source);
	
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

const char* ShaderWidget2::vs_source =
		"attribute vec3 VertexPosition;"
		"attribute vec3 VertexColor;"
		"varying vec3 Color;"
		""
		"void main(void) {"
		"	Color = VertexColor;"
		"	gl_Position = vec4(VertexPosition, 1.0);"
		"}";

const char* ShaderWidget2::fs_source =
	"varying vec3 Color;"
	""
	"void main(void) {"
	"	gl_FragColor = vec4(Color, 1.0);"
	"}";

ShaderWidget2::ShaderWidget2 ()
		: Widget(NULL)
{
	init_resources();
}

ShaderWidget2::~ShaderWidget2()
{
	glDeleteBuffers(2, vbo_triangle);
	glDeleteVertexArrays(1, &vao_triangle);
}

void ShaderWidget2::render ()
{
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear (GL_COLOR_BUFFER_BIT);

	glUseProgram(program_.id());

	glBindVertexArray(vao_triangle);

	/* Push each element in buffer_vertices to the vertex shader */
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUseProgram(0);
}

bool ShaderWidget2::init_resources()
{
	if(!program_.isValid()) {
		return false;
	}

	float positionData[] = {
			-0.8f, -0.8f, 0.0f,
			0.8f, -0.8f, 0.0f,
			0.0f, 0.8f, 0.0f
	};

	float colorData[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
	};

	program_.attachShader(vs_source, GL_VERTEX_SHADER);
	program_.attachShader(fs_source, GL_FRAGMENT_SHADER);

	glGenBuffers(2, vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle[0]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

	// Create and set-up the vertex array object
	glGenVertexArrays(1, &vao_triangle );
	glBindVertexArray(vao_triangle);
	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(0); // Vertex position
	glEnableVertexAttribArray(1); // Vertex color
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle[0]);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0,
	(GLubyte *)NULL );
	// Map index 1 to the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle[1]);
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0,
	(GLubyte *)NULL );

	if(!program_.link()) {
		return false;
	}

	return true;
}

ShaderWidget3::ShaderWidget3 ()
		: Widget(NULL)
{
	init_resources();
}

ShaderWidget3::~ShaderWidget3()
{
	//glDeleteBuffers(1, &vertex_buffer_);
	//glDeleteVertexArrays(1, &vertex_array_id_);
}

const char* ShaderWidget3::vs_source =
		"void main(void) {"
		"	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
		"}";

const char* ShaderWidget3::fs_source =
	"void main(void) {"
		"	vec3 color = vec3(1.0, 1.0, 0.0);"
	"	gl_FragColor = vec4(color, 0.2);"
	"}";

void ShaderWidget3::render ()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glUseProgram(program_.id());

	glBegin(GL_TRIANGLES);
	glVertex3i(200, 200, 0);
	glVertex3i(800, 200, 0);
	glVertex3i(500, 600,0);
	glEnd();

	glPopMatrix();

	glUseProgram(0);
}

bool ShaderWidget3::init_resources()
{
	if(!program_.isValid()) {
		return false;
	}

	program_.attachShader(vs_source, GL_VERTEX_SHADER);
	program_.attachShader(fs_source, GL_FRAGMENT_SHADER);

	if(!program_.link()) {
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

void ShaderTest::shader_load1 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO", NULL, NULL);
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

	ShaderWidget1 widget;

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

	/* release BILO */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void ShaderTest::shader_load2 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO", NULL, NULL);
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

	ShaderWidget2 widget;

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

	/* release BILO */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void ShaderTest::shader_load3 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BILO", NULL, NULL);
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

	ShaderWidget3 widget;

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

	/* release BILO */
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
	BILO::Interface::instance()->resizeEvent(w, h);
}

void ShaderTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BILO::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void ShaderTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BILO::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void ShaderTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BILO::Interface::instance()->cursorPosEvent(xpos, ypos);
}
