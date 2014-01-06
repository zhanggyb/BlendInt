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
    Object obj1;
    obj1.set_name("obj1");

    std::cout << "obj1 ref count: " << obj1.GetReferenceCount() << std::endl;
	
	Object* obj2 = new Object;
	
	obj2->set_name("obj2");
	
    std::cout << "obj2 ref count: " << obj2->GetReferenceCount() << std::endl;

	delete obj2;

	ASSERT_TRUE(true);
}


/**
 * test new Object 
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, New2)
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
 * test new Object
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, New3)
{
	// TODO: add test code here
    Object* obj1 = new Object;
    obj1->set_name("obj1");

    Object* obj2 = new Object(obj1);
    obj2->set_name("obj2");
    
    obj1->PrintSubs();
    obj2->PrintSupers();
    
    delete obj1;

	ASSERT_TRUE(Object::GetMapSize() == 0);
}


/**
 * test Object::Attach(Object* sub) method
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, Attach1)
{
	// TODO: add test code here
    Object* obj1 = new Object;
    obj1->set_name("obj1");

    Object* obj2 = new Object;
    obj2->set_name("obj2");
 
    obj1->Attach(obj2);

    Object::Print();

    obj1->PrintSubs();
    obj2->PrintSupers();

    delete obj1;

	ASSERT_TRUE(Object::GetMapSize() == 0);
}

/**
 * test Object::Attach(Object* sub) method
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, Attach2)
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
    
    obj1->Attach(obj2);
    obj1->Attach(obj3);
    obj1->Attach(obj4);

    Object::Print();

    obj1->PrintSubs();
    obj2->PrintSupers();
    obj3->PrintSupers();
    obj4->PrintSupers();

    delete obj1;

	ASSERT_TRUE(Object::GetMapSize() == 0);
}

/**
 * test Object::Detach(Object* sub) method
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, Detach1)
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
    
    obj1->Attach(obj2);
    obj1->Attach(obj3);
    obj1->Attach(obj4);

    obj1->PrintSubs();
    obj2->PrintSupers();
    obj3->PrintSupers();
    obj4->PrintSupers();

    obj1->Detach(obj2);
    obj1->Detach(obj3);
    obj1->Detach(obj4);

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
 * test Object::AttachTo(Object* super) and DetachFrom method
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, AttachTo1)
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
    
    obj2->AttachTo(obj1);
    obj3->AttachTo(obj1);
    obj4->AttachTo(obj1);

    obj1->PrintSubs();
    obj2->PrintSupers();
    obj3->PrintSupers();
    obj4->PrintSupers();

    obj2->DetachFrom(obj1);
    obj3->DetachFrom(obj1);
    obj4->DetachFrom(obj1);

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
TEST_F(ObjectTest1, MultiAttach1)
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
    
   	obj1->Attach(obj3);
	obj2->Attach(obj3);
	obj3->Attach(obj4);

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
TEST_F(ObjectTest1, LoopAttach1)
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
    
   	obj1->Attach(obj2);
	obj2->Attach(obj3);
	obj3->Attach(obj4);
	obj4->Attach(obj1);

    obj1->PrintSubs();
    obj2->PrintSubs();
    obj3->PrintSubs();
    obj4->PrintSubs();

    delete obj1;

	ASSERT_TRUE(Object::GetMapSize() == 0);
}

/**
 * test unbindall
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, DetachAll1)
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
    
   	obj1->Attach(obj2);
	obj1->Attach(obj3);
	obj1->Attach(obj4);

    obj1->PrintSubs();
    obj2->PrintSupers();
    obj3->PrintSupers();
    obj4->PrintSupers();

	obj1->DetachAllSubs();

    delete obj1;
	delete obj2;
	delete obj3;
	delete obj4;

	ASSERT_TRUE(Object::GetMapSize() == 0);
}

/**
 * test unbindall
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, DetachFromAllSupers1)
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
    
   	obj1->Attach(obj4);
	obj2->Attach(obj4);
	obj3->Attach(obj4);

    obj1->PrintSubs();
    obj2->PrintSubs();
    obj3->PrintSubs();
    obj4->PrintSupers();

	obj4->DetachFromAllSupers();

    delete obj1;
	delete obj2;
	delete obj3;
	delete obj4;

	ASSERT_TRUE(Object::GetMapSize() == 0);
}

/**
 * test mix bind
 *
 * Expected result: 
 */
TEST_F(ObjectTest1, MixAttach1)
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

    Object* obj5 = new Object;
    obj5->set_name("obj5");
    
    Object* obj6 = new Object;
    obj6->set_name("obj6");

   	obj1->Attach(obj2);
	obj2->Attach(obj3);
	obj3->Attach(obj4);

	obj5->Attach(obj2);
	obj6->Attach(obj5);

    delete obj1;
	delete obj6;

	ASSERT_TRUE(Object::GetMapSize() == 0);
}
