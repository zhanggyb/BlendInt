/* Test case for FontFace */

#ifndef _DRAWABLE_TEST_H
#define _DRAWABLE_TEST_H

#include <BlendInt/AbstractForm.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace BlendInt;
using namespace std;

class myTexture: public AbstractForm
{
public:

	myTexture (AbstractForm *parent = 0);

	virtual ~myTexture ();

protected:

	virtual void update (int property_type) {}

	virtual bool update (int type, const void* property) {return true;}

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

class myDrawable1: public AbstractForm
{
public:

	myDrawable1 (AbstractForm* parent = 0);

	virtual ~myDrawable1 ();

protected:

	virtual void update (int property_type) {}

	virtual bool update (int type, const void* property) {return true;}

	virtual void render ();

	virtual void press_key (KeyEvent* event);

	virtual void press_context_menu (ContextMenuEvent* event);

	virtual void release_context_menu (ContextMenuEvent* event);

	virtual void press_mouse (MouseEvent* event);
	
	virtual void release_mouse (MouseEvent* event);
	
	virtual void move_mouse (MouseEvent* event);
};

class myDrawable2: public AbstractForm
{
public:

	myDrawable2 (AbstractForm* parent = 0);

	virtual ~myDrawable2 ();

protected:

	virtual void update (int property_type) {}

	virtual bool update (int type, const void* property) {return true;}

	virtual void render ();

	virtual void press_key (KeyEvent* event);

	virtual void press_context_menu (ContextMenuEvent* event);

	virtual void release_context_menu (ContextMenuEvent* event);

	virtual void press_mouse (MouseEvent* event);
	
	virtual void release_mouse (MouseEvent* event);

	virtual void move_mouse (MouseEvent* event);
};

class myDrawable3: public AbstractForm
{
public:

	myDrawable3 (AbstractForm* parent = 0);

	virtual ~myDrawable3 ();

protected:

	virtual void update (int property_type) {}

	virtual bool update (int type, const void* property) {return true;}

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

	//CPPUNIT_TEST(texture1);
	//CPPUNIT_TEST(mydrawable1);
	//CPPUNIT_TEST(mydrawable2);
	//CPPUNIT_TEST(mydrawable3);

	CPPUNIT_TEST(bind_test1);
	CPPUNIT_TEST(bind_test2);
	CPPUNIT_TEST(bind_test3);
	CPPUNIT_TEST(bind_test4);
	CPPUNIT_TEST(bind_test5);
	CPPUNIT_TEST(bind_test6);
	CPPUNIT_TEST(bind_test7);

	//CPPUNIT_TEST(bind_test8);
	//CPPUNIT_TEST(bind_test9);
	//CPPUNIT_TEST(bind_test10);

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

	// the following functions use Widget to test bind/unbind
	void bind_test1 ();
	void bind_test2 ();
	void bind_test3 ();
	void bind_test4 ();
	void bind_test5 ();
	void bind_test6 ();
	void bind_test7 ();
	void bind_test8 ();
	void bind_test9 ();
	void bind_test10 ();
	void bind_test11 ();
	void bind_test12 ();
	void bind_test13 ();
	void bind_test14 ();
	void bind_test15 ();
	void bind_test16 ();
	void bind_test17 ();
	void bind_test18 ();
	void bind_test19 ();

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);
};

#endif  /* _DRAWABLE_TEST_H */
