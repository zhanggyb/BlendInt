#ifndef _BUTTONGROUPTEST1_HPP_
#define _BUTTONGROUPTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class ButtonGroupTest1: public testing::Test
{
public:
	ButtonGroupTest1 ();
	virtual ~ButtonGroupTest1 ();

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

#endif  // _BUTTONGROUPTEST1_HPP_
