#ifndef _TOGGLEBUTTONTEST1_HPP_
#define _TOGGLEBUTTONTEST1_HPP_

#include <gtest/gtest.h>
#include <BlendInt/Window.hpp>
#include <BlendInt/Object.hpp>

class ToggleButtonTest1: public testing::Test
{
public:
	ToggleButtonTest1 ();
	virtual ~ToggleButtonTest1 ();

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

#endif  // _TOGGLEBUTTONTEST1_HPP_
