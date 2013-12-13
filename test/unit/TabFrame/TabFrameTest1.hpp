#ifndef _TABFRAMETEST1_HPP_
#define _TABFRAMETEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class TabFrameTest1: public testing::Test
{
public:
	TabFrameTest1 ();
	virtual ~TabFrameTest1 ();

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

#endif  // _TABFRAMETEST1_HPP_
