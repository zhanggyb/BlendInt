#include "FileSystemModelTest1.hpp"
#include <BlendInt/Gui/FileSystemModel.hpp>

using namespace BlendInt;

FileSystemModelTest1::FileSystemModelTest1()
: testing::Test()
{
	// TODO: add constructor code
}

FileSystemModelTest1::~FileSystemModelTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(FileSystemModelTest1, Foo1)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();

	model.InsertRows(0, 5, root);

	model.RemoveRows(0, 3, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(true);
}
