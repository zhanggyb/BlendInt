// cpp

#include <iostream>
#include <string>

#include <BIL/Traceable.h>

#include "TraceableTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (TraceableTest);

TraceableTest::TraceableTest ()
{

}

TraceableTest::~TraceableTest ()
{

}

void TraceableTest::setUp ()
{

}

void TraceableTest::tearDown ()
{

}

void TraceableTest::checksolo1 ()
{
#ifdef DEBUG
	Traceable::reset();
#endif

	Traceable obj;	// id == 1

	uint64_t id = obj.getId();

	cout << "id: " << id << endl;

    CPPUNIT_ASSERT(Traceable::getList().size() == 1);
}

void TraceableTest::checksolo2 ()
{
#ifdef DEBUG
	Traceable::reset();
#endif

	Traceable* obj = new Traceable;	// id == 1

	uint64_t id = obj->getId();

	cout << "id: " << id << endl;

	delete obj;
	obj = NULL;

    CPPUNIT_ASSERT(Traceable::getList().size() == 0);
}

void TraceableTest::checkparent1 ()
{
#ifdef DEBUG
	Traceable::reset();
#endif

	Traceable* parent = new Traceable;	// id == 1
	Traceable* child = new Traceable(parent);	// id == 2

	int sizebefore = Traceable::getList().size(); // 1

	child->setParent(NULL);

	int sizeafter = Traceable::getList().size();	// 2

	delete child; delete parent;

  CPPUNIT_ASSERT(sizebefore == 1 && sizeafter == 2);
}

void TraceableTest::checkparent2 ()
{
#ifdef DEBUG
	Traceable::reset();
#endif

	Traceable* parent = new Traceable;	// id == 1
	Traceable* child = new Traceable(parent);	// id == 2

	int sizebefore = Traceable::getList().size(); // 1

	int mapsize = Traceable::mapSize();	// 2

	child->setParent(NULL);

	int sizeafter = Traceable::getList().size();	// 2

	delete child; delete parent;

  CPPUNIT_ASSERT(sizebefore == 1 && sizeafter == 2 && mapsize == 2);
}

void TraceableTest::checkparent3 ()
{
#ifdef DEBUG
	Traceable::reset();
#endif

	Traceable* parent = new Traceable;	// id == 1
	Traceable* child1 = new Traceable(parent);	// id == 2
	Traceable* child2 = new Traceable(parent);	// id == 3

	int sizebefore = Traceable::getList().size(); // 1

	int mapsize = Traceable::mapSize();	// 3

	child1->setParent(NULL);
	child2->setParent(NULL);
	child1->setParent(parent);

	int sizeafter = Traceable::getList().size();	// 2

	delete child1; delete parent; delete child2;

  CPPUNIT_ASSERT(sizebefore == 1 && sizeafter == 2 && mapsize == 3);
}


void TraceableTest::create1 ()
{
	Traceable obj;	// id == 1

	uint64_t id = obj.getId();

	cout << "id: " << id << endl;

    CPPUNIT_ASSERT(true);
}

void TraceableTest::remove ()
{
	Traceable obj1, obj2, obj3;

    obj2.setParent(&obj1);
    obj3.setParent(&obj1);

    cout << "id: obj1.getId()" << endl;
    cout << "id: obj2.getId()" << endl;
    cout << "id: obj3.getId()" << endl;
    
	CPPUNIT_ASSERT(true);
}

