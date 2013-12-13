#ifndef _BOXFRAMETEST1_HPP_
#define _BOXFRAMETEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class BoxFrameTest1: public testing::Test
{
public:
	BoxFrameTest1 ();
	virtual ~BoxFrameTest1 ();

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

#endif  // _BOXFRAMETEST1_HPP_
