/* Test case for FontFace */

#ifndef _ABSTRACTBUTTON_TEST_H
#define _ABSTRACTBUTTON_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace std;

class AbstractButtonTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(AbstractButtonTest);

	CPPUNIT_TEST(show1);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	AbstractButtonTest ();

	virtual ~AbstractButtonTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void show1 ();
};

#endif  /* _ABSTRACTBUTTON_TEST_H */
