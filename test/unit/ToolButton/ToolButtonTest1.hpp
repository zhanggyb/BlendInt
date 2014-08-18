#ifndef _TOOLBUTTONTEST1_HPP_
#define _TOOLBUTTONTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class ToolButtonTest1: public testing::Test
{
public:
	ToolButtonTest1 ();
	virtual ~ToolButtonTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _TOOLBUTTONTEST1_HPP_
