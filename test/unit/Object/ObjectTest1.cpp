#include "ObjectTest1.hpp"
#include <BlendInt/Object.hpp>

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
    Object* obj1 = new Object;
    obj1->set_name("obj1");

    std::cout << "obj1 ref count: " << obj1->ref_count() << std::endl;
	
	Object* obj2 = new Object;
	
	obj2->set_name("obj2");
	
    std::cout << "obj2 ref count: " << obj2->ref_count() << std::endl;

    Object::Destroy(obj2);

#ifdef DEBUG
    Object::CountOnce (obj1);
    Object::CountOnce (obj1);
    std::cout << "obj1 ref count: " << obj1->ref_count() << std::endl;
    Object::Destroy(obj1);
    Object::Destroy(obj1);
#endif

    std::cout << "size of object: " << sizeof(Object) << std::endl;

	ASSERT_TRUE(true);
}

