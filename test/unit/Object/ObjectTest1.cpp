#include "ObjectTest1.hpp"
#include <BlendInt/Core/Object.hpp>

#include <iostream>

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
TEST_F(ObjectTest1, New1)
{
	// TODO: add test code here
    RefPtr<Object> obj1 = Object::Create();

    RefPtr<Object> obj2 = Object::Create();

    obj2 = obj1;

    std::cout << "Object ref count: " << obj1->count() << std::endl;

	ASSERT_TRUE(true);
}

