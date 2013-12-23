#ifndef _MENUTEST1_HPP_
#define _MENUTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class MenuTest1: public testing::Test
{
public:
	MenuTest1 ();
	virtual ~MenuTest1 ();

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

#endif  // _MENUTEST1_HPP_
