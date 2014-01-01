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

    Object::Print();

    delete obj1;

    std::cout << "size of Object: " << sizeof(Object) << std::endl;

	ASSERT_TRUE(Object::GetMapSize() == 0);
}

/**
 * test Object::Bind(Object* sub) method
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, Bind1)
{
	// TODO: add test code here
    Object* obj1 = new Object;
    obj1->set_name("obj1");

    Object* obj2 = new Object;
    obj2->set_name("obj2");
 
    obj1->Bind(obj2);

    Object::Print();

    obj1->PrintSubs();
    obj2->PrintSupers();

    delete obj1;

	ASSERT_TRUE(Object::GetMapSize() == 0);
}

/**
 * test Object::Bind(Object* sub) method
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, Bind2)
{
	// TODO: add test code here
    Object* obj1 = new Object;
    obj1->set_name("obj1");

    Object* obj2 = new Object;
    obj2->set_name("obj2");
 
    Object* obj3 = new Object;
    obj3->set_name("obj3");
   
    Object* obj4 = new Object;
    obj4->set_name("obj4");
    
    obj1->Bind(obj2);
    obj1->Bind(obj3);
    obj1->Bind(obj4);

    Object::Print();

    obj1->PrintSubs();
    obj2->PrintSupers();
    obj3->PrintSupers();
    obj4->PrintSupers();

    delete obj1;

	ASSERT_TRUE(Object::GetMapSize() == 0);
}

/**
 * test Object::Unbind(Object* sub) method
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, Unbind1)
{
	// TODO: add test code here
    Object* obj1 = new Object;
    obj1->set_name("obj1");

    Object* obj2 = new Object;
    obj2->set_name("obj2");
 
    Object* obj3 = new Object;
    obj3->set_name("obj3");
   
    Object* obj4 = new Object;
    obj4->set_name("obj4");
    
    obj1->Bind(obj2);
    obj1->Bind(obj3);
    obj1->Bind(obj4);

    obj1->PrintSubs();
    obj2->PrintSupers();
    obj3->PrintSupers();
    obj4->PrintSupers();

    obj1->Unbind(obj2);
    obj1->Unbind(obj3);
    obj1->Unbind(obj4);

    obj1->PrintSubs();
    obj2->PrintSupers();
    obj3->PrintSupers();
    obj4->PrintSupers();

    delete obj1;
    delete obj2;
    delete obj3;
    delete obj4;

	ASSERT_TRUE(Object::GetMapSize() == 0);
}

/**
 * test Object::BindTo(Object* super) and UnbindFrom method
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, BindTo1)
{
	// TODO: add test code here
    Object* obj1 = new Object;
    obj1->set_name("obj1");

    Object* obj2 = new Object;
    obj2->set_name("obj2");
 
    Object* obj3 = new Object;
    obj3->set_name("obj3");
   
    Object* obj4 = new Object;
    obj4->set_name("obj4");
    
    obj2->BindTo(obj1);
    obj3->BindTo(obj1);
    obj4->BindTo(obj1);

    obj1->PrintSubs();
    obj2->PrintSupers();
    obj3->PrintSupers();
    obj4->PrintSupers();

    obj2->UnbindFrom(obj1);
    obj3->UnbindFrom(obj1);
    obj4->UnbindFrom(obj1);

    obj1->PrintSubs();
    obj2->PrintSupers();
    obj3->PrintSupers();
    obj4->PrintSupers();

    delete obj1;
    delete obj2;
    delete obj3;
    delete obj4;

	ASSERT_TRUE(Object::GetMapSize() == 0);
}

/**
 * test multi bind
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, MultiBind1)
{
	// TODO: add test code here
    Object* obj1 = new Object;
    obj1->set_name("obj1");

    Object* obj2 = new Object;
    obj2->set_name("obj2");
 
    Object* obj3 = new Object;
    obj3->set_name("obj3");
   
    Object* obj4 = new Object;
    obj4->set_name("obj4");
    
   	obj1->Bind(obj3);
	obj2->Bind(obj3);
	obj3->Bind(obj4);

    obj1->PrintSubs();
    obj2->PrintSubs();
    obj3->PrintSupers();
    obj4->PrintSupers();

    delete obj1;
    delete obj2;

	ASSERT_TRUE(Object::GetMapSize() == 0);
}

/**
 * test multi bind
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, LoopBind1)
{
	// TODO: add test code here
    Object* obj1 = new Object;
    obj1->set_name("obj1");

    Object* obj2 = new Object;
    obj2->set_name("obj2");
 
    Object* obj3 = new Object;
    obj3->set_name("obj3");
   
    Object* obj4 = new Object;
    obj4->set_name("obj4");
    
   	obj1->Bind(obj2);
	obj2->Bind(obj3);
	obj3->Bind(obj4);
	obj4->Bind(obj1);

    obj1->PrintSubs();
    obj2->PrintSubs();
    obj3->PrintSubs();
    obj4->PrintSubs();

    delete obj1;

	ASSERT_TRUE(Object::GetMapSize() == 0);
}