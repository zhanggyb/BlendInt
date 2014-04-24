#ifndef _BUTTONTEST1_HPP_
#define _BUTTONTEST1_HPP_

#include <gtest/gtest.h>
#include <BlendInt/Core/Object.hpp>

class ButtonTest1: public testing::Test
{
public:
	ButtonTest1 ();
	virtual ~ButtonTest1 ();

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

#endif  // _BUTTONTEST1_HPP_
