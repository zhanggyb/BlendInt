#ifndef _HORIZONTALLAYOUTTEST1_HPP_
#define _HORIZONTALLAYOUTTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class HLayoutTest1: public testing::Test
{
public:
	HLayoutTest1 ();
	virtual ~HLayoutTest1 ();

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
