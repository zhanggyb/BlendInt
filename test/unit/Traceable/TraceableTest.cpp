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
	int mapsize = Traceable::mapSize();

	CPPUNIT_ASSERT(mapsize == 0);
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

	int solosize = Traceable::getList().size();

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

	cout << endl << "map size: "<< Traceable::mapSize() << endl;

	cout << "solo list size: " << Traceable::getList().size() << endl;

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

	cout << endl << "map size: "<< Traceable::mapSize() << endl;

	cout << "solo list size: " << Traceable::getList().size() << endl;

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

	cout << endl << "map size: "<< Traceable::mapSize() << endl;

	cout << "solo list size: " << Traceable::getList().size() << endl;

  CPPUNIT_ASSERT(sizebefore == 1 && sizeafter == 2 && mapsize == 3);
}

void TraceableTest::checkparent4 (void)
{
	using namespace BIL;

#ifdef DEBUG
	// Traceable::reset();	// reset the map
#endif

	Traceable *obj1 = NULL;
	Traceable *obj2 = NULL;
	Traceable *obj3 = NULL;
	Traceable *obj4 = NULL;

	obj1 = new Traceable;
	obj2 = new Traceable(obj1);
	obj3 = new Traceable(obj1);
	obj4 = new Traceable(obj1);

	delete obj1; /* this should also delete all children */

	int mapsize = Traceable::mapSize();
	cout << endl << "map size: "<< mapsize << endl;

	int solosize = Traceable::getList().size();
	cout << "solo list size: " << solosize << endl;

	CPPUNIT_ASSERT (mapsize == 0 && solosize == 0);
}

void TraceableTest::checkparent5 (void)
{
	using namespace BIL;

#ifdef DEBUG
	// Traceable::reset();	// reset the map
#endif

	Traceable *obj1 = new Traceable;
	Traceable *obj2 = new Traceable;

	obj2->setParent(obj1);

	delete obj1;

	int mapsize = Traceable::mapSize();
	int solosize = Traceable::getList().size();

	cout << endl << "map size: "<< mapsize << endl;
	cout << "solo list size: " << solosize << endl;

	CPPUNIT_ASSERT(mapsize == 0 && solosize == 0);
}

void TraceableTest::checkparent6 (void)
{
	using namespace BIL;

#ifdef DEBUG
//	Traceable::reset();	// reset the map
#endif

	Traceable *obj1 = new Traceable;
	Traceable *obj2 = new Traceable;

	obj1->addChild(obj2);

	delete obj1;

	int mapsize = Traceable::mapSize();
	int solosize = Traceable::getList().size();

	cout << endl << "map size: "<< mapsize << endl;
	cout << "solo list size: " << solosize << endl;

	CPPUNIT_ASSERT(mapsize == 0 && solosize == 0);
}

void TraceableTest::checkparent7 (void)
{
	using namespace BIL;

#ifdef DEBUG
//	Traceable::reset();	// reset the map
#endif

	Traceable *obj1 = new Traceable;
	Traceable *obj2 = new Traceable;
	Traceable *obj3 = new Traceable;
	Traceable *obj4 = new Traceable;
	Traceable *obj5 = new Traceable;
	Traceable *obj6 = new Traceable;

	obj2->setParent(obj1);
	obj3->setParent(obj2);
	obj4->setParent(obj3);
	obj5->setParent(obj2);
	obj6->setParent(obj1);

	delete obj1;

	int mapsize = Traceable::mapSize();
	int solosize = Traceable::getList().size();

	cout << endl << "map size: "<< mapsize << endl;
	cout << "solo list size: " << solosize << endl;

	CPPUNIT_ASSERT(mapsize == 0 && solosize == 0);
}

void TraceableTest::checkparent8 (void)
{
	using namespace BIL;

#ifdef DEBUG
	Traceable::reset();	// reset the map
#endif

	Traceable *obj1 = new Traceable;	// id = 1
	Traceable *obj2 = new Traceable;	// id = 2
	Traceable *obj3 = new Traceable;	// id = 3
	Traceable *obj4 = new Traceable;	// id = 4
	Traceable *obj5 = new Traceable;	// id = 5
	Traceable *obj6 = new Traceable;	// id = 6

	obj2->setParent(obj1);
	obj3->setParent(obj2);

	obj4->addChild(obj5);
	obj4->addChild(obj6);
	obj4->addChild(obj3);

	delete obj1;	// now only 4 objects

	int mapsize = Traceable::mapSize();
	int solosize = Traceable::getList().size();

	cout << endl << "map size: "<< mapsize << endl;
	cout << "solo list size: " << solosize << endl;

	list<Traceable*>& tlist = Traceable::getList();
	list<Traceable*>::iterator it;
	for (it = tlist.begin(); it != tlist.end(); it++) {
		cout << (*it)->getId() << endl;
	}

	map<uint64_t, Traceable*>& tmap = Traceable::getMap();
	map<uint64_t, Traceable*>::iterator j;
	for (j = tmap.begin(); j != tmap.end(); j++) {
		cout << j->second->getId() << endl;
	}

	//CPPUNIT_ASSERT(mapsize == 4 && solosize == 1);

	delete obj4;

	mapsize = Traceable::mapSize();
	solosize = Traceable::getList().size();

		cout << endl << "map size: "<< mapsize << endl;
		cout << "solo list size: " << solosize << endl;

	CPPUNIT_ASSERT(mapsize == 0 && solosize == 0);
}
