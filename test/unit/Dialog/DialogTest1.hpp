#ifndef _DIALOGTEST1_HPP_
#define _DIALOGTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class DialogTest1: public testing::Test
{
public:
	DialogTest1 ();
	virtual ~DialogTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _DIALOGTEST1_HPP_
