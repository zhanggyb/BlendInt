#ifndef _OBJECTTEST1_HPP_
#define _OBJECTTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class ThemeTest1: public testing::Test
{
public:
	ThemeTest1 ();
	virtual ~ThemeTest1 ();

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
