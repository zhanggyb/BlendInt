#ifndef _ABSTRACTFORMTEST1_HPP_
#define _ABSTRACTFORMTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class AbstractFormTest1: public testing::Test
{
public:
	AbstractFormTest1 ();
	virtual ~AbstractFormTest1 ();

protected:

	virtual void SetUp ()
	{

	}

	virtual void TearDown ()
	{

	}
};

#endif  // _ABSTRACTFORMTEST1_HPP_
