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
 * 	column become 5 (default) + 2
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

	ASSERT_TRUE(model.GetColumns(root) == 7);
}

/**
 * test InsertColumns() method
 *
 * Insert 2 rows into an empty model, and add 2 another columns at 2
 *
 * Expected result:
 *
 * 	column become 5 (default) + 2
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

	ASSERT_TRUE(model.GetColumns(root) == 7);
}

/**
 * test InsertColumns() method
 *
 * Insert 2 rows into an empty model, and append 2 another columns
 *
 * Expected result:
 *
 * 	column become 5 (default) + 2
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

	ASSERT_TRUE(model.GetColumns(root) == 7);
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

/**
 * test RemoveColumns() method
 *
 * Insert 3 rows into an empty model, and try to remove 1 columns at 0
 *
 * Expected result:
 *
 * 	column count is 4
 */
TEST_F(FileSystemModelTest1, RemoveColumns1)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();
	model.InsertRows(0, 3, root);

	model.RemoveColumns(0, 1, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetColumns(root) == 4);
}

/**
 * test RemoveColumns() method
 *
 * Insert 5 rows into an empty model, and try to remove 1 columns at 1
 *
 * Expected result:
 *
 * 	column count is 4
 */
TEST_F(FileSystemModelTest1, RemoveColumns2)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();
	model.InsertRows(0, 3, root);

	model.RemoveColumns(1, 1, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetColumns(root) == 4);
}

/**
 * test RemoveColumns() method
 *
 * Insert 5 rows into an empty model, and try to remove 1 columns at 4
 *
 * Expected result:
 *
 * 	column count is 4
 */
TEST_F(FileSystemModelTest1, RemoveColumns3)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();
	model.InsertRows(0, 3, root);

	model.RemoveColumns(4, 1, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetColumns(root) == 4);
}

/**
 * test RemoveColumns() method
 *
 * Insert 3 rows into an empty model, and try to remove > 5 columns at 1
 *
 * Expected result:
 *
 * 	column count is 1
 */
TEST_F(FileSystemModelTest1, RemoveColumns4)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();
	model.InsertRows(0, 3, root);

	model.RemoveColumns(1, 9, root);

#ifdef DEBUG
	model.Print();
#endif

	DBG_PRINT_MSG("columns: %d", model.GetColumns(root));

	ASSERT_TRUE(model.GetColumns(root) == 1);
}

/**
 * test RemoveColumns() method
 *
 * Insert 3 rows into an empty model, and try to remove 6 columns at 0, this remove all data
 *
 * Expected result:
 *
 * 	column count is 0
 */
TEST_F(FileSystemModelTest1, RemoveColumns5)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();
	model.InsertRows(0, 3, root);

	model.RemoveColumns(0, 6, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE((model.GetColumns(root) == 0) && (model.GetRows(root) == 0));
}

/**
 * test RemoveColumns() method
 *
 * Insert 3 rows into an empty model, and try to remove 7 columns at 0, this remove all data
 *
 * Expected result:
 *
 * 	column count is 0
 */
TEST_F(FileSystemModelTest1, RemoveColumns6)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();
	model.InsertRows(0, 3, root);

	model.RemoveColumns(0, 7, root);

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE((model.GetColumns(root) == 0) && (model.GetRows(root) == 0));
}

/**
 * test Load() method
 *
 * Load file list in a given path
 *
 * Expected result:
 *
 * 	see print
 */
TEST_F(FileSystemModelTest1, Load1)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();
	model.Load(".");

#ifdef DEBUG
	model.Print();
#endif

	ASSERT_TRUE(model.GetColumns(root) == 5);
}

/**
 * test GetIndex() method
 *
 * Load file list in a given path, use GetIndex to check the index data
 *
 * Expected result:
 *
 * 	see print
 */
TEST_F(FileSystemModelTest1, GetIndex1)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();

	model.Load(".");

	ModelIndex index = model.GetIndex(0, 0, root);

	while(index.IsValid()) {
		DBG_PRINT_MSG("%s", ConvertFromString(*index.GetData()).c_str());
		index = index.GetDownIndex();
	}

	ASSERT_TRUE(model.GetColumns(root) == 5);
}

/**
 * test GetIndex() method
 *
 * Load file list in a given path, use GetIndex to check the index data
 *
 * Expected result:
 *
 * 	see print
 */
TEST_F(FileSystemModelTest1, GetIndex2)
{
	FileSystemModel model;

	ModelIndex root = model.GetRootIndex();

	model.Load(".");

	ModelIndex index = model.GetIndex(3, 0, root);

	while(index.IsValid()) {
		DBG_PRINT_MSG("%s", ConvertFromString(*index.GetData()).c_str());
		index = index.GetRightIndex();
	}

	ASSERT_TRUE(model.GetColumns(root) == 5);
}
