#ifndef _FILESYSTEMMODELTEST1_HPP_
#define _FILESYSTEMMODELTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class FileSystemModelTest1: public testing::Test
{
public:
	FileSystemModelTest1 ();
	virtual ~FileSystemModelTest1 ();

protected:

	virtual void SetUp ()
	{

	}

	virtual void TearDown ()
	{
	}
};

#endif  // _FILESYSTEMMODELTEST1_HPP_
