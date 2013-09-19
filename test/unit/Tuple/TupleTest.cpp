// cpp

#include <iostream>
#include <string>

#include <BILO/Tuple.hpp>

#include "TupleTest.h"

using namespace BILO;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (TupleTest);

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
