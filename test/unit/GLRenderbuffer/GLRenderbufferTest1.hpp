#ifndef _GLRENDERBUFFERTEST1_HPP_
#define _GLRENDERBUFFERTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class GLRenderbufferTest1: public testing::Test
{
public:
	GLRenderbufferTest1 ();
	virtual ~GLRenderbufferTest1 ();

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

#endif  // _GLRENDERBUFFERTEST1_HPP_
