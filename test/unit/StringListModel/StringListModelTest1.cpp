#include "StringListModelTest1.hpp"
#include <BlendInt/Gui/StringListModel.hpp>

using namespace BlendInt;

StringListModelTest1::StringListModelTest1()
: testing::Test()
{
	// TODO: add constructor code
}

StringListModelTest1::~StringListModelTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(StringListModelTest1, Foo1)
{
	StringListModel model;

	ModelIndex index = model.GetRootIndex();

	model.InsertRows(0, 5, index);

	model.InsertRows(8, 5, index);

	model.Print();

	ASSERT_TRUE(model.GetRows() == 10);
}
