#ifndef _WINDOWTEST1_HPP_
#define _WINDOWTEST1_HPP_

#include <gtest/gtest.h>
#include <BlendInt/Core/Object.hpp>

class WindowTest1: public testing::Test
{
public:
	WindowTest1 ();
	virtual ~WindowTest1 ();

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

#endif  // _WINDOWTEST1_HPP_
