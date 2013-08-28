/* Test case for FontFace */

#ifndef _CPPEVENTS_TEST_H
#define _CPPEVENTS_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

#include <BIL/Button.hpp>
#include <BIL/Label.hpp>

using namespace std;

class CppEventsTest: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(CppEventsTest);

	CPPUNIT_TEST(connect1);

	CPPUNIT_TEST_SUITE_END();

public:

	CppEventsTest ();

	virtual ~CppEventsTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void connect1 ();

	Cpp::ConnectionScope scope_;

};

#endif  /* _CPPEVENTS_TEST_H */
