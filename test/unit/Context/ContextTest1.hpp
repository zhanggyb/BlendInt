#ifndef _ContextTest1_HPP_
#define _ContextTest1_HPP_

#include <gtest/gtest.h>

#include <BlendInt/Core/Object.hpp>

class ContextTest1: public testing::Test
{
public:
	ContextTest1 ();
	virtual ~ContextTest1 ();

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

#endif  // _ContextTest1_HPP_
