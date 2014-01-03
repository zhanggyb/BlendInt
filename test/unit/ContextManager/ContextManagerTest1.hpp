#ifndef _CONTEXTMANAGERTEST1_HPP_
#define _CONTEXTMANAGERTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class ContextManagerTest1: public testing::Test
{
public:
	ContextManagerTest1 ();
	virtual ~ContextManagerTest1 ();

protected:

	virtual void SetUp ()
	{
		ASSERT_TRUE(BlendInt::CheckAllocatedObjects());
	}

	virtual void TearDown ()
	{
		ASSERT_TRUE(BlendInt::CheckAllocatedObjects());
	}
};

#endif  // _CONTEXTMANAGERTEST1_HPP_
