// cpp

#include <BIL/Tuple.h>
#include <iostream>
#include <string>

#include "TupleTest.h"

using namespace BIL;
using namespace std;

// CPPUNIT_TEST_SUITE_REGISTRATION (TupleTest);

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
    Vec3i test;

    CPPUNIT_ASSERT((test.data[0] == 0) &&
                   (test.coord.y == 0) &&
                   (test.coord.z == 0));
}

void TupleTest::create2 ()
{
    Coord3f test;

    CPPUNIT_ASSERT((test.coord.x == 0.0) &&
                   (test.coord.y == 0.0) &&
                   (test.coord.z == 0.0));
}

void TupleTest::create3 ()
{
    Coord3f test(1.0, 1.0, 1.0);
    Coord3f dist (test);

    CPPUNIT_ASSERT((dist.coord.x == 1.0) &&
                   (dist.coord.y == 1.0) &&
                   (dist.coord.z == 1.0));
}


void TupleTest::create4 ()
{
    Coord3f test(2.0, 2.0, 2.0);
    Coord3f dist;
    dist = test;

    CPPUNIT_ASSERT((dist.coord.x == 2.0) &&
                   (dist.coord.y == 2.0) &&
                   (dist.coord.z == 2.0));
}


void TupleTest::create5 ()
{
    Coord3f test (1.0, 1.0, 1.0);
    Coord3f dist (1.0, 1.0, 1.0);

    CPPUNIT_ASSERT(test == dist);
}
