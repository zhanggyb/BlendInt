#ifndef _FILEBUTTONTEST1_HPP_
#define _FILEBUTTONTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class FileButtonTest1: public testing::Test
{
public:
	FileButtonTest1 ();
	virtual ~FileButtonTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _FILEBUTTONTEST1_HPP_
