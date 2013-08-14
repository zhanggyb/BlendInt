/* Test case for FontFace */

#ifndef _DRAWABLE_TEST_H
#define _DRAWABLE_TEST_H

#include <BIL/Application.h>
#include <BIL/Window.h>
#include <BIL/Drawable.h>

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

	void render (void);

	void KeyPressEvent (KeyEvent* event);

	void MousePressEvent (MouseEvent* event);
	
	void MouseReleaseEvent (MouseEvent* event);

	void MouseMoveEvent (MouseEvent* event);

	void InputMethodEvent (unsigned int character);

	void CursorEnterEvent (int entered);

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

	void render (void);

	void KeyPressEvent (KeyEvent* event);

	void MousePressEvent (MouseEvent* event);
	
	void MouseReleaseEvent (MouseEvent* event);
	
	void MouseMoveEvent (MouseEvent* event);

	void InputMethodEvent (unsigned int character);

	void CursorEnterEvent (int entered);
};

class myDrawable2: public Drawable
{
public:

	myDrawable2 (Traceable* parent = NULL);

	virtual ~myDrawable2 ();

	void render (void);

	void KeyPressEvent (KeyEvent* event);

	void MousePressEvent (MouseEvent* event);
	
	void MouseReleaseEvent (MouseEvent* event);

	void MouseMoveEvent (MouseEvent* event);

	void InputMethodEvent (unsigned int character);

	void CursorEnterEvent (int entered);
};

class myDrawable3: public Drawable
{
public:

	myDrawable3 (Traceable* parent = NULL);

	virtual ~myDrawable3 ();

	void render (void);

	void KeyPressEvent (KeyEvent* event);

	void MousePressEvent (MouseEvent* event);
	
	void MouseReleaseEvent (MouseEvent* event);

	void MouseMoveEvent (MouseEvent* event);

	void InputMethodEvent (unsigned int character);

	void CursorEnterEvent (int entered);
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

};

#endif  /* _DRAWABLE_TEST_H */
