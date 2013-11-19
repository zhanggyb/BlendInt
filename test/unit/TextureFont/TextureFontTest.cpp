// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "TextureFontTest.h"

#include <BlendInt/Interface.hpp>
#include <BlendInt/Label.hpp>
#include <BlendInt/FontConfig.hpp>
#include <BlendInt/Rect.hpp>
#include <BlendInt/TextureFont.hpp>
#include <BlendInt/ShaderManager.hpp>
#include <BlendInt/AbstractWidget.hpp>

using namespace BlendInt;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(TextureFontTest);

TextureFontTest::TextureFontTest ()
{

}

TextureFontTest::~TextureFontTest ()
{

}

void TextureFontTest::setUp ()
{
}

void TextureFontTest::tearDown ()
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

static GLfloat black[4] = { 0, 0, 0, 1 };

void TextureFontTest::show1 ()
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

	Freetype fe;
	fe.open(Font("Sans"), 96);
	fe.setCharSize(100);
	TextureFont tex_font;
	tex_font.generate(&fe, L'A');

	ShaderManager* sm = ShaderManager::instance();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		/* Render here */
		app->Render();

		/* Enable blending, necessary for our alpha texture */
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(sm->text_program().id());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_font.texture());
		glUniform1i(sm->text_uniform_tex(), 0);

		glUniform4fv(sm->text_uniform_color(), 1, black);

		/* Set up the VBO for our vertex data */
		glEnableVertexAttribArray(sm->text_attribute_coord());
		glBindBuffer(GL_ARRAY_BUFFER, sm->text_vbo());
		glVertexAttribPointer(sm->text_attribute_coord(), 4, GL_FLOAT, GL_FALSE, 0, 0);

		// Render character from atlas here
		/* Calculate the vertex and texture coordinates */
		float x2 = 100 + tex_font.glyph().bitmap_left;
		float y2 = 100 + tex_font.glyph().bitmap_top;
		float w = tex_font.glyph().bitmap_width;
		float h = tex_font.glyph().bitmap_height;

		point box[4] = {
			{x2, y2, 0, 0},
			{x2 + w, y2, 1, 0},
			{x2, y2 - h, 0, 1},
			{x2 + w, y2 - h, 1, 1},
		};

		/* Draw the character on the screen */
		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisableVertexAttribArray(sm->text_attribute_coord());
		glUseProgram(0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* release BlendInt */
	Interface::release();
	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void TextureFontTest::show2 ()
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

	Freetype fe;
	fe.open(Font("Sans"), 96);
	fe.setCharSize(100);
	TextureFont tex_font;
	tex_font.generate(&fe, L'A');

	ShaderManager* sm = ShaderManager::instance();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		/* Render here */
		app->Render();

		/* Enable blending, necessary for our alpha texture */
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(sm->text_program().id());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_font.texture());
		glUniform1i(sm->text_uniform_tex(), 0);

		glUniform4fv(sm->text_uniform_color(), 1, black);

		/* Set up the VBO for our vertex data */
		glEnableVertexAttribArray(sm->text_attribute_coord());
		glBindBuffer(GL_ARRAY_BUFFER, sm->text_vbo());
		glVertexAttribPointer(sm->text_attribute_coord(), 4, GL_FLOAT, GL_FALSE, 0, 0);

		// Render character from atlas here
		/* Calculate the vertex and texture coordinates */

		float x2 = 100 + tex_font.glyph().bitmap_left;
		float y2 = 100 + tex_font.glyph().bitmap_top;
		float w = tex_font.glyph().bitmap_width;
		float h = tex_font.glyph().bitmap_height;

		point box[4] = {
			{x2, y2, 0, 0},
			{x2 + w, y2, 1, 0},
			{x2, y2 - h, 0, 1},
			{x2 + w, y2 - h, 1, 1},
		};

		/* Draw the character on the screen */
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * 6, &(tex_font.glyph().vertexes[0]), GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisableVertexAttribArray(sm->text_attribute_coord());
		glUseProgram(0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* release BlendInt */
	Interface::release();
	glfwTerminate();
	CPPUNIT_ASSERT(true);
}


void TextureFontTest::cbError (int error, const char* description)
{
	std::cerr << "Error: " << description
			<< " (error code: " << error << ")"
			<< std::endl;
}

void TextureFontTest::cbWindowSize (GLFWwindow* window, int w, int h)
{
	BlendInt::Interface::instance()->resizeEvent(w, h);
}

void TextureFontTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BlendInt::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void TextureFontTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BlendInt::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void TextureFontTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BlendInt::Interface::instance()->cursorPosEvent(xpos, ypos);
}
