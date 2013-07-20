// cpp

#include <BIL/BasicObject.h>
#include <iostream>
#include <string>

#include "BasicObjectTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (BasicObjectTest);

BasicObjectTest::BasicObjectTest ()
{

}

BasicObjectTest::~BasicObjectTest ()
{

}

void BasicObjectTest::setUp ()
{

}

void BasicObjectTest::tearDown ()
{

}

void BasicObjectTest::create1 ()
{
	BasicObject obj;	// id == 1

	uint64_t id = obj.getId();

	cout << "id: " << id << endl;

    CPPUNIT_ASSERT(true);
}

void BasicObjectTest::remove ()
{
	BasicObject obj1, obj2, obj3;

    obj2.setParent(&obj1);
    obj3.setParent(&obj1);

    cout << "id: obj1.getId()" << endl;
    cout << "id: obj2.getId()" << endl;
    cout << "id: obj3.getId()" << endl;
    
	CPPUNIT_ASSERT(true);
}

