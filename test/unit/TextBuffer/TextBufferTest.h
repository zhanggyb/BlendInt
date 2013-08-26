/* Test case for FontFace */

#ifndef _TEXTBUFFER_TEST_H
#define _TEXTBUFFER_TEST_H

#include <BIL/Application.hpp>
#include <BIL/Window.hpp>
#include <BIL/Drawable.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace BIL;
using namespace std;

class TextBufferTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(TextBufferTest);

	//CPPUNIT_TEST(showcharacter1);
	CPPUNIT_TEST(showtextline1);
	CPPUNIT_TEST(showtextline2);
	CPPUNIT_TEST(showtextline3);
	CPPUNIT_TEST(showtextline4);
	CPPUNIT_TEST(multiple_buf_show1);

	CPPUNIT_TEST(calculatebox1);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	TextBufferTest ();

	virtual ~TextBufferTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void showcharacter1 ();
	void showtextline1 ();
	void showtextline2 ();
	void showtextline3 ();
	void showtextline4 ();
	void multiple_buf_show1 ();
	void calculatebox1 ();

	void draw_grid (int width, int height);
};

#endif  /* _TEXTBUFFER_TEST_H */
