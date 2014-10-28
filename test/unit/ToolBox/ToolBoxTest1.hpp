#ifndef _TOOLBOXTEST1_HPP_
#define _TOOLBOXTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class ToolBoxTest1: public testing::Test
{
public:
	ToolBoxTest1 ();
	virtual ~ToolBoxTest1 ();

protected:

	virtual void SetUp ()
	{

	}

	virtual void TearDown ()
	{

	}
};

#endif  // _TOOLBOXTEST1_HPP_
