#ifndef _WINDOWTEST1_HPP_
#define _WINDOWTEST1_HPP_

#include <Common/Window.hpp>
#include <gtest/gtest.h>

class WindowTest1: public testing::Test
{
public:
	WindowTest1 ();
	virtual ~WindowTest1 ();

protected:

	virtual void SetUp ()
	{
		ASSERT_TRUE(BlendInt::CheckAllocatedObjects());
	}

	virtual void TearDown ()
	{
		ASSERT_TRUE(BlendInt::CheckAllocatedObjects());
	}
};

#endif  // _WINDOWTEST1_HPP_
