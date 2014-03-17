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
#ifdef DEBUG
		ASSERT_TRUE(BlendInt::Object::CheckAllocatedObjects());
#endif
	}

	virtual void TearDown ()
	{
#ifdef DEBUG
		ASSERT_TRUE(BlendInt::Object::CheckAllocatedObjects());
#endif
	}
};

#endif  // _ABSTRACTFORMTEST1_HPP_
