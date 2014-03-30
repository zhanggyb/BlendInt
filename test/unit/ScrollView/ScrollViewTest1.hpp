#ifndef _SCROLLVIEWTEST1_HPP_
#define _SCROLLVIEWTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class ScrollViewTest1: public testing::Test
{
public:
	ScrollViewTest1 ();
	virtual ~ScrollViewTest1 ();

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

#endif  // _SCROLLVIEWTEST1_HPP_
