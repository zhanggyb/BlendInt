// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include <BIL/Font.hpp>

#include "FontTest.h"

using namespace BILO;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(FontTest);

FontTest::FontTest ()
{

}

FontTest::~FontTest ()
{

}

void FontTest::setUp ()
{
}

void FontTest::tearDown ()
{
}

void FontTest::check1 ()
{
    bool result = false;

    Font a("Sans");
    Font b("Sansb");

    result = a < b;

	CPPUNIT_ASSERT (result);
}

void FontTest::check2 ()
{
    bool result = false;

    Font a("Sans");
    Font b("Sans");

    result = a < b;

	CPPUNIT_ASSERT (!result);
}

void FontTest::check3 ()
{
    bool result = false;

    Font a("Sans", 12);
    Font b("Sans", 10);

    result = b < a;
    if (result) {
        result = (a == b);
    }

	CPPUNIT_ASSERT (result);
}

void FontTest::check4 ()
{
    bool result = false;

    Font a("Droid Sans", 12);
    Font b("Sans", 10);

    result = a < b;

	CPPUNIT_ASSERT (result);
}

void FontTest::check5 ()
{
    bool result = false;

    Font a("Sans", 10, true);
    Font b("Sans", 10, false);

    result = b < a;

	CPPUNIT_ASSERT (result);

}

void FontTest::check6 ()
{
    bool result = false;

    Font a("Sans", 10, true, false);
    Font b("Sans", 10, true, true);

    result = a < b;

	CPPUNIT_ASSERT (result);
}

void FontTest::check7 ()
{
    bool result = false;

    Font a("Droid Sans");
    Font b("Droid Sans", 50);

    result = a < b;

	CPPUNIT_ASSERT (result);
}
