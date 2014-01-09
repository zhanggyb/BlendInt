#ifndef _BUTTONTEST1_HPP_
#define _BUTTONTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class ButtonTest1: public testing::Test
{
public:
	ButtonTest1 ();
	virtual ~ButtonTest1 ();

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

#endif  // _BUTTONTEST1_HPP_
