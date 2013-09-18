/* Test case for FontFace */

#ifndef _DRAWABLE_TEST_H
#define _DRAWABLE_TEST_H

#include <BILO/Drawable.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace BILO;
using namespace std;

class myTexture: public Drawable
{
public:

	myTexture (Drawable *parent = 0);

	virtual ~myTexture ();

protected:

	virtual void update (int property) {}

	virtual void render ();

	virtual void press_key (KeyEvent* event);

	virtual void press_context_menu (ContextMenuEvent* event);

	virtual void release_context_menu (ContextMenuEvent* event);

	virtual void press_mouse (MouseEvent* event);
	
	virtual void release_mouse (MouseEvent* event);

	virtual void move_mouse (MouseEvent* event);

private:
	void makeCheckImage (void);

	static const int checkImageWidth = 64;
	static const int checkImageHeight = 64;

	GLuint _texName;
	GLubyte _checkImage[checkImageHeight][checkImageWidth][4];
};

class myDrawable1: public Drawable
{
public:

	myDrawable1 (Drawable* parent = 0);

	virtual ~myDrawable1 ();

protected:

	virtual void update (int property) {}

	virtual void render ();

	virtual void press_key (KeyEvent* event);

	virtual void press_context_menu (ContextMenuEvent* event);

	virtual void release_context_menu (ContextMenuEvent* event);

	virtual void press_mouse (MouseEvent* event);
	
	virtual void release_mouse (MouseEvent* event);
	
	virtual void move_mouse (MouseEvent* event);
};

class myDrawable2: public Drawable
{
public:

	myDrawable2 (Drawable* parent = 0);

	virtual ~myDrawable2 ();

protected:

	virtual void update (int property) {}

	virtual void render ();

	virtual void press_key (KeyEvent* event);

	virtual void press_context_menu (ContextMenuEvent* event);

	virtual void release_context_menu (ContextMenuEvent* event);

	virtual void press_mouse (MouseEvent* event);
	
	virtual void release_mouse (MouseEvent* event);

	virtual void move_mouse (MouseEvent* event);
};

class myDrawable3: public Drawable
{
public:

	myDrawable3 (Drawable* parent = 0);

	virtual ~myDrawable3 ();

protected:

	virtual void update (int property) {}

	virtual void render ();

	virtual void press_key (KeyEvent* event);

	virtual void press_context_menu (ContextMenuEvent* event);

	virtual void release_context_menu (ContextMenuEvent* event);

	virtual void press_mouse (MouseEvent* event);
	
	virtual void release_mouse (MouseEvent* event);

	virtual void move_mouse (MouseEvent* event);
};

class DrawableTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(DrawableTest);

	CPPUNIT_TEST(texture1);
	CPPUNIT_TEST(mydrawable1);
	CPPUNIT_TEST(mydrawable2);
	CPPUNIT_TEST(mydrawable3);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	DrawableTest ();

	virtual ~DrawableTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void texture1 ();
	void mydrawable1 ();
	void mydrawable2 ();
	void mydrawable3 ();

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);
};

#endif  /* _DRAWABLE_TEST_H */
