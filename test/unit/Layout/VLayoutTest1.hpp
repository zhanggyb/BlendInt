/*
 * VLayoutTest1.hpp
 *
 *  Created on: 2013Äê12ÔÂ7ÈÕ
 *      Author: zhanggy
 */

#ifndef VLAYOUTTEST1_HPP_
#define VLAYOUTTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class VLayoutTest1: public testing::Test {
public:
	VLayoutTest1();
	virtual ~VLayoutTest1();

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

#endif /* VLAYOUTTEST1_HPP_ */
