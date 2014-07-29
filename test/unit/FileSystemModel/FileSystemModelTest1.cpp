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
 * test InsertRows() method
 *
 * Insert 5 rows into an empty model
 * Expected result:
 * 	check the row created and make sure all data are deleted in the destructor
 */
TEST_F(FileSystemModelTest1, InsertRows1)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();
	model.InsertRows(0, 5, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetRows(root) == 5);
}

/**
 * test InsertRows() method
 *
 * Insert 5 rows into an empty model, and insert 2 another rows at 0
 *
 * Expected result: 
 *
 * 	row become 7
 */
TEST_F(FileSystemModelTest1, InsertRows2)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();

	model.InsertRows(0, 5, root);

	model.InsertRows(0, 2, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetRows(root) == 7);
}

/**
 * test InsertRows() method
 *
 * Insert 5 rows into an empty model, and insert 2 another rows at 2
 *
 * Expected result:
 *
 * 	row become 7
 */
TEST_F(FileSystemModelTest1, InsertRows3)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();

	model.InsertRows(0, 5, root);

	model.InsertRows(2, 2, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetRows(root) == 7);
}

/**
 * test InsertRows() method
 *
 * Insert 5 rows into an empty model, and append 2 another rows
 *
 * Expected result:
 *
 * 	row become 7
 */
TEST_F(FileSystemModelTest1, InsertRows4)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();

	model.InsertRows(0, 5, root);

	model.InsertRows(5, 2, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetRows(root) == 7);
}

/**
 * test InsertRows() method
 *
 * Insert 5 rows into an empty model, and append 5 another rows at 2
 *
 * Expected result:
 *
 * 	row become 10
 */
TEST_F(FileSystemModelTest1, InsertRows5)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();

	model.InsertRows(0, 5, root);

	model.InsertRows(2, 5, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetRows(root) == 10);
}

/**
 * test InsertColumns() method
 *
 * Insert 2 rows into an empty model, and add 2 another columns at 0
 *
 * Expected result:
 *
 * 	column become 3 (default) + 2
 */
TEST_F(FileSystemModelTest1, InsertColumns1)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();

	model.InsertRows(0, 2, root);

	model.InsertColumns(0, 2, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetColumns(root) == 5);
}

/**
 * test InsertColumns() method
 *
 * Insert 2 rows into an empty model, and add 2 another columns at 2
 *
 * Expected result:
 *
 * 	column become 3 (default) + 2
 */
TEST_F(FileSystemModelTest1, InsertColumns2)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();

	model.InsertRows(0, 2, root);

	model.InsertColumns(2, 2, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetColumns(root) == 5);
}

/**
 * test InsertColumns() method
 *
 * Insert 2 rows into an empty model, and append 2 another columns
 *
 * Expected result:
 *
 * 	column become 3 (default) + 2
 */
TEST_F(FileSystemModelTest1, InsertColumns3)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();

	model.InsertRows(0, 2, root);

	model.InsertColumns(2, 2, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetColumns(root) == 5);
}

/**
 * test InsertColumns() method
 *
 * Insert 5 rows into an empty model, and remove 2 rows at 0
 *
 * Expected result:
 *
 * 	rows is 3 now
 */
TEST_F(FileSystemModelTest1, RemoveRows1)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();

	model.InsertRows(0, 5, root);

	model.RemoveRows(0, 2, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetRows(root) == 3);
}

/**
 * test InsertColumns() method
 *
 * Insert 5 rows into an empty model, and try to remove 6 rows at 1
 *
 * Expected result:
 *
 * 	row count is 1 now
 */
TEST_F(FileSystemModelTest1, RemoveRows2)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();

	model.InsertRows(0, 5, root);

	model.RemoveRows(1, 6, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetRows(root) == 1);
}

/**
 * test InsertColumns() method
 *
 * Insert 5 rows into an empty model, and try to remove 3 rows at 3
 *
 * Expected result:
 *
 * 	row count is 3 now
 */
TEST_F(FileSystemModelTest1, RemoveRows3)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();

	model.InsertRows(0, 5, root);

	model.RemoveRows(3, 3, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetRows(root) == 3);
}

/**
 * test InsertColumns() method
 *
 * Insert 5 rows into an empty model, and try to remove 2 rows at 6
 *
 * Expected result:
 *
 * 	nothing happen, row count is still 5
 */
TEST_F(FileSystemModelTest1, RemoveRows4)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();

	model.InsertRows(0, 5, root);

	model.RemoveRows(6, 2, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetRows(root) == 5);
}
