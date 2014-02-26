#include "ObjectTest1.hpp"
#include <BlendInt/Core/Object.hpp>

#include <iostream>

#include <list>
#include <set>

using namespace BlendInt;

ObjectTest1::ObjectTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ObjectTest1::~ObjectTest1()
{
	// TODO: add destructor code
}

/**
 * test new Object 
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, RefPtr1)
{
    RefPtr<Object> obj1 = Object::Create("obj1");

    RefPtr<Object> obj2;

	ASSERT_FALSE(obj2);

    obj2 = obj1;

    RefPtr<Object> obj3(obj2);

	ASSERT_TRUE(obj3->count() == 3);
}

/**
 *
 *
 */
TEST_F(ObjectTest1, RefPtr2)
{
    RefPtr<Object> obj1 = Object::Create("obj1");

    RefPtr<Object> obj2 = Object::Create("obj2");

    obj2 = obj1;

    RefPtr<Object> obj3(obj2);

	ASSERT_TRUE(obj3->count() == 3);
}

/**
 * Test RefPtr in container
 */
TEST_F(ObjectTest1, RefPtr3)
{
    std::list< RefPtr<Object> > test_list;

    test_list.push_back(Object::Create("obj1"));
    test_list.push_back(Object::Create("obj2"));

    test_list.clear();
	
    ASSERT_TRUE(true);
}

/**
 * Test RefPtr in std::set
 */
TEST_F(ObjectTest1, RefPtr4)
{
    std::set< RefPtr<Object> > test_set;

    test_set.insert(Object::Create("obj1"));
    test_set.insert(Object::Create("obj2"));

    test_set.clear();
	
    ASSERT_TRUE(true);
}

/**
 * Test RefPtr::destroy
 */
TEST_F(ObjectTest1, RefPtr5)
{
    RefPtr<Object> obj;

    obj = Object::Create("Object");

    obj.destroy();
	
    ASSERT_TRUE(true);
}

