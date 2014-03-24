#ifndef _MENUBUTTONTEST1_HPP_
#define _MENUBUTTONTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class MenuButtonTest1: public testing::Test
{
public:
	MenuButtonTest1 ();
	virtual ~MenuButtonTest1 ();

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

#endif  // _MENUBUTTONTEST1_HPP_
