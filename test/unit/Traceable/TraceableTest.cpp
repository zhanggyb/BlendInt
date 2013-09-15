// cpp

#include <iostream>
#include <string>

#include <BIL/Traceable.hpp>

#include "TraceableTest.h"

using namespace BIL;
using namespace std;

//CPPUNIT_TEST_SUITE_REGISTRATION (TraceableTest);

TraceableTest::TraceableTest ()
{

}

TraceableTest::~TraceableTest ()
{

}

void TraceableTest::setUp ()
{
	int mapsize = Traceable::mapSize();

	if(mapsize > 0) {
		map<uint64_t, Traceable*>::const_iterator it;
        for (it = Traceable::getMap().begin(); it != Traceable::getMap().end(); it++)
		{
			cout << "id: " << it->first << " was not deleted!" << endl;
		}
	}

	CPPUNIT_ASSERT(mapsize == 0);
}

void TraceableTest::tearDown ()
{
	int mapsize = Traceable::mapSize();

	if(mapsize > 0) {
		map<uint64_t, Traceable*>::const_iterator it;
		for (it = Traceable::getMap().begin(); it != Traceable::getMap().end(); it++)
		{
			cout << "id: " << it->first << " was not deleted!" << endl;
		}
	}

	CPPUNIT_ASSERT(mapsize == 0);
}

void TraceableTest::checkparent1 ()
{
#ifdef DEBUG
	Traceable::reset();
#endif

	Traceable* parent = new Traceable;	// id == 1
	Traceable* child = new Traceable(parent);	// id == 2

	int sizebefore = Traceable::getMap().size(); // 1

	child->set_parent(0);

    int sizeafter = Traceable::getMap().size();	// 2

	delete child; delete parent;

	cout << endl << "map size: "<< Traceable::mapSize() << endl;

  CPPUNIT_ASSERT(sizebefore == 2 && sizeafter == 2);
}

void TraceableTest::checkparent2 ()
{
#ifdef DEBUG
	Traceable::reset();
#endif

	Traceable* parent = new Traceable;	// id == 1
	Traceable* child = new Traceable(parent);	// id == 2

	int mapsize = Traceable::mapSize();	// 2

	child->set_parent(0);

	delete child; delete parent;

	cout << endl << "map size: "<< Traceable::mapSize() << endl;

  CPPUNIT_ASSERT(mapsize == 2);
}

void TraceableTest::checkparent3 ()
{
#ifdef DEBUG
	Traceable::reset();
#endif

	Traceable* parent = new Traceable;	// id == 1
	Traceable* child1 = new Traceable(parent);	// id == 2
	Traceable* child2 = new Traceable(parent);	// id == 3

	int mapsize = Traceable::mapSize();	// 3

	child1->set_parent(0);
	child2->set_parent(0);
	child1->set_parent(0);

	delete child1; delete parent; delete child2;

	cout << endl << "map size: "<< Traceable::mapSize() << endl;

  CPPUNIT_ASSERT(mapsize == 3);
}

void TraceableTest::checkparent4 (void)
{
	using namespace BIL;

#ifdef DEBUG
	// Traceable::reset();	// reset the map
#endif

	Traceable* obj1 = new Traceable;
	Traceable* obj2 = new Traceable(obj1);
	Traceable* obj3 = new Traceable(obj1);
	Traceable* obj4 = new Traceable(obj1);

	delete obj1; /* this should also delete all children */

	int mapsize = Traceable::mapSize();
	cout << endl << "map size: "<< mapsize << endl;

	CPPUNIT_ASSERT (mapsize == 0);
}

void TraceableTest::checkparent5 (void)
{
	using namespace BIL;

#ifdef DEBUG
	// Traceable::reset();	// reset the map
#endif

	Traceable *obj1 = new Traceable;
	Traceable *obj2 = new Traceable;

	obj2->set_parent(obj1);

	delete obj1;

	int mapsize = Traceable::mapSize();

	cout << endl << "map size: "<< mapsize << endl;

	CPPUNIT_ASSERT(mapsize == 0);
}

