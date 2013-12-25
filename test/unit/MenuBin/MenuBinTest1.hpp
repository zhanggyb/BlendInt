#ifndef _MENUBINTEST1_HPP_
#define _MENUBINTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class MenuBinTest1: public testing::Test
{
public:
	MenuBinTest1 ();
	virtual ~MenuBinTest1 ();

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

#endif  // _MENUBINTEST1_HPP_
