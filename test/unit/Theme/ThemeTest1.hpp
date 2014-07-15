#ifndef _OBJECTTEST1_HPP_
#define _OBJECTTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class ThemeTest1: public testing::Test
{
public:
	ThemeTest1 ();
	virtual ~ThemeTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _OBJECTTEST1_HPP_
