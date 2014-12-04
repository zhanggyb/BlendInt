#ifndef _TIMETEST1_HPP_
#define _TIMETEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class TimeTest1: public testing::Test
{
public:
	TimeTest1 ();
	virtual ~TimeTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _TIMETEST1_HPP_
