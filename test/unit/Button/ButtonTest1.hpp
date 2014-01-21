#ifndef _BUTTONTEST1_HPP_
#define _BUTTONTEST1_HPP_

#include <gtest/gtest.h>
#include <BlendInt/Object.hpp>

class ButtonTest1: public testing::Test
{
public:
	ButtonTest1 ();
	virtual ~ButtonTest1 ();

protected:

	virtual void SetUp ()
	{
		ASSERT_TRUE(BlendInt::Object::CheckAllocatedObjects());
	}

	virtual void TearDown ()
	{
		ASSERT_TRUE(BlendInt::Object::CheckAllocatedObjects());
	}
};

#endif  // _BUTTONTEST1_HPP_
