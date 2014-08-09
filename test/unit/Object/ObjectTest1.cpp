#include "ObjectTest1.hpp"
#include <BlendInt/Core/Object.hpp>

#include <iostream>

#include <list>
#include <set>

#include "WidgetSim.hpp"

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
    RefPtr<Object> obj1(new Object);

    RefPtr<Object> obj2;

	ASSERT_FALSE(obj2);

    obj2 = obj1;

    RefPtr<Object> obj3(obj2);

	ASSERT_TRUE(obj3->reference_count() == 3);
}

/**
 *
 *
 */
TEST_F(ObjectTest1, RefPtr2)
{
    RefPtr<Object> obj1(new Object);

    RefPtr<Object> obj2(new Object);

    obj2 = obj1;

    RefPtr<Object> obj3(obj2);

	ASSERT_TRUE(obj3->reference_count() == 3);
}

/**
 * Test RefPtr in container
 */
TEST_F(ObjectTest1, RefPtr3)
{
    std::list< RefPtr<Object> > test_list;
    RefPtr<Object> obj1(new Object);
    RefPtr<Object> obj2(new Object);

    test_list.push_back(obj1);
    test_list.push_back(obj2);

    test_list.clear();
	
    ASSERT_TRUE(true);
}

/**
 * Test RefPtr in std::set
 */
TEST_F(ObjectTest1, RefPtr4)
{
    std::set< RefPtr<Object> > test_set;
    RefPtr<Object> obj1(new Object);
    RefPtr<Object> obj2(new Object);
    RefPtr<Object> obj3(new Object);

    test_set.insert(obj1);
    test_set.insert(obj2);
    test_set.insert(obj3);

    test_set.clear();
	
    ASSERT_TRUE(true);
}

/**
 * Test RefPtr::destroy
 */
TEST_F(ObjectTest1, RefPtr5)
{
    RefPtr<Object> obj (new Object);

    obj.destroy();
	
    ASSERT_TRUE(true);
}

/**
 * Test RefPtr::destroy
 */
TEST_F(ObjectTest1, RefPtr6)
{
    RefPtr<Object> obj1(new Object);
    RefPtr<Object> obj2(new Object);

    std::set< RefPtr<Object> > test_set;

    test_set.insert(obj1);
    test_set.insert(obj2);

    std::cout << "obj1 count: " << obj1->reference_count() << std::endl;

    test_set.erase(obj2);

    std::cout << "obj2 count: " << obj2->reference_count() << std::endl;

    ASSERT_TRUE(true);
}

/**
 * Test RefPtr cast functions
 */
TEST_F(ObjectTest1, RefPtr7)
{
    RefPtr<WidgetSim> widget;

	widget.reset(new WidgetSim);

	RefPtr<Object> obj = RefPtr<Object>::cast_dynamic(widget);

	RefPtr<Object> obj2;

	obj2.operator=<WidgetSim>(widget);
	
	RefPtr<Object> obj3;
	obj3 = widget;

    RefPtr<Object> obj4(widget);

    ASSERT_TRUE(obj4->reference_count() == 5);
}

/**
 * Test RefPtr::destroy
 */
TEST_F(ObjectTest1, Children1)
{
    WidgetSimPtr widget1(new WidgetSim);

    {
        WidgetSimPtr widget2(new WidgetSim);
        WidgetSimPtr widget3(new WidgetSim);

        widget1->AddChild(widget2);
        widget1->AddChild(widget3);
    }

    widget1->print();

    ASSERT_TRUE(true);
}

