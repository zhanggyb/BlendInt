#ifndef _OBJECTTEST1_HPP_
#define _OBJECTTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class ObjectTest1: public testing::Test
{
public:
	ObjectTest1 ();
	virtual ~ObjectTest1 ();

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

#endif  // _OBJECTTEST1_HPP_
