#include "ListModelTest1.hpp"
#include <BlendInt/Gui/ListModel.hpp>
#include <BlendInt/Gui/FileSystemModel.hpp>

using namespace BlendInt;

ListModelTest1::ListModelTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ListModelTest1::~ListModelTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ListModelTest1, Foo1)
{
	ListModel model;

	ItemDeque* dec = new ItemDeque;
	dec->push_back(String("(0, 0)"));
	dec->push_back(String("(0, 1)"));
	dec->push_back(String("(0, 2)"));

	model.PushBack(dec);

	dec = new ItemDeque;
	dec->push_back(String("(1, 0)"));
	dec->push_back(String("(1, 1)"));
//	dec->push_back(String("(2, 2)"));
//	dec->push_back(String("(2, 3)"));

	model.PushBack(dec);

	model.Print();

	FileSystemModel fs_model;
	fs_model.Load("/home/zhanggyb");

#ifdef DEBUG
	fs_model.Print();
#endif

	ASSERT_TRUE(true);
}
