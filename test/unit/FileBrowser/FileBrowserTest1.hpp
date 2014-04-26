#ifndef _FILEBROWSERTEST1_HPP_
#define _FILEBROWSERTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class FileBrowserTest1: public testing::Test
{
public:
	FileBrowserTest1 ();
	virtual ~FileBrowserTest1 ();

protected:

	virtual void SetUp ()
	{
#ifdef DEBUG
		ASSERT_TRUE(BlendInt::Object::CheckAllocatedObjects());
#endif
	}

	virtual void TearDown ()
	{
#ifdef DEBUG
		ASSERT_TRUE(BlendInt::Object::CheckAllocatedObjects());
#endif
	}
};

#endif  // _FILEBROWSERTEST1_HPP_
