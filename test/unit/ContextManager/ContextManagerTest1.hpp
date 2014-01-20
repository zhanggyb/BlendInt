#ifndef _CONTEXTMANAGERTEST1_HPP_
#define _CONTEXTMANAGERTEST1_HPP_

#include <gtest/gtest.h>

#include <BlendInt/Object.hpp>

class ContextManagerTest1: public testing::Test
{
public:
	ContextManagerTest1 ();
	virtual ~ContextManagerTest1 ();

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

#endif  // _CONTEXTMANAGERTEST1_HPP_
