#ifndef _BUTTONBOXTEST1_HPP_
#define _BUTTONBOXTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class ButtonBoxTest1: public testing::Test
{
public:
	ButtonBoxTest1 ();
	virtual ~ButtonBoxTest1 ();

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

#endif  // _BUTTONBOXTEST1_HPP_
