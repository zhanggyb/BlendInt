#ifndef _PERFORMANCETEST1_HPP_
#define _PERFORMANCETEST1_HPP_

#include <gtest/gtest.h>
#include <BlendInt/Core/Object.hpp>

class PerformanceTest1: public testing::Test
{
public:
	PerformanceTest1 ();
	virtual ~PerformanceTest1 ();

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

#endif  // _PERFORMANCETEST1_HPP_
