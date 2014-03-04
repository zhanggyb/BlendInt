#ifndef _VERTICALLAYOUTTEST1_HPP_
#define _VERTICALLAYOUTTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class VLayoutTest1: public testing::Test
{
public:
	VLayoutTest1 ();
	virtual ~VLayoutTest1 ();

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

#endif  // _VERTICALLAYOUTTEST1_HPP_
