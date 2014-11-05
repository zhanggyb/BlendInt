#ifndef _CLOCKTEST1_HPP_
#define _CLOCKTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class ClockTest1: public testing::Test
{
public:
	ClockTest1 ();
	virtual ~ClockTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _CLOCKTEST1_HPP_