void TraceableTest::checkparent6 (void)
{
	using namespace BIL;

#ifdef DEBUG
//	Traceable::reset();	// reset the map
#endif

	Traceable *obj1 = new Traceable;
	Traceable *obj2 = new Traceable;

	obj1->add_child(obj2);

	delete obj1;

	int mapsize = Traceable::mapSize();

	cout << endl << "map size: "<< mapsize << endl;

	CPPUNIT_ASSERT(mapsize == 0);
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

	obj2->set_parent(obj1);
	obj3->set_parent(obj2);
	obj4->set_parent(obj3);
	obj5->set_parent(obj2);
	obj6->set_parent(obj1);

	delete obj1;

	int mapsize = Traceable::mapSize();

	cout << endl << "map size: "<< mapsize << endl;

	CPPUNIT_ASSERT(mapsize == 0);
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

	obj2->set_parent(obj1);
	obj3->set_parent(obj2);

	obj4->add_child(obj5);
	obj4->add_child(obj6);
	obj4->add_child(obj3);

	delete obj1;	// now only 4 objects

	int mapsize = Traceable::mapSize();

	cout << endl << "map size: "<< mapsize << endl;

	const map<uint64_t, Traceable*>& tmap = Traceable::getMap();
	map<uint64_t, Traceable*>::const_iterator j;
	for (j = tmap.begin(); j != tmap.end(); j++) {
		cout << j->second->id() << endl;
	}

	//CPPUNIT_ASSERT(mapsize == 4 && solosize == 1);

	delete obj4;

	mapsize = Traceable::mapSize();

		cout << endl << "map size: "<< mapsize << endl;

	CPPUNIT_ASSERT(mapsize == 0);
}

void TraceableTest::checkparent9 (void)
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
	Traceable obj7;
	Traceable obj8;

	obj2->set_parent(obj1);
	obj3->set_parent(obj2);

	obj4->add_child(obj5);
	obj4->add_child(obj6);
	obj4->add_child(obj3);

	obj7.add_child(obj4);
	obj7.add_child(&obj8);
	obj8.set_parent(&obj7);

	int mapsize = Traceable::mapSize();

	cout << endl << "map size: "<< mapsize << endl; // 8

	const map<uint64_t, Traceable*>& tmap = Traceable::getMap();
	map<uint64_t, Traceable*>::const_iterator j;
	for (j = tmap.begin(); j != tmap.end(); j++) {
		cout << j->second->id() << endl;
	}

	//CPPUNIT_ASSERT(mapsize == 4 && solosize == 1);

	delete obj4; delete obj1;

	mapsize = Traceable::mapSize();

	cout << endl << "map size: "<< mapsize << endl; // 2

	CPPUNIT_ASSERT(mapsize == 2);
}

void TraceableTest::checkparent10 ()
{
	using namespace BIL;

#ifdef DEBUG
	Traceable::reset();	// reset the map
#endif

	Traceable* obj[100] = {0};
	for(int i; i < 100; i++)
	{
		obj[i] = new Traceable;

		if (i < 1) continue;
		
		obj[i]->set_parent(obj[i-1]);
	}

	Traceable root;
	root.add_child(obj[0]);

	int mapsize = Traceable::mapSize();

	cout << endl << "map size: "<< mapsize << endl; // 8

	CPPUNIT_ASSERT(mapsize == 101); // check tearDown()
}

void TraceableTest::solotest1 ()
{
	Traceable a;
	Traceable b;
	Traceable c;
	Traceable d;
	int count = 0;

	cout << endl;

	count = Traceable::getMap().size();

	CPPUNIT_ASSERT(count == 4);
}

void TraceableTest::solotest2 ()
{
	Traceable* a = new Traceable();
	Traceable* b = new Traceable();
	Traceable* c = new Traceable();
	Traceable* d = new Traceable();
	int count = 0;
	int countb = 0;

	cout << endl;

  count = Traceable::getMap().size();
	cout << "count: " << count << endl;

	delete c; delete d;

    countb = Traceable::getMap().size();
	cout << "countb: " << countb << endl;

	delete a; delete b;
	CPPUNIT_ASSERT((count == 4) && (countb == 2));
}
