/* Test case for FontFace */

#ifndef _DRAWABLE_TEST_H
#define _DRAWABLE_TEST_H

#include <BIL/Drawable.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace BIL;
using namespace std;

class myTexture: public Drawable
{
public:

	myTexture (Traceable *parent = NULL);

	virtual ~myTexture ();

protected:

	virtual void update () {}

	virtual void render ();

	virtual void keyPressEvent (KeyEvent* event);

	virtual void contextMenuPressEvent (ContextMenuEvent* event);

	virtual void contextMenuReleaseEvent (ContextMenuEvent* event);

	virtual void mousePressEvent (MouseEvent* event);
	
	virtual void mouseReleaseEvent (MouseEvent* event);

	virtual void mouseMoveEvent (MouseEvent* event);

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

	myDrawable1 (Traceable* parent = NULL);

	virtual ~myDrawable1 ();

protected:

	virtual void update () {}

	virtual void render ();

	virtual void keyPressEvent (KeyEvent* event);

	virtual void contextMenuPressEvent (ContextMenuEvent* event);

	virtual void contextMenuReleaseEvent (ContextMenuEvent* event);

	virtual void mousePressEvent (MouseEvent* event);
	
	virtual void mouseReleaseEvent (MouseEvent* event);
	
	virtual void mouseMoveEvent (MouseEvent* event);
};

class myDrawable2: public Drawable
{
public:

	myDrawable2 (Traceable* parent = NULL);

	virtual ~myDrawable2 ();

protected:

	virtual void update () {}

	virtual void render ();

	virtual void keyPressEvent (KeyEvent* event);

	virtual void contextMenuPressEvent (ContextMenuEvent* event);

	virtual void contextMenuReleaseEvent (ContextMenuEvent* event);

	virtual void mousePressEvent (MouseEvent* event);
	
	virtual void mouseReleaseEvent (MouseEvent* event);

	virtual void mouseMoveEvent (MouseEvent* event);
};

class myDrawable3: public Drawable
{
public:

	myDrawable3 (Traceable* parent = NULL);

	virtual ~myDrawable3 ();

protected:

	virtual void update () {}

	virtual void render ();

	virtual void keyPressEvent (KeyEvent* event);

	virtual void contextMenuPressEvent (ContextMenuEvent* event);

	virtual void contextMenuReleaseEvent (ContextMenuEvent* event);

	virtual void mousePressEvent (MouseEvent* event);
	
	virtual void mouseReleaseEvent (MouseEvent* event);

	virtual void mouseMoveEvent (MouseEvent* event);
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
