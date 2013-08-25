/* Test case for FontFace */

#ifndef _BUTTON_TEST_H
#define _BUTTON_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace std;

class ButtonTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(ButtonTest);

	CPPUNIT_TEST(show1);
	CPPUNIT_TEST(toggle_button_show1);


	CPPUNIT_TEST_SUITE_END()
	;

public:

	ButtonTest ();

	virtual ~ButtonTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void show1 ();

	void toggle_button_show1 ();
};

#endif  /* _BUTTON_TEST_H */
