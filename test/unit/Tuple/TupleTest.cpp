// cpp

#include <iostream>
#include <string>

#include <BIL/Tuple.hpp>

#include "TupleTest.h"

using namespace BIL;
using namespace std;

//CPPUNIT_TEST_SUITE_REGISTRATION (TupleTest);

TupleTest::TupleTest ()
{

}

TupleTest::~TupleTest ()
{

}

void TupleTest::setUp ()
{

}

void TupleTest::tearDown ()
{

}

void TupleTest::create1 ()
{
    Tuple3i test;

    CPPUNIT_ASSERT((test.data[0] == 0) &&
                   (test.coord.y == 0) &&
                   (test.coord.z == 0));
}

void TupleTest::create2 ()
{
    Tuple3f test;

    CPPUNIT_ASSERT((test.coord.x == 0.0) &&
                   (test.coord.y == 0.0) &&
                   (test.coord.z == 0.0));
}

void TupleTest::create3 ()
{
    Tuple3f test(1.0, 1.0, 1.0);
    Tuple3f dist (test);

    CPPUNIT_ASSERT((dist.coord.x == 1.0) &&
                   (dist.coord.y == 1.0) &&
                   (dist.coord.z == 1.0));
}


void TupleTest::create4 ()
{
    Tuple3f test(2.0, 2.0, 2.0);
    Tuple3f dist;
    dist = test;

    CPPUNIT_ASSERT((dist.coord.x == 2.0) &&
                   (dist.coord.y == 2.0) &&
                   (dist.coord.z == 2.0));
}


void TupleTest::create5 ()
{
    Tuple3f test (1.0, 1.0, 1.0);
    Tuple3f dist (1.0, 1.0, 1.0);

    CPPUNIT_ASSERT(test == dist);
}

void TupleTest::checkdefault1 ()
{
    RGBAf test (1.0, 1.0, 1.0);

    CPPUNIT_ASSERT(test.rgba.a == 1.0);
}

void TupleTest::testRGBA1()
{
	RGBA color(0xFF00FF);

	unsigned char r = color.rgba.r;
	unsigned char g = color.rgba.g;
	unsigned char b = color.rgba.b;
	unsigned char a = color.rgba.a;

	CPPUNIT_ASSERT((r == 0xFF) &&
				   (g == 0x00) &&
				   (b == 0xFF) &&
				   (a == 0xFF));
}

void TupleTest::testRGBA2()
{
	RGBA color;

	color = 0x1A2B3C4D;

	unsigned char r = color.rgba.r;
	unsigned char g = color.rgba.g;
	unsigned char b = color.rgba.b;
	unsigned char a = color.rgba.a;

	CPPUNIT_ASSERT((r == 0x1A) &&
				   (g == 0x2B) &&
				   (b == 0x3C) &&
				   (a == 0x4D));
}
