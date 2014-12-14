#ifndef _THREADTEST1_HPP_
#define _THREADTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class ThreadTest1: public testing::Test
{
public:
	ThreadTest1 ();
	virtual ~ThreadTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _THREADTEST1_HPP_
