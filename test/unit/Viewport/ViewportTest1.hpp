#ifndef _VIEWPORTTEST1_HPP_
#define _VIEWPORTTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class ViewportTest1: public testing::Test
{
public:
	ViewportTest1 ();
	virtual ~ViewportTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _VIEWPORTTEST1_HPP_
