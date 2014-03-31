#ifndef _SCROLLBARTEST1_HPP_
#define _SCROLLBARTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class ScrollBarTest1: public testing::Test
{
public:
	ScrollBarTest1 ();
	virtual ~ScrollBarTest1 ();

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

#endif  // _SCROLLBARTEST1_HPP_
