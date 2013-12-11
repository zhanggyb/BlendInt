#ifndef _HORIZONTALLAYOUTTEST1_HPP_
#define _HORIZONTALLAYOUTTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class HorizontalLayoutTest1: public testing::Test
{
public:
	HorizontalLayoutTest1 ();
	virtual ~HorizontalLayoutTest1 ();

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

#endif  // _HORIZONTALLAYOUTTEST1_HPP_
