#ifndef _COMBOBOXTEST1_HPP_
#define _COMBOBOXTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class ColorWheelTest1: public testing::Test
{
public:
	ColorWheelTest1 ();
	virtual ~ColorWheelTest1 ();

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

#endif  // _COMBOBOXTEST1_HPP_
