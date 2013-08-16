// cpp

#include <iostream>
#include <string>

#include <BIL/ChildrenList.hpp>

#include "ChildrenListTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (ChildrenListTest);

ChildrenListTest::ChildrenListTest ()
{

}

ChildrenListTest::~ChildrenListTest ()
{

}

void ChildrenListTest::setUp ()
{

}

void ChildrenListTest::tearDown ()
{

}

void ChildrenListTest::check1 (void)
{
	using namespace BIL;

	ChildrenList<int> list;

	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);

  CPPUNIT_ASSERT(list.size() == 4);
}

void ChildrenListTest::check2 (void)
{
	using namespace BIL;

	ChildrenList<int> list;

	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);

	CPPUNIT_ASSERT(list.find(2) == true);
}

void ChildrenListTest::check3 (void)
{
	using namespace BIL;

	ChildrenList<int> list;

	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);

	list.pop_back();
	list.pop_back();

	CPPUNIT_ASSERT(list.size() == 2);
}

void ChildrenListTest::check4 (void)
{
	using namespace BIL;

	ChildrenList<int> list;

	list.push_back(1);
	list.push_back(2);

	list.push_back(2); /* this should not success */
	list.push_back(2); /* this should not success either */

	CPPUNIT_ASSERT(list.size() == 2);
}

void ChildrenListTest::check5 (void)
{
	using namespace BIL;

	ChildrenList<int> list;

	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);

	list.erase(3);
	list.erase(4);

	CPPUNIT_ASSERT(list.size() == 2);
}

void ChildrenListTest::check6 (void)
{
	using namespace BIL;

	ChildrenList<int> list;

	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);

	list.clear();

	CPPUNIT_ASSERT(list.size() == 0);
}

void ChildrenListTest::check7 (void)
{
	using namespace BIL;

	ChildrenList<int> list;

	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);

	ChildrenList<int>::iterator it;

	for (it = list.begin(); it != list.end(); it++) {
		std::cout << (*it) << std::endl;
	}

	list.clear();

	CPPUNIT_ASSERT(list.size() == 0);
}

void ChildrenListTest::check8 (void)
{
	using namespace BIL;

	ChildrenList<int> list;

	ChildrenList<int>::iterator it;

	for (it = list.begin(); it != list.end(); it++) {
		std::cout << (*it) << std::endl;
	}

	list.clear();

	CPPUNIT_ASSERT(list.size() == 0);
}

void ChildrenListTest::check9 (void)
{
	using namespace BIL;

	ChildrenList<int> list1;
	ChildrenList<int> list2;

	list1.push_back(0);
	list1.push_back(1);
	list1.push_back(2);
	list1.push_back(3);
	list1.push_back(4);

	list2.push_back(5);
	list2.push_back(6);
	list2.push_back(7);

	ChildrenList<int>::iterator it;

	for (it = list1.begin(); it != list1.end(); it++) {
		std::cout << (*it) << std::endl;
	}

	for (it = list2.begin(); it != list2.end(); it++) {
		std::cout << (*it) << std::endl;
	}

	list1.clear();
	list2.clear();

	CPPUNIT_ASSERT(list2.size() == 0);
}
