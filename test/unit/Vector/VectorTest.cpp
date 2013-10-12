// cpp

#include <iostream>
#include <string>

#include <BlendInt/Vector.hpp>

#include "VectorTest.h"

using namespace BlendInt;
using namespace std;

//CPPUNIT_TEST_SUITE_REGISTRATION (VectorTest);

VectorTest::VectorTest ()
{

}

VectorTest::~VectorTest ()
{

}

void VectorTest::setUp ()
{

}

void VectorTest::tearDown ()
{

}

void VectorTest::create1 ()
{
    Vector3<int> test;

    CPPUNIT_ASSERT((test[0] == 0) &&
                   (test.y() == 0) &&
                   (test[2] == 0));
}

void VectorTest::create2 ()
{
    Vector3<float> test;

    CPPUNIT_ASSERT((test.x() == 0.0) &&
                   (test.y() == 0.0) &&
                   (test.z() == 0.0));
}

void VectorTest::create3 ()
{
    Vector3<int> test;
    test[0] = 1; test[1] = 1; test[2] = 1;

    Vector3<int> dist (test);

    CPPUNIT_ASSERT((dist.x() == 1) &&
                   (dist.y() == 1) &&
                   (dist.z() == 1));
}


void VectorTest::create4 ()
{
    Vector3<int> test;
    test[0] = 2; test[1] = 2; test[2] = 2;
    Vector3<int> dist;
    dist = test;

    CPPUNIT_ASSERT((dist.x() == 2) &&
                   (dist.y() == 2) &&
                   (dist.z() == 2));
}


void VectorTest::create5 ()
{
    Vector3<int> test (1, 1, 1);
    Vector3<int> dist (1, 1, 1);

    CPPUNIT_ASSERT(test == dist);
}

void VectorTest::create6 ()
{
    Vector2<int> test (1, 1);
    Vector2<int> dist;

    	dist[0] = test[0];
    	dist[1] = test[1];

    CPPUNIT_ASSERT(test == dist);
}

void VectorTest::create7 ()
{
    Vector4<int> test (4, 4, 4, 4);
    Vector4<int> dist;

    	dist[0] = test[0];
    	dist[1] = test[1];
    	dist[2] = test[2];
    		dist[5] = test[5];	// should work

    CPPUNIT_ASSERT(test == dist);
}

void VectorTest::checkdefault1 ()
{
    Vector3<int> test (1, 1, 1);

    CPPUNIT_ASSERT(test.x() == 1);
}

