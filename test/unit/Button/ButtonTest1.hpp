#ifndef _BUTTONTEST1_HPP_
#define _BUTTONTEST1_HPP_

#include <gtest/gtest.h>
#include <BlendInt/Core/Object.hpp>

class ButtonTest1: public testing::Test
{
public:
	ButtonTest1 ();
	virtual ~ButtonTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _BUTTONTEST1_HPP_
