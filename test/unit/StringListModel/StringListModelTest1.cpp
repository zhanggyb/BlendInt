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
 * test StringListModel::InsertRows() method
 *
 * Expected result: 
 */
TEST_F(StringListModelTest1, Insert1)
{
	StringListModel model;

	ModelIndex index = model.GetRootIndex();

	model.InsertRows(0, 5, index);

	model.InsertRows(8, 5, index);

	model.Print();

	ASSERT_TRUE(model.GetRows() == 10);
}

/**
 * test StringListModel::RemoveRows() method
 *
 * Expected result: 
 */
TEST_F(StringListModelTest1, Remove1)
{
	StringListModel model;

	ModelIndex index = model.GetRootIndex();

	model.InsertRows(0, 5, index);

	model.Print();

	model.RemoveRows(2, 2, index);
	
	model.Print();

	ASSERT_TRUE(model.GetRows() == 3);
}
