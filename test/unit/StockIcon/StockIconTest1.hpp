#ifndef _STOCKICONTEST1_HPP_
#define _STOCKICONTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class StockIconTest1: public testing::Test
{
public:
	StockIconTest1 ();
	virtual ~StockIconTest1 ();

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

#endif  // _STOCKICONTEST1_HPP_
