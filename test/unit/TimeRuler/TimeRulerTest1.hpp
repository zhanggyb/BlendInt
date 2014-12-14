#ifndef _TIMERULERTEST1_HPP_
#define _TIMERULERTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class TimeRulerTest1: public testing::Test
{
public:
	TimeRulerTest1 ();
	virtual ~TimeRulerTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _TIMERULERTEST1_HPP_
