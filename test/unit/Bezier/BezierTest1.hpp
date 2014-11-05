#ifndef _BEZIERTEST1_HPP_
#define _BEZIERTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class BezierTest1: public testing::Test
{
public:
	BezierTest1 ();
	virtual ~BezierTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _BEZIERTEST1_HPP_
