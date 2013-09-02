// cpp

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include <BIL/Interface.hpp>
#include <BIL/Drawable.hpp>
#include "DrawableTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (DrawableTest);

myTexture::myTexture (Traceable *parent)
    : Drawable(parent)
{
    resize (400, 400);
    set_pos (Point(20, 20));

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glShadeModel (GL_FLAT);
    glEnable (GL_DEPTH_TEST);
    makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &_texName);
    glBindTexture(GL_TEXTURE_2D, _texName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
                 checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, _checkImage);

}

myTexture::~myTexture()
{

}

void myTexture::render (void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, _texName);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0); glVertex3f (100.0, 100, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f (400.0, 100, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f (400.0, 400.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f (100.0, 400.0, 0.0);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void myTexture::keyPressEvent (KeyEvent* event)
{
}

void myTexture::contextMenuPressEvent (ContextMenuEvent* event)
{

}

void myTexture::contextMenuReleaseEvent (ContextMenuEvent* event)
{

}

void myTexture::mousePressEvent (MouseEvent* event)
{

}
	
void myTexture::mouseReleaseEvent (MouseEvent* event)
{

}

void myTexture::mouseMoveEvent (MouseEvent* event)
{

}

void myTexture::makeCheckImage (void)
{
    int i, j, c;
    for(i = 0; i < checkImageHeight; i++) {
        for(j = 0; j < checkImageWidth; j++) {
            c = (((i & 0x8) == 0) ^ ((j & 0x8)) == 0) * 255;
            _checkImage[i][j][0] = (GLubyte)c;
            _checkImage[i][j][1] = (GLubyte)c;
            _checkImage[i][j][2] = (GLubyte)c;
            _checkImage[i][j][3] = (GLubyte)255;
        }
    }
}

myDrawable1::myDrawable1(Traceable* parent)
	: Drawable (parent)
{
	round_box_type_ = RoundBoxAll;
}

myDrawable1::~myDrawable1()
{

}

void myDrawable1::render(void)
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 0.9f, 1.0f, 0.95f);

		DrawBox(GL_POLYGON,
				pos_.x(),
				pos_.y(),
				pos_.x() + size_.width(),
				pos_.y() + size_.height(),
				5.0);

	glDisable(GL_BLEND);
}

void myDrawable1::keyPressEvent (KeyEvent* event)
{
}

void myDrawable1::contextMenuPressEvent (ContextMenuEvent* event)
{

}

void myDrawable1::contextMenuReleaseEvent (ContextMenuEvent* event)
{

}

void myDrawable1::mousePressEvent (MouseEvent* event)
{
}
	
void myDrawable1::mouseReleaseEvent (MouseEvent* event)
{

}

void myDrawable1::mouseMoveEvent (MouseEvent* event)
{

}

myDrawable2::myDrawable2(Traceable* parent)
	: Drawable (parent)
{
	round_box_type_ = RoundBoxAll;
}

myDrawable2::~myDrawable2()
{

}

void myDrawable2::render(void)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
		glColor4f(1.0f, 0.9f, 1.0f, 0.95f);

		DrawBoxShade(GL_POLYGON,
				pos_.x(),
				pos_.y(),
				pos_.x() + size_.width(),
				pos_.y() + size_.height(),
				5.0,
				0.5, 0.1);

	glDisable(GL_BLEND);
}

void myDrawable2::keyPressEvent (KeyEvent* event)
{
}

void myDrawable2::contextMenuPressEvent (ContextMenuEvent* event)
{

}

void myDrawable2::contextMenuReleaseEvent (ContextMenuEvent* event)
{

}

void myDrawable2::mousePressEvent (MouseEvent* event)
{
}
	
void myDrawable2::mouseReleaseEvent (MouseEvent* event)
{
}

void myDrawable2::mouseMoveEvent (MouseEvent* event)
{

}

myDrawable3::myDrawable3(Traceable* parent)
	: Drawable (parent)
{
	round_box_type_ = RoundBoxAll;
}

myDrawable3::~myDrawable3()
{

}

void myDrawable3::render(void)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
		glColor4f(1.0f, 0.9f, 1.0f, 0.95f);

		drawRoundBox(pos_.x(),
				pos_.y(),
				pos_.x() + size_.width(),
				pos_.y() + size_.height(),
				5.0);
		DrawShadowBox(pos_.x(),
				pos_.y(),
				pos_.x() + size_.width(),
				pos_.y() + size_.height(),
				5.0,
				64);

	glDisable(GL_BLEND);
}


void myDrawable3::keyPressEvent (KeyEvent* event)
{
}

void myDrawable3::contextMenuPressEvent (ContextMenuEvent* event)
{

}

void myDrawable3::contextMenuReleaseEvent (ContextMenuEvent* event)
{

}

void myDrawable3::mousePressEvent (MouseEvent* event)
{

}
	
void myDrawable3::mouseReleaseEvent (MouseEvent* event)
{

}

void myDrawable3::mouseMoveEvent (MouseEvent* event)
{

}

DrawableTest::DrawableTest ()
{

}

DrawableTest::~DrawableTest ()
{

}

void DrawableTest::setUp ()
{

}

void DrawableTest::tearDown ()
{

}

void DrawableTest::texture1 ()
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

    myTexture widget;

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

void DrawableTest::mydrawable1()
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

    myDrawable1 widget;
    widget.set_pos(Point(50, 50));
    widget.resize(80, 40);

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

void DrawableTest::mydrawable2()
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

    myDrawable2 widget;
    widget.set_pos(Point(50, 50));
    widget.resize(80, 40);

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

void DrawableTest::mydrawable3()
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

    myDrawable3 widget;
    widget.set_pos(Point(50, 50));
    widget.resize(80, 40);

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

void DrawableTest::cbError (int error, const char* description)
{
	std::cerr << "Error: " << description
			<< " (error code: " << error << ")"
			<< std::endl;
}

void DrawableTest::cbWindowSize (GLFWwindow* window, int w, int h)
{
	BIL::Interface::instance()->resizeEvent(w, h);
}

void DrawableTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BIL::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void DrawableTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BIL::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void DrawableTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BIL::Interface::instance()->cursorPosEvent(xpos, ypos);
}

